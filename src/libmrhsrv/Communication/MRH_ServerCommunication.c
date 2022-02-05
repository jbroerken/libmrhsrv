/**
 *  libmrhsrv
 *  Copyright (C) 2021 - 2022 Jens Brörken
 *
 *  This software is provided 'as-is', without any express or implied
 *  warranty.  In no event will the authors be held liable for any damages
 *  arising from the use of this software.
 *
 *  Permission is granted to anyone to use this software for any purpose,
 *  including commercial applications, and to alter it and redistribute it
 *  freely, subject to the following restrictions:
 *
 *  1. The origin of this software must not be misrepresented; you must not
 *     claim that you wrote the original software. If you use this software
 *     in a product, an acknowledgment in the product documentation would be
 *     appreciated but is not required.
 *
 *  2. Altered source versions must be plainly marked as such, and must not be
 *     misrepresented as being the original software.
 *
 *  3. This notice may not be removed or altered from any source distribution.
 */

// C
#include <time.h>
#include <string.h>

// External
#include <sodium.h>

// Project
#include "../../../include/libmrhsrv/libmrhsrv/Communication/MRH_ServerCommunication.h"
#include "../Error/MRH_ServerErrorInternal.h"
#include "../MRH_ServerTypesInternal.h"
#include "./NetMessage/MRH_NetMessageV1.h"
#include "./MsQuic/MRH_MsQuic.h"

// Pre-defined
/*
#if crypto_box_SEEDBYTES != crypto_box_KEYBYTES // Warn, code relies on this
    #error "Seed bytes not equal key bytes, encryption / decryption will fail!"
#endif
*/


//*************************************************************************************
// Connection
//*************************************************************************************

int MRH_SRV_Connect(MRH_Srv_Context* p_Context, MRH_Srv_Server* p_Server, const char* p_Address, int i_Port, int i_WaitS)
{
    if (p_Server == NULL || p_Address == NULL || i_Port <= 0)
    {
        MRH_ERR_SetServerError(MRH_SERVER_ERROR_GENERAL_INVALID_PARAM);
        return -1;
    }
    
    // Set address and port
    strncpy(p_Server->p_Address, p_Address, MRH_SRV_SIZE_SERVER_ADDRESS);
    p_Server->i_Port = i_Port;
    
    // @NOTE: Callback handles seting the connection inside the context!
    MRH_MsQuicConnection* p_MsQuic = p_Server->p_MsQuic;
    HQUIC p_NewConnection;
    
    if (QUIC_FAILED(p_MsQuic->p_MsQuicAPI->ConnectionOpen(p_Context->p_MsQuicRegistration,
                                                          (void*)MRH_MsQuicConnectionCallback,
                                                          p_MsQuic,
                                                          &p_NewConnection)))
    {
        MRH_ERR_SetServerError(MRH_SERVER_ERROR_AUTH_CONNECTION_CREATE);
        return -1;
    }
    else if (QUIC_FAILED(p_MsQuic->p_MsQuicAPI->ConnectionStart(p_NewConnection,
                                                                p_Context->p_MsQuicConfiguration,
                                                                QUIC_ADDRESS_FAMILY_UNSPEC,
                                                                p_Server->p_Address,
                                                                p_Server->i_Port)))
    {
        p_Context->p_MsQuicAPI->ConnectionClose(p_NewConnection);
        MRH_ERR_SetServerError(MRH_SERVER_ERROR_AUTH_CONNECTION_START);
        return -1;
    }
    
    // Should we wait here for connection success?
    if (i_WaitS < 0)
    {
        return 0;
    }
    
    // Wait for the connection to be set
    time_t u32_EndTimeS = time(NULL) + (i_WaitS);
    
    while (time(NULL) < u32_EndTimeS)
    {
        if (p_MsQuic->p_Connection != NULL)
        {
            return 0;
        }
        
        sleep(1);
    }
    
    return -1;
}

int MRH_SRV_CreatePasswordHash(uint8_t* p_Buffer, const char* p_Password, const char* p_Salt, uint8_t u8_HashType)
{
    if (p_Buffer == NULL || p_Password == NULL || p_Salt == NULL)
    {
        MRH_ERR_SetServerError(MRH_SERVER_ERROR_GENERAL_INVALID_PARAM);
        return -1;
    }
    
    // Copy salt to match expected size
    unsigned char p_FullSalt[crypto_pwhash_SALTBYTES] = { '\0' };
    memcpy(p_FullSalt, p_Salt, crypto_pwhash_SALTBYTES);
    
    memset(p_Buffer, '\0', MRH_SRV_SIZE_ACCOUNT_PASSWORD); /* @NOTE: = SEEDBYTES (32) which is KEYBYTES (32) */
    
    unsigned long long ull_OpsLimit;
    size_t us_MemLimit;
    int i_Alg;
    
    switch (u8_HashType)
    {
        case 0:
            ull_OpsLimit = crypto_pwhash_OPSLIMIT_INTERACTIVE;
            us_MemLimit = 128 * 1024 * 1024;//crypto_pwhash_argon2id_MEMLIMIT_SENSITIVE;
            i_Alg = crypto_pwhash_ALG_ARGON2ID13;
            break;
            
        default:
            MRH_ERR_SetServerError(MRH_SERVER_ERROR_GENERAL_INVALID_PARAM);
            return -1;
    }
    
    if (crypto_pwhash(p_Buffer,
                      crypto_box_SEEDBYTES,
                      p_Password,
                      crypto_secretbox_KEYBYTES,
                      p_FullSalt,
                      ull_OpsLimit,
                      us_MemLimit,
                      i_Alg) != 0)
    {
        MRH_ERR_SetServerError(MRH_SERVER_ERROR_ENCRYPTION_PW_HASH_MEM);
        return -1;
    }
    
    return 0;
}

int MRH_SRV_EncryptNonce(uint8_t* p_Buffer, uint32_t u32_Nonce, const uint8_t* p_Password)
{
    if (p_Buffer == NULL || p_Password == NULL)
    {
        MRH_ERR_SetServerError(MRH_SERVER_ERROR_GENERAL_INVALID_PARAM);
        return -1;
    }
    
    unsigned char p_Nonce[crypto_secretbox_NONCEBYTES] = { '\0' };
    randombytes_buf(p_Nonce, crypto_secretbox_NONCEBYTES);
    memcpy(p_Buffer, p_Nonce, crypto_secretbox_NONCEBYTES);
    
    if (crypto_secretbox_easy(&(p_Buffer[crypto_secretbox_NONCEBYTES]),
                              (const unsigned char*)&u32_Nonce,
                              sizeof(u32_Nonce),
                              p_Nonce,
                              p_Password) != 0) /* @NOTE: KEYBYTES == SEEDBYTES (32) */
    {
        MRH_ERR_SetServerError(MRH_SERVER_ERROR_ENCRYPTION_FAILED);
        return -1;
    }
    
    return 0;
}

int MRH_SRV_DecryptNonce(uint32_t* p_Nonce, const uint8_t* p_EncryptedNonce, const uint8_t* p_Password)
{
    if (p_Nonce == NULL || p_EncryptedNonce == NULL || p_Password == NULL)
    {
        MRH_ERR_SetServerError(MRH_SERVER_ERROR_GENERAL_INVALID_PARAM);
        return -1;
    }
    
    // Grab nonce from nonce encryption first first
    unsigned char p_FullNonce[crypto_secretbox_NONCEBYTES] = { '\0' };
    memcpy(p_FullNonce, p_EncryptedNonce, crypto_secretbox_NONCEBYTES);
    
    if (crypto_secretbox_open_easy((unsigned char*)p_Nonce,
                                   (const unsigned char*)&(p_EncryptedNonce[crypto_secretbox_NONCEBYTES]),
                                   crypto_secretbox_MACBYTES + sizeof(uint32_t),
                                   (const unsigned char*)p_FullNonce,
                                   (const unsigned char*)p_Password) != 0)
    {
        MRH_ERR_SetServerError(MRH_SERVER_ERROR_ENCRYPTION_FAILED);
        return -1;
    }
    
    return 0;
}

void MRH_SRV_Disconnect(MRH_Srv_Server* p_Server, int i_WaitS)
{
    if (p_Server == NULL || p_Server->p_MsQuic->p_Connection == NULL)
    {
        return;
    }
    
    // Perform connection shutdown
    p_Server->p_MsQuic->p_MsQuicAPI->ConnectionShutdown(p_Server->p_MsQuic->p_Connection,
                                                        QUIC_CONNECTION_SHUTDOWN_FLAG_NONE,
                                                        0);
    
    // Should we wait here for a disconnect?
    if (i_WaitS < 0)
    {
        return;
    }
    
    // Wait for the connection to be disconnected
    time_t u32_EndTimeS = time(NULL) + (i_WaitS);
    
    while (time(NULL) < u32_EndTimeS)
    {
        if (p_Server->p_MsQuic->p_Connection == NULL)
        {
            return;
        }
        
        sleep(1);
    }
}

int MRH_SRV_IsConnected(MRH_Srv_Server* p_Server)
{
    if (p_Server == NULL)
    {
        return -1;
    }
    
    return p_Server->p_MsQuic->p_Connection != NULL ? 0 : -1;
}

//*************************************************************************************
// Encryption
//*************************************************************************************

static inline size_t MRH_SRV_GetEncryptedSize(size_t us_MessageSize)
{
    // Messages are [Nonce][Encrypted Buffer (MAC + Message Bytes)]
    return crypto_secretbox_NONCEBYTES +
           crypto_secretbox_MACBYTES +
           us_MessageSize;
}

static int MRH_SRV_Encrypt(uint8_t* p_EncryptedBuffer, const uint8_t* p_MessageBuffer, size_t us_MessageSize, const char* p_Password)
{
    if (p_Password == NULL)
    {
        return -1;
    }
    
    unsigned char p_Key[crypto_secretbox_KEYBYTES] = { '\0' };
    memcpy(p_Key, p_Password, crypto_secretbox_KEYBYTES);
    
    unsigned char p_Nonce[crypto_secretbox_NONCEBYTES] = { '\0' };
    randombytes_buf(p_Nonce, crypto_secretbox_NONCEBYTES);
    memcpy(p_EncryptedBuffer, p_Nonce, crypto_secretbox_NONCEBYTES);
    
    if (crypto_secretbox_easy(&(p_EncryptedBuffer[crypto_secretbox_NONCEBYTES]),
                              p_MessageBuffer,
                              us_MessageSize,
                              p_Nonce,
                              p_Key) != 0)
    {
        return -1;
    }
    
    return 0;
}

static int MRH_SRV_Decrypt(uint8_t* p_MessageBuffer, const uint8_t* p_EncryptedBuffer, size_t us_EncryptedSize, const char* p_Password)
{
    if (p_Password == NULL)
    {
        return -1;
    }
    
    unsigned char p_Key[crypto_secretbox_KEYBYTES] = { '\0' };
    memcpy(p_Key, p_Password, crypto_secretbox_KEYBYTES);
    
    unsigned char p_Nonce[crypto_secretbox_NONCEBYTES] = { '\0' };
    memcpy(p_Nonce, p_EncryptedBuffer, crypto_secretbox_NONCEBYTES);
    
    if (crypto_secretbox_open_easy(p_MessageBuffer,
                                   &(p_EncryptedBuffer[crypto_secretbox_NONCEBYTES]),
                                   crypto_secretbox_MACBYTES + us_EncryptedSize,
                                   p_Nonce,
                                   p_Key) != 0)
    {
        return -1;
    }
    
    return 0;
}

//*************************************************************************************
// Recieve
//*************************************************************************************

MRH_Srv_NetMessage MRH_SRV_RecieveMessage(MRH_Srv_Server* p_Server, uint8_t* p_Buffer, const char* p_Password)
{
    if (p_Server == NULL || p_Buffer == NULL)
    {
        MRH_ERR_SetServerError(MRH_SERVER_ERROR_GENERAL_INVALID_PARAM);
        return MRH_SRV_MSG_UNK;
    }
    
    MRH_MsQuicConnection* p_MsQuic = p_Server->p_MsQuic;
    
    for (size_t i = 0; i < MRH_SRV_MESSAGE_BUFFER_COUNT; ++i)
    {
        if (p_MsQuic->p_Recieved[i].i_State != MRH_MSQ_MESSAGE_COMPLETE)
        {
            continue;
        }
        
        // Needs to be decrypted?
        switch (p_MsQuic->p_Recieved[i].p_Buffer[0])
        {
            case MRH_SRV_MSG_TEXT:
            case MRH_SRV_MSG_LOCATION:
            case MRH_SRV_MSG_CUSTOM:
                // @NOTE: Exclude message id from decryption!
                if (MRH_SRV_Decrypt(&(p_Buffer[1]),
                                    &(p_MsQuic->p_Recieved[i].p_Buffer[1]),
                                    p_MsQuic->p_Recieved[i].us_SizeCur - 1,
                                    p_Password) < 0)
                {
                    MRH_ERR_SetServerError(MRH_SERVER_ERROR_ENCRYPTION_FAILED);
                    p_Buffer[0] = MRH_SRV_MSG_UNK;
                }
                else
                {
                    p_Buffer[0] = p_MsQuic->p_Recieved[i].p_Buffer[0];
                }
                break;
                
            default:
                // No encprytion, simply copy
                memcpy(p_Buffer,
                       &(p_MsQuic->p_Recieved[i].p_Buffer[0]),
                       p_MsQuic->p_Recieved[i].us_SizeCur);
                break;
                
        }
        
        // Set as read
        p_MsQuic->p_Recieved[i].i_State = MRH_MSQ_MESSAGE_FREE;
        
        // Return net message id
        return (MRH_Srv_NetMessage)(p_Buffer[0]);
    }
    
    // Nothing
    return MRH_SRV_MSG_UNK;
}

int MRH_SRV_SetNetMessage(void* p_Message, const uint8_t* p_Buffer)
{
    if (p_Message == NULL || p_Buffer == NULL)
    {
        MRH_ERR_SetServerError(MRH_SERVER_ERROR_GENERAL_INVALID_PARAM);
        return -1;
    }
    
    switch (p_Buffer[0]) // 0 = Net Message ID
    {
        /**
         *  Net Message Version 1
         */
        
        // Server Auth
        case MRH_SRV_MSG_AUTH_CHALLENGE:
            TO_MRH_SRV_MSG_AUTH_CHALLENGE((MRH_SRV_MSG_AUTH_CHALLENGE_DATA*)p_Message,
                                          &(p_Buffer[1]));
            break;
        case MRH_SRV_MSG_AUTH_STATE:
            TO_MRH_SRV_MSG_AUTH_STATE((MRH_SRV_MSG_AUTH_STATE_DATA*)p_Message,
                                      &(p_Buffer[1]));
            break;
            
        // Communication
        case MRH_SRV_MSG_NO_DATA:
            TO_MRH_SRV_MSG_NO_DATA((MRH_SRV_MSG_NO_DATA_DATA*)p_Message,
                                   &(p_Buffer[1]));
            break;
        case MRH_SRV_MSG_TEXT:
            TO_MRH_SRV_MSG_TEXT((MRH_SRV_MSG_TEXT_DATA*)p_Message,
                                &(p_Buffer[1]));
            break;
        case MRH_SRV_MSG_LOCATION:
            TO_MRH_SRV_MSG_LOCATION((MRH_SRV_MSG_LOCATION_DATA*)p_Message,
                                    &(p_Buffer[1]));
            break;
        case MRH_SRV_MSG_CUSTOM:
            TO_MRH_SRV_MSG_CUSTOM((MRH_SRV_MSG_CUSTOM_DATA*)p_Message,
                                  &(p_Buffer[1]));
            break;
            
        /**
         *  Default (No Data, Unknown, Not Readable, etc.)
         */
            
        default:
            MRH_ERR_SetServerError(MRH_SERVER_ERROR_GENERAL_INVALID_PARAM);
            return -1;
    }
    
    return 0;
}

//*************************************************************************************
// Send
//*************************************************************************************

int MRH_SRV_SendMessage(MRH_Srv_Server* p_Server, MRH_Srv_NetMessage e_Message, const void* p_Data, const char* p_Password)
{
    if (p_Server == NULL)
    {
        MRH_ERR_SetServerError(MRH_SERVER_ERROR_GENERAL_INVALID_PARAM);
        return -1;
    }
    
    // Find the server for the channel
    MRH_MsQuicConnection* p_MsQuic = p_Server->p_MsQuic;
    
    if (p_MsQuic->p_Connection == NULL)
    {
        MRH_ERR_SetServerError(MRH_SERVER_ERROR_SEND_DISCONNECTED);
        return -1;
    }
    
    // Now create send buffer
    MRH_MsQuicMessage* p_Message = NULL;
    
    for (size_t i = 0; i < MRH_SRV_MESSAGE_BUFFER_COUNT; ++i)
    {
        if (p_MsQuic->p_Send[i].i_State == MRH_MSQ_MESSAGE_FREE ||
            p_MsQuic->p_Send[i].i_State == MRH_MSQ_MESSAGE_COMPLETE)
        {
            p_Message = &(p_MsQuic->p_Send[i]);
            p_Message->i_State = MRH_MSQ_MESSAGE_IN_USE;
            break;
        }
    }
    
    if (p_Message == NULL)
    {
        MRH_ERR_SetServerError(MRH_SERVER_ERROR_SEND_QUEUE_FULL);
        return -1;
    }
    
    // Now, create or expand the buffer as needed
    uint8_t p_MessageBuffer[MRH_SRV_SIZE_MESSAGE_BUFFER_MAX];
    size_t us_MessageSize = 1; // The real message size, start with message id
    
    int i_Encrypt; // Define if message uses end to end encryption
    
    // Set message id
    p_MessageBuffer[0] = (uint8_t)e_Message;
    
    // Grab data from the message to send
    switch (e_Message)
    {
        /**
         *  Net Message Version 1
         */
        
        // Server Auth
        case MRH_SRV_MSG_AUTH_REQUEST:
            us_MessageSize += FROM_MRH_SRV_MSG_AUTH_REQUEST(&(p_MessageBuffer[1]),
                                                            (const MRH_SRV_MSG_AUTH_REQUEST_DATA*)p_Data);
            i_Encrypt = -1;
            break;
        case MRH_SRV_MSG_AUTH_PROOF:
            us_MessageSize += FROM_MRH_SRV_MSG_AUTH_PROOF(&(p_MessageBuffer[1]),
                                                          (const MRH_SRV_MSG_AUTH_PROOF_DATA*)p_Data);
            i_Encrypt = -1;
            break;
            
        // Communication
        case MRH_SRV_MSG_DATA_AVAIL:
            us_MessageSize += FROM_MRH_SRV_MSG_DATA_AVAIL(&(p_MessageBuffer[1]),
                                                          (const MRH_SRV_MSG_DATA_AVAIL_DATA*)p_Data);
            i_Encrypt = -1;
            break;
        case MRH_SRV_MSG_TEXT:
            us_MessageSize += FROM_MRH_SRV_MSG_TEXT(&(p_MessageBuffer[1]),
                                                    (const MRH_SRV_MSG_TEXT_DATA*)p_Data);
            i_Encrypt = 0;
            break;
        case MRH_SRV_MSG_LOCATION:
            us_MessageSize += FROM_MRH_SRV_MSG_LOCATION(&(p_MessageBuffer[1]),
                                                        (const MRH_SRV_MSG_LOCATION_DATA*)p_Data);
            i_Encrypt = 0;
            break;
        case MRH_SRV_MSG_CUSTOM:
            us_MessageSize += FROM_MRH_SRV_MSG_CUSTOM(&(p_MessageBuffer[1]),
                                                      (const MRH_SRV_MSG_CUSTOM_DATA*)p_Data);
            i_Encrypt = 0;
            break;
            
        /**
         *  Default (Unknown, Not Readable, etc.)
         */
            
        default:
            MRH_ERR_SetServerError(MRH_SERVER_ERROR_SEND_INVALID_MESSAGE);
            p_Message->i_State = MRH_MSQ_MESSAGE_FREE;
            return -1;
    }
    
    // Do we have a password for encryption?
    if (i_Encrypt == 0 && p_Password == NULL)
    {
        MRH_ERR_SetServerError(MRH_SERVER_ERROR_GENERAL_INVALID_PARAM);
        return -1;
    }
    
    // Now, create or expand the buffer as needed
    size_t us_BufferSize = sizeof(QUIC_BUFFER);
    
    if (i_Encrypt == 0)
    {
        us_BufferSize += MRH_SRV_GetEncryptedSize(us_MessageSize);
    }
    else
    {
        us_BufferSize += us_MessageSize;
    }
    
    if (p_Message->p_Buffer == NULL || p_Message->us_SizeMax < us_BufferSize)
    {
        if ((p_Message->p_Buffer = (uint8_t*)realloc(p_Message->p_Buffer, us_BufferSize)) == NULL)
        {
            MRH_ERR_SetServerError(MRH_SERVER_ERROR_GENERAL_MALLOC);
            p_Message->i_State = MRH_MSQ_MESSAGE_FREE;
            return -1;
        }
        
        p_Message->us_SizeMax = us_BufferSize;
    }
    
    // And now write the message content
    if (i_Encrypt == 0)
    {
        // Set net message
        p_Message->p_Buffer[sizeof(QUIC_BUFFER)] = p_MessageBuffer[0];
        
        // Encrypt message data
        if (MRH_SRV_Encrypt(&(p_Message->p_Buffer[sizeof(QUIC_BUFFER) + 1]),
                            &(p_MessageBuffer[1]),
                            us_MessageSize - 1,
                            p_Password) < 0)
        {
            MRH_ERR_SetServerError(MRH_SERVER_ERROR_ENCRYPTION_FAILED);
            p_Message->i_State = MRH_MSQ_MESSAGE_FREE;
            return -1;
        }
    }
    else
    {
        memcpy(&(p_Message->p_Buffer[sizeof(QUIC_BUFFER)]),
               p_MessageBuffer,
               us_MessageSize);
    }
    
    p_Message->us_SizeCur = us_BufferSize;
    
    // Setup buffer for quic usage
    QUIC_BUFFER* p_QuicBuffer = (QUIC_BUFFER*)(p_Message->p_Buffer);
    p_QuicBuffer->Buffer = &(p_Message->p_Buffer[sizeof(QUIC_BUFFER)]);
    p_QuicBuffer->Length = us_BufferSize - sizeof(QUIC_BUFFER); // Wanted is the payload size
    
    // Create a stream to send the message on
    HQUIC p_Stream;
    
    if (QUIC_FAILED(p_MsQuic->p_MsQuicAPI->StreamOpen(p_MsQuic->p_Connection,
                                                      QUIC_STREAM_OPEN_FLAG_UNIDIRECTIONAL, /* QUIC_STREAM_OPEN_FLAG_NONE, */
                                                      MRH_MsQuicStreamCallback,
                                                      p_Message, /* Pass message as context */
                                                      &p_Stream)))
    {
        MRH_ERR_SetServerError(MRH_SERVER_ERROR_SEND_STREAM_CREATE);
        p_Message->i_State = MRH_MSQ_MESSAGE_FREE;
        return -1;
    }
    else if (QUIC_FAILED(p_MsQuic->p_MsQuicAPI->StreamStart(p_Stream,
                                                            QUIC_STREAM_START_FLAG_SHUTDOWN_ON_FAIL)))
    {
        MRH_ERR_SetServerError(MRH_SERVER_ERROR_SEND_STREAM_START);
        p_MsQuic->p_MsQuicAPI->StreamClose(p_Stream);
        p_Message->i_State = MRH_MSQ_MESSAGE_FREE;
        return -1;
    }
    else if (QUIC_FAILED(p_MsQuic->p_MsQuicAPI->StreamSend(p_Stream,
                                                           p_QuicBuffer,
                                                           1,
                                                           QUIC_SEND_FLAG_FIN,
                                                           p_Message))) /* send context */
    {
        MRH_ERR_SetServerError(MRH_SERVER_ERROR_SEND_STREAM_SEND);
        p_MsQuic->p_MsQuicAPI->StreamClose(p_Stream);
        p_Message->i_State = MRH_MSQ_MESSAGE_FREE;
        return -1;
    }
    
    return 0;
}

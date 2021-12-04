/**
 *  libmrhsrv
 *  Copyright (C) 2021 Jens Brörken
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


//*************************************************************************************
// Connection
//*************************************************************************************

int MRH_SRV_Connect(MRH_Srv_Context* p_Context, MRH_Srv_Server* p_Server, const char* p_Address, int i_Port)
{
    if (p_Server == NULL ||
        p_Address == NULL || strlen(p_Address) == 0 || strlen(p_Address) > MRH_SRV_SIZE_SERVER_ADDRESS ||
        i_Port <= 0)
    {
        MRH_ERR_SetServerError(MRH_SERVER_ERROR_GENERAL_INVALID_PARAM);
        return -1;
    }
    
    // Set address and port
    strncpy(p_Server->p_Address, p_Address, strlen(p_Address));
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
    
    // Wait for the connection to be set
    time_t u32_EndTime = time(NULL) + (p_Server->i_TimeoutMS / 1000);
    
    while (time(NULL) < u32_EndTime)
    {
        if (p_MsQuic->p_Connection != NULL)
        {
            return 0;
        }
        
        sleep(1);
    }
    
    return -1;
}

int MRH_SRV_CreateAccNonceHash(uint8_t* p_Buffer, uint32_t u32_Nonce, uint8_t u8_HashType, const char* p_Password, const char* p_Salt)
{
    if (p_Buffer == NULL ||
        p_Password == NULL || strlen(p_Password) > MRH_SRV_SIZE_ACCOUNT_PASSWORD ||
        p_Salt == NULL || strlen(p_Salt) > MRH_SRV_SIZE_ACCOUNT_PASSWORD_SALT)
    {
        MRH_ERR_SetServerError(MRH_SERVER_ERROR_GENERAL_INVALID_PARAM);
        return -1;
    }
    
    // First, create the hashed password
    unsigned char p_FullSalt[crypto_pwhash_SALTBYTES] = { '\0' };
    memcpy(p_FullSalt, p_Salt, strlen(p_Salt));
    
    unsigned char p_Key[crypto_box_SEEDBYTES] = { '\0' };
    
    unsigned long long ull_OpsLimit;
    size_t us_MemLimit;
    int i_Alg;
    
    switch (u8_HashType)
    {
        case 0:
            ull_OpsLimit = crypto_pwhash_OPSLIMIT_INTERACTIVE;
            us_MemLimit = crypto_pwhash_argon2id_MEMLIMIT_SENSITIVE;
            i_Alg = crypto_pwhash_ALG_ARGON2ID13;
            break;
            
        default:
            MRH_ERR_SetServerError(MRH_SERVER_ERROR_GENERAL_INVALID_PARAM);
            return -1;
    }
    
    if (crypto_pwhash(p_Key,
                      crypto_box_SEEDBYTES,
                      p_Password,
                      strlen(p_Password),
                      p_FullSalt,
                      ull_OpsLimit,
                      us_MemLimit,
                      i_Alg) != 0)
    {
        MRH_ERR_SetServerError(MRH_SERVER_ERROR_ENCRYPTION_PW_HASH_MEM);
        return -1;
    }
    
    // Now with the key we encrypt the nonce
    unsigned char p_Nonce[crypto_secretbox_NONCEBYTES] = { '\0' };
    randombytes_buf(p_Nonce, crypto_secretbox_NONCEBYTES);
    memcpy(p_Buffer, p_Nonce, crypto_secretbox_NONCEBYTES);
    
    if (crypto_secretbox_easy(&(p_Buffer[crypto_secretbox_NONCEBYTES]),
                              (const unsigned char*)&u32_Nonce,
                              sizeof(u32_Nonce),
                              p_Nonce,
                              p_Key) != 0)
    {
        MRH_ERR_SetServerError(MRH_SERVER_ERROR_ENCRYPTION_FAILED);
        return -1;
    }
    
    return 0;
}

void MRH_SRV_Disconnect(MRH_Srv_Server* p_Server)
{
    if (p_Server == NULL)
    {
        return;
    }
    
    if (p_Server->p_MsQuic->p_Connection != NULL)
    {
        p_Server->p_MsQuic->p_MsQuicAPI->ConnectionShutdown(p_Server->p_MsQuic->p_Connection,
                                                            QUIC_CONNECTION_SHUTDOWN_FLAG_NONE,
                                                            0);
        while (p_Server->p_MsQuic->p_Connection != NULL);
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

static inline size_t MRH_SRV_GetEncryptedSize()
{
    // Messages are [Nonce][Encrypted Buffer (MAC + Message Bytes)]
    return crypto_secretbox_NONCEBYTES +
           crypto_secretbox_MACBYTES +
           MRH_SRV_SIZE_MESSAGE_BUFFER;
}

static int MRH_SRV_Encrypt(uint8_t* p_EncryptedBuffer, const uint8_t* p_MessageBuffer, const char* p_Password)
{
    if (strlen(p_Password) > crypto_secretbox_KEYBYTES)
    {
        return -1;
    }
    
    unsigned char p_Key[crypto_secretbox_KEYBYTES] = { '\0' };
    memcpy(p_Key, p_Password, strlen(p_Password));
    
    unsigned char p_Nonce[crypto_secretbox_NONCEBYTES] = { '\0' };
    randombytes_buf(p_Nonce, crypto_secretbox_NONCEBYTES);
    memcpy(p_EncryptedBuffer, p_Nonce, crypto_secretbox_NONCEBYTES);
    
    if (crypto_secretbox_easy(&(p_EncryptedBuffer[crypto_secretbox_NONCEBYTES]),
                              p_MessageBuffer,
                              MRH_SRV_SIZE_MESSAGE_BUFFER,
                              p_Nonce,
                              p_Key) != 0)
    {
        return -1;
    }
    
    return 0;
}

static int MRH_SRV_Decrypt(uint8_t* p_MessageBuffer, const uint8_t* p_EncryptedBuffer, const char* p_Password)
{
    if (strlen(p_Password) > crypto_secretbox_KEYBYTES)
    {
        return -1;
    }
    
    unsigned char p_Key[crypto_secretbox_KEYBYTES] = { '\0' };
    memcpy(p_Key, p_Password, strlen(p_Password));
    
    unsigned char p_Nonce[crypto_secretbox_NONCEBYTES] = { '\0' };
    memcpy(p_Nonce, p_EncryptedBuffer, crypto_secretbox_NONCEBYTES);
    
    if (crypto_secretbox_open_easy(p_MessageBuffer,
                                   &(p_EncryptedBuffer[crypto_secretbox_NONCEBYTES]),
                                   crypto_secretbox_MACBYTES + MRH_SRV_SIZE_MESSAGE_BUFFER,
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

size_t MRH_SRV_RecieveBufferSize(void)
{
    return MRH_SRV_GetEncryptedSize();
}

MRH_Srv_NetMessage MRH_SRV_RecieveMessage(MRH_Srv_Server* p_Server, uint8_t* p_Buffer)
{
    if (p_Server == NULL || p_Buffer == NULL)
    {
        MRH_ERR_SetServerError(MRH_SERVER_ERROR_GENERAL_INVALID_PARAM);
        return MRH_SRV_CS_MSG_UNK;
    }
    
    MRH_MsQuicConnection* p_MsQuic = p_Server->p_MsQuic;
    
    for (size_t i = 0; i < MRH_SRV_MESSAGE_BUFFER_COUNT; ++i)
    {
        if (p_MsQuic->p_Recieved[i].i_State != MRH_MSQ_MESSAGE_COMPLETE)
        {
            continue;
        }
        
        memcpy(p_Buffer,
               &(p_MsQuic->p_Recieved[i].p_Buffer[0]),
               p_MsQuic->p_Recieved[i].us_SizeCur);
        
        // Set as read
        p_MsQuic->p_Recieved[i].i_State = MRH_MSQ_MESSAGE_FREE;
        
        // Return net message id
        return (MRH_Srv_NetMessage)(p_Buffer[0]);
    }
    
    // Nothing
    return MRH_SRV_CS_MSG_UNK;
}

int MRH_SRV_SetNetMessage(void* p_Message, const uint8_t* p_Buffer, const char* p_Password)
{
    if (p_Message == NULL || p_Buffer == NULL)
    {
        MRH_ERR_SetServerError(MRH_SERVER_ERROR_GENERAL_INVALID_PARAM);
        return -1;
    }
    
    uint8_t p_MessageBuffer[MRH_SRV_SIZE_MESSAGE_BUFFER] = { '\0' };
    
    if (p_Password != NULL)
    {
        if (MRH_SRV_Decrypt(p_MessageBuffer, p_Buffer, p_Password) < 0)
        {
            MRH_ERR_SetServerError(MRH_SERVER_ERROR_ENCRYPTION_FAILED);
            return -1;
        }
    }
    else
    {
        // No encprytion, copy
        // @TODO: Do this better, this copy is useless
        memcpy(p_MessageBuffer,
               p_Buffer,
               MRH_SRV_SIZE_MESSAGE_BUFFER);
    }
    
    MRH_Srv_NetMessage e_Message = p_MessageBuffer[0];
    
    switch (e_Message)
    {
        /**
         *  Net Message Version 1
         */
        
        // Server Auth
        case MRH_SRV_S_MSG_AUTH_CHALLENGE:
            TO_MRH_SRV_S_MSG_AUTH_CHALLENGE((MRH_SRV_S_MSG_AUTH_CHALLENGE_DATA*)p_Message, p_MessageBuffer);
            break;
        case MRH_SRV_S_MSG_AUTH_RESULT:
            TO_MRH_SRV_S_MSG_AUTH_RESULT((MRH_SRV_S_MSG_AUTH_RESULT_DATA*)p_Message, p_MessageBuffer);
            break;
            
        // Device Auth
        case MRH_SRV_C_MSG_PAIR_CHALLENGE:
            TO_MRH_SRV_C_MSG_PAIR_CHALLENGE((MRH_SRV_C_MSG_PAIR_CHALLENGE_DATA*)p_Message, p_MessageBuffer);
            break;
        case MRH_SRV_C_MSG_PAIR_PROOF:
            TO_MRH_SRV_C_MSG_PAIR_PROOF((MRH_SRV_C_MSG_PAIR_PROOF_DATA*)p_Message, p_MessageBuffer);
            break;
        case MRH_SRV_C_MSG_PAIR_RESULT:
            TO_MRH_SRV_C_MSG_PAIR_RESULT((MRH_SRV_C_MSG_PAIR_RESULT_DATA*)p_Message, p_MessageBuffer);
            break;
            
        // Channel
        case MRH_SRV_S_MSG_CHANNEL_RESPONSE:
            TO_MRH_SRV_S_MSG_CHANNEL_RESPONSE((MRH_SRV_S_MSG_CHANNEL_RESPONSE_DATA*)p_Message, p_MessageBuffer);
            break;
            
        // Text
        case MRH_SRV_C_MSG_TEXT:
            TO_MRH_SRV_C_MSG_TEXT((MRH_SRV_C_MSG_TEXT_DATA*)p_Message, p_MessageBuffer);
            break;
            
        // Location
        case MRH_SRV_C_MSG_LOCATION:
            TO_MRH_SRV_C_MSG_LOCATION((MRH_SRV_C_MSG_LOCATION_DATA*)p_Message, p_MessageBuffer);
            break;
            
        // Custom
        case MRH_SRV_C_MSG_CUSTOM:
            TO_MRH_SRV_C_MSG_CUSTOM((MRH_SRV_C_MSG_CUSTOM_DATA*)p_Message, p_MessageBuffer);
            break;
        case MRH_SRV_S_MSG_CUSTOM:
            TO_MRH_SRV_S_MSG_CUSTOM((MRH_SRV_S_MSG_CUSTOM_DATA*)p_Message, p_MessageBuffer);
            break;
            
        /**
         *  Default (No Data, Unknown, Not Readable, etc.)
         */
            
        default:
            break;
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
    uint8_t p_MessageBuffer[MRH_SRV_SIZE_MESSAGE_BUFFER];
    
    // Grab data from the message to send
    switch (e_Message)
    {
        /**
         *  Net Message Version 1
         */
        
        // Availability
        case MRH_SRV_C_MSG_HELLO:
            memset(p_MessageBuffer, '\0', MRH_SRV_SIZE_MESSAGE_BUFFER);
            p_MessageBuffer[0] = MRH_SRV_C_MSG_HELLO;
            break;
            
        // Server Auth
        case MRH_SRV_C_MSG_AUTH_REQUEST:
            FROM_MRH_SRV_C_MSG_AUTH_REQUEST(p_MessageBuffer, (const MRH_SRV_C_MSG_AUTH_REQUEST_DATA*)p_Data);
            break;
        case MRH_SRV_C_MSG_AUTH_PROOF:
            FROM_MRH_SRV_C_MSG_AUTH_PROOF(p_MessageBuffer, (const MRH_SRV_C_MSG_AUTH_PROOF_DATA*)p_Data);
            break;
            
        // Device Auth
        case MRH_SRV_C_MSG_PAIR_CHALLENGE:
            FROM_MRH_SRV_C_MSG_PAIR_CHALLENGE(p_MessageBuffer, (const MRH_SRV_C_MSG_PAIR_CHALLENGE_DATA*)p_Data);
            break;
        case MRH_SRV_C_MSG_PAIR_PROOF:
            FROM_MRH_SRV_C_MSG_PAIR_PROOF(p_MessageBuffer, (const MRH_SRV_C_MSG_PAIR_PROOF_DATA*)p_Data);
            break;
        case MRH_SRV_C_MSG_PAIR_RESULT:
            FROM_MRH_SRV_C_MSG_PAIR_RESULT(p_MessageBuffer, (const MRH_SRV_C_MSG_PAIR_RESULT_DATA*)p_Data);
            break;
            
        // Channel
        case MRH_SRV_C_MSG_CHANNEL_REQUEST:
            FROM_MRH_SRV_C_MSG_CHANNEL_REQUEST(p_MessageBuffer, (const MRH_SRV_C_MSG_CHANNEL_REQUEST_DATA*)p_Data);
            break;
            
        // Text
        case MRH_SRV_C_MSG_TEXT:
            FROM_MRH_SRV_C_MSG_TEXT(p_MessageBuffer, (const MRH_SRV_C_MSG_TEXT_DATA*)p_Data);
            break;
            
        // Location
        case MRH_SRV_C_MSG_LOCATION:
            FROM_MRH_SRV_C_MSG_LOCATION(p_MessageBuffer, (const MRH_SRV_C_MSG_LOCATION_DATA*)p_Data);
            break;
            
        // Custom
        case MRH_SRV_C_MSG_CUSTOM:
            FROM_MRH_SRV_C_MSG_CUSTOM(p_MessageBuffer, (const MRH_SRV_C_MSG_CUSTOM_DATA*)p_Data);
            break;
            
        /**
         *  Default (Unknown, Not Readable, etc.)
         */
            
        default:
            MRH_ERR_SetServerError(MRH_SERVER_ERROR_SEND_INVALID_MESSAGE);
            p_Message->i_State = MRH_MSQ_MESSAGE_FREE;
            return -1;
    }
    
    // Now, create or expand the buffer as needed
    size_t us_BufferSize = sizeof(QUIC_BUFFER);
    
    if (p_Password != NULL)
    {
        us_BufferSize += MRH_SRV_GetEncryptedSize();
    }
    else
    {
        us_BufferSize += MRH_SRV_SIZE_MESSAGE_BUFFER;
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
    
    // And now write the message contents
    if (p_Password != NULL)
    {
        if (MRH_SRV_Encrypt(&(p_Message->p_Buffer[sizeof(QUIC_BUFFER)]), p_MessageBuffer, p_Password) < 0)
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
               MRH_SRV_SIZE_MESSAGE_BUFFER);
    }
    
    p_Message->us_SizeCur = us_BufferSize;
    
    // Setup buffer for quic usage
    QUIC_BUFFER* p_QuicBuffer = (QUIC_BUFFER*)(p_Message->p_Buffer);
    p_QuicBuffer->Buffer = &(p_Message->p_Buffer[sizeof(QUIC_BUFFER)]);
    p_QuicBuffer->Length = MRH_SRV_SIZE_MESSAGE_BUFFER;
    
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

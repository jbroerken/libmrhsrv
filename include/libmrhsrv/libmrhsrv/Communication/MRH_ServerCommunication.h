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

#ifndef MRH_ServerCommunication_h
#define MRH_ServerCommunication_h

// C

// External

// Project
#include "./MRH_NetMessage.h"
#include "../MRH_ServerTypes.h"


#ifdef __cplusplus
extern "C"
{
#endif
    
    //*************************************************************************************
    // Connection
    //*************************************************************************************
    
    /**
     *  Connect to a server by channel name.
     *
     *  \param p_Context The library context to use for connecting.
     *  \param p_Server The server to connect to.
     *  \param p_Address The server address. The buffer has to be of size
     *                   MRH_SRV_SIZE_SERVER_ADDRESS.
     *  \param i_Port The server port.
     *  \param i_WaitS Wait for the specified timeout and check if the connection
     *                 is established. -1 skips checking for a successfull connection.
     *
     *  \return 0 on success, -1 on failure.
     */
    
    extern int MRH_SRV_Connect(MRH_Srv_Context* p_Context, MRH_Srv_Server* p_Server, const char* p_Address, int i_Port, int i_WaitS);
    
    /**
     *  Create a password hash with a provided salt.
     *
     *  \param p_Buffer The password hash buffer. The buffer has to be of size
     *                  MRH_SRV_SIZE_PASSWORD.
     *  \param p_Password The account password to hash with. The buffer has to be of size
     *                    MRH_SRV_SIZE_ACCOUNT_PASSWORD.
     *  \param p_Salt The password hash salt to use. The buffer has to be of size
     *                MRH_SRV_SIZE_ACCOUNT_PASSWORD_SALT.
     *  \param u8_HashType The type of hash to use for the password.
     *
     *  \return 0 on success, -1 on failure.
     */
    
    extern int MRH_SRV_CreatePasswordHash(uint8_t* p_Buffer, const char* p_Password, const char* p_Salt, uint8_t u8_HashType);
    
    /**
     *  Create a nonce hash with a given password password.
     *
     *  \param p_Buffer The nonce hash buffer. The buffer has to be of size
     *                  MRH_SRV_SIZE_NONCE_HASH.
     *  \param u32_Nonce The nonce to hash.
     *  \param p_Password The password to hash with. The buffer has to
     *                    be of size MRH_SRV_SIZE_ACCOUNT_PASSWORD.
     *
     *  \return 0 on success, -1 on failure.
     */
    
    extern int MRH_SRV_CreateNonceHash(uint8_t* p_Buffer, uint32_t u32_Nonce, const uint8_t* p_Password);
    
    /**
     *  Disconnect from a server.
     *
     *  \param p_Server The server to disconnect from.
     */
    
    extern void MRH_SRV_Disconnect(MRH_Srv_Server* p_Server);
    
    /**
     *  Check if the server is connected.
     *
     *  \param p_Server The server to check.
     *
     *  \return 0 if connected, -1 if not.
     */
    
    extern int MRH_SRV_IsConnected(MRH_Srv_Server* p_Server);
    
    //*************************************************************************************
    // Recieve
    //*************************************************************************************

    /**
     *  Recieve a message from the connected channels. The given buffer has to match the
     *  recieve buffer size.
     *
     *  \param p_Server The server to check.
     *  \param p_Buffer The buffer to write the message. The buffer has to be of size
     *                  MRH_SRV_SIZE_MESSAGE_BUFFER.
     *  \param p_Password The password to use for message data decryption. NULL skips
     *                    decryption. The buffer has to be of size
     *                    MRH_SRV_SIZE_DEVICE_PASSWORD.
     *
     *  \return The recieved net message type on success, MRH_SRV_CS_MSG_UNK if nothing
     *          was recieved.
     */
    
    extern MRH_Srv_NetMessage MRH_SRV_RecieveMessage(MRH_Srv_Server* p_Server, uint8_t* p_Buffer, const char* p_Password);
    
    /**
     *  Set the data of a recieved message with a message buffer. The given buffer has to
     *  match the recieve buffer size.
     *
     *  \param p_Message The message to set.
     *  \param p_Buffer The buffer containing the message data. The buffer has to be of size
     *                  MRH_SRV_SIZE_MESSAGE_BUFFER.
     *
     *  \return 0 on success, -1 on failure.
     */
    
    int MRH_SRV_SetNetMessage(void* p_Message, const uint8_t* p_Buffer);
    
    //*************************************************************************************
    // Send
    //*************************************************************************************
    
    /**
     *  Send a message to a server.
     *
     *  \param p_Server The server to send to.
     *  \param e_Message The type of net message to send.
     *  \param p_Data The net message data to send.
     *  \param p_Password The password to use for message data encryption. NULL skips
     *                    encryption. The buffer has to be of size
     *                    MRH_SRV_SIZE_DEVICE_PASSWORD.
     *
     *  \return 0 if the message was sent, -1 on failure.
     */
    
    extern int MRH_SRV_SendMessage(MRH_Srv_Server* p_Server, MRH_Srv_NetMessage e_Message, const void* p_Data, const char* p_Password);
    
#ifdef __cplusplus
}
#endif

#endif /* MRH_Server_h */

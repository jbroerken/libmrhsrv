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
#include <stdlib.h>
#include <string.h>

// External

// Project
#include "../../include/libmrhsrv/libmrhsrv/MRH_Server.h"
#include "./Communication/MRH_CommunicationAuth.h"
#include "./Error/MRH_ServerErrorInternal.h"


//*************************************************************************************
// Connect
//*************************************************************************************

MRH_ServerConnection* MRH_SRV_Connect(MRH_Srv_Actor e_Client, const char* p_Address, const char* p_DeviceKey, const char* p_Password)
{
    return MRH_SRV_ConnectEx(e_Client, p_Address, MRH_SRV_DEFAULT_PORT, p_DeviceKey, p_Password, MRH_SRV_DEFAULT_MESSAGE_SIZE, MRH_SRV_DEFAULT_TIMEOUT_S);
}

MRH_ServerConnection* MRH_SRV_ConnectEx(MRH_Srv_Actor e_Client, const char* p_Address, int i_Port, const char* p_DeviceKey, const char* p_Password, MRH_Uint32 u32_MessageSize, MRH_Uint32 u32_TimeoutS)
{
    if (e_Client == MRH_SRV_SERVER_CONNECTION || e_Client == MRH_SRV_SERVER_COMMUNICATION ||
        p_Address == NULL || p_DeviceKey == NULL || p_Password == NULL ||
        u32_MessageSize == 0 || u32_TimeoutS == 0)
    {
        MRH_ERR_SetServerError(MRH_SERVER_ERROR_GENERAL_INVALID_PARAM);
        return NULL;
    }
    
    // First, build the connection info
    MRH_ServerConnection* p_Connection = (struct MRH_Srv_Connection_t*)malloc(sizeof(struct MRH_Srv_Connection_t));
    
    if (p_Connection == NULL)
    {
        MRH_ERR_SetServerError(MRH_SERVER_ERROR_GENERAL_MALLOC);
        return NULL;
    }
    
    // Set defaults
    p_Connection->p_Address = NULL;
    p_Connection->i_Port = i_Port;
    p_Connection->i_Socket = MRH_SRV_SOCKET_INVALID;
    p_Connection->i_PlatformConnected = (e_Client == MRH_SRV_CLIENT_PLATFORM ? 0 : -1);
    p_Connection->i_AppConnected = (e_Client == MRH_SRV_CLIENT_APP ? 0 : -1);
    p_Connection->u8_DeviceType = (MRH_Uint8)e_Client;
    p_Connection->p_DeviceKey = NULL;
    p_Connection->p_Password = NULL;
    p_Connection->u32_MessageSize = u32_MessageSize;
    p_Connection->c_MessageContainer.us_Messages = 0;
    p_Connection->u32_ConnectionTimeoutS = u32_TimeoutS;
    p_Connection->u32_HeartbeatTimerS = u32_TimeoutS; // Set later by server
    p_Connection->u64_NextHeartbeatS = 0;
    
    // Allocate info
    size_t us_AddressLen = strlen(p_Address);
    size_t us_DeviceKeyLen = strlen(p_DeviceKey);
    size_t us_PasswordLen = strlen(p_Password);
    
    p_Connection->p_Address = (char*)malloc(us_AddressLen + 1);
    p_Connection->p_DeviceKey = (char*)malloc(us_DeviceKeyLen + 1);
    p_Connection->p_Password = (char*)malloc(us_PasswordLen + 1);
    
    if (p_Connection->p_Address == NULL || p_Connection->p_DeviceKey == NULL || p_Connection->p_Password == NULL)
    {
        MRH_ERR_SetServerError(MRH_SERVER_ERROR_GENERAL_MALLOC);
        return MRH_SRV_Disconnect(p_Connection);
    }
    
    strncpy(p_Connection->p_Address, p_Address, us_AddressLen);
    p_Connection->p_Address[us_AddressLen] = '\0';
    
    strncpy(p_Connection->p_DeviceKey, p_DeviceKey, us_DeviceKeyLen);
    p_Connection->p_DeviceKey[us_DeviceKeyLen] = '\0';
    
    strncpy(p_Connection->p_Password, p_Password, us_PasswordLen);
    p_Connection->p_Password[us_PasswordLen] = '\0';
    
    // Connection defaults setup, perform auth
    if (MRH_ATH_ConnectConnectionServer(p_Connection) < 0 || MRH_ATH_ConnectCommunicationServer(p_Connection) < 0)
    {
        // Keep error from auth
        return MRH_SRV_Disconnect(p_Connection);
    }
    
    // All setup, return for usage
    return p_Connection;
}

//*************************************************************************************
// Disconnect
//*************************************************************************************

MRH_ServerConnection* MRH_SRV_Disconnect(MRH_ServerConnection* p_Connection)
{
    if (p_Connection == NULL)
    {
        MRH_ERR_SetServerError(MRH_SERVER_ERROR_GENERAL_INVALID_PARAM);
        return NULL;
    }
    
    // Close socket first
    if (p_Connection->i_Socket != MRH_SRV_SOCKET_INVALID)
    {
        MRH_ATH_Disconnect(p_Connection);
    }
    
    // Remove connection details
    if (p_Connection->p_Address != NULL)
    {
        free(p_Connection->p_Address);
    }
    
    if (p_Connection->p_DeviceKey != NULL)
    {
        free(p_Connection->p_DeviceKey);
    }
    
    if (p_Connection->p_Password != NULL)
    {
        free(p_Connection->p_Password);
    }
    
    // Clean up connection data
    size_t i, j;
    MRH_Srv_Message* p_Message;
    MRH_Srv_MessageChunk* p_Chunk;
    
    for (i = 0; i < p_Connection->c_MessageContainer.us_Messages; ++i)
    {
        if ((p_Message = p_Connection->c_MessageContainer.p_Messages[i]) == NULL)
        {
            continue;
        }
        
        for (j = 0; j < p_Message->us_Chunks; ++j)
        {
            if ((p_Chunk = p_Message->p_Chunks[j]) == NULL)
            {
                continue;
            }
            
            if (p_Chunk->p_Buffer != NULL)
            {
                free(p_Chunk->p_Buffer);
            }
            
            free(p_Message->p_Chunks[j]);
        }
        
        free(p_Connection->c_MessageContainer.p_Messages[i]);
    }
    
    p_Connection->c_MessageContainer.us_Messages = 0;
    
    // All cleared, done
    return NULL;
}

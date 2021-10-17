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

MRH_ServerConnection* MRH_SRV_Connect(MRH_Srv_Actor e_Client, const MRH_Srv_ConnectionInfo* p_Info)
{
    if (e_Client == MRH_SRV_SERVER_CONNECTION || e_Client == MRH_SRV_SERVER_COMMUNICATION ||
        p_Info == NULL || p_Info->u32_TimeoutS == 0)
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
    
    // Set connection info
    p_Connection->i_PlatformConnected = (e_Client != MRH_SRV_CLIENT_APP ? 0 : -1);
    p_Connection->i_AppConnected = (e_Client == MRH_SRV_CLIENT_APP ? 0 : -1);
    p_Connection->u8_DeviceType = (MRH_Uint8)e_Client;
    p_Connection->u32_ConnectionTimeoutS = p_Info->u32_TimeoutS;
    p_Connection->u32_HeartbeatTimerS = p_Info->u32_TimeoutS; // Set later by server
    p_Connection->u64_NextHeartbeatS = 0;
    
    // Set device info
    size_t us_DeviceKeyLen = strlen(p_Info->p_DeviceKey);
    size_t us_DevicePasswordLen = strlen(p_Info->p_DevicePassword);
    
    if (us_DeviceKeyLen == 0 || us_DeviceKeyLen > MRH_SRV_SIZE_DEVICE_KEY ||
        us_DevicePasswordLen == 0 || us_DevicePasswordLen > MRH_SRV_SIZE_DEVICE_PASSWORD)
    {
        MRH_ERR_SetServerError(MRH_SERVER_ERROR_GENERAL_INVALID_PARAM);
        return MRH_SRV_Disconnect(p_Connection);
    }
    
    memset(p_Connection->p_DeviceKey, '\0', MRH_SRV_SIZE_DEVICE_KEY);
    memset(p_Connection->p_DevicePassword, '\0', MRH_SRV_SIZE_DEVICE_PASSWORD);
    strncpy(p_Connection->p_DeviceKey, p_Connection->p_DeviceKey, us_DeviceKeyLen);
    strncpy(p_Connection->p_DevicePassword, p_Connection->p_DevicePassword, us_DevicePasswordLen);
    
    // Set connection server info
    size_t us_AddressLen = strlen(p_Info->p_ConnectionAddress);
    
    if (us_AddressLen == 0 || us_AddressLen > MRH_SRV_SIZE_SERVER_ADDRESS)
    {
        MRH_ERR_SetServerError(MRH_SERVER_ERROR_GENERAL_INVALID_PARAM);
        return MRH_SRV_Disconnect(p_Connection);
    }
    
    memset(p_Connection->p_ConnectionServer.p_Channel, '\0', MRH_SRV_SIZE_SERVER_CHANNEL);
    memset(p_Connection->p_ConnectionServer.p_Address, '\0', MRH_SRV_SIZE_DEVICE_KEY);
    strncpy(p_Connection->p_ConnectionServer.p_Address, p_Info->p_ConnectionAddress, us_AddressLen);
    p_Connection->p_ConnectionServer.i_Port = p_Info->i_ConnectionPort;
    p_Connection->p_ConnectionServer.i_Socket = MRH_SRV_SOCKET_INVALID;

    // Set default channel info
    for (int i = 0; i < MRH_SRV_SIZE_CHANNEL_COUNT; ++i)
    {
        memset(p_Connection->p_Channels[i].p_Channel, '\0', MRH_SRV_SIZE_SERVER_CHANNEL);
        memset(p_Connection->p_Channels[i].p_Address, '\0', MRH_SRV_SIZE_DEVICE_KEY);
        p_Connection->p_Channels[i].i_Port = -1;
        p_Connection->p_Channels[i].i_Socket = MRH_SRV_SOCKET_INVALID;
    }
    
    // Connection defaults setup, perform auth
    if (MRH_ATH_ConnectConnectionServer(p_Connection) < 0 || MRH_ATH_ConnectCommunicationServer(p_Connection) < 0)
    {
        // @NOTE: Do not set a error, keep error from auth
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
    
    // Close connections first
    MRH_ATH_Disconnect(p_Connection);
    
    // Now remove data
    free(p_Connection);
    
    // All cleared, done
    return NULL;
}

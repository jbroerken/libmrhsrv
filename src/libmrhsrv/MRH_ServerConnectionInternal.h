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

#ifndef MRH_ServerConnectionInternal_h
#define MRH_ServerConnectionInternal_h

// C

// External
#include <MRH_Typedefs.h>

// Project
#include "../../include/libmrhsrv/libmrhsrv/MRH_ServerConnection.h"
#include "../../include/libmrhsrv/libmrhsrv/MRH_ServerSizes.h"

// Pre-defined
#define MRH_SRV_SOCKET_INVALID -1


#ifdef __cplusplus
extern "C"
{
#endif
    
    //*************************************************************************************
    // Server
    //*************************************************************************************
    
    typedef struct MRH_Srv_Server_t
    {
        char p_Channel[MRH_SRV_SIZE_SERVER_CHANNEL];
        char p_Address[MRH_SRV_SIZE_SERVER_ADDRESS];
        int i_Port;
        int i_Socket;
        
    }MRH_Srv_Server;
    
    //*************************************************************************************
    // Connection
    //*************************************************************************************

    struct MRH_Srv_Connection_t
    {
        // Servers
        MRH_Srv_Server p_ConnectionServer;
        MRH_Srv_Server p_Channels[MRH_SRV_SIZE_CHANNEL_COUNT + 1]; // Connection server = first channel
        
        // Connection Info
        int i_PlatformConnected;
        int i_AppConnected;
        
        // Device
        MRH_Uint8 u8_DeviceType;
        char p_DeviceKey[MRH_SRV_SIZE_DEVICE_KEY];
        char p_DevicePassword[MRH_SRV_SIZE_DEVICE_PASSWORD];
        
        // Timings
        MRH_Uint32 u32_ConnectionTimeoutS;
        MRH_Uint32 u32_HeartbeatTimerS; // The timer to add after a heartbeat
        MRH_Uint64 u64_NextHeartbeatS; // Timepoint in seconds by system time
    };

#ifdef __cplusplus
}
#endif


#endif /* MRH_ServerConnectionInternal_h */

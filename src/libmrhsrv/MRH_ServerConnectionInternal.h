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

// Pre-defined
#define MRH_SRV_SOCKET_INVALID -1


#ifdef __cplusplus
extern "C"
{
#endif
    
    //*************************************************************************************
    // Messages
    //*************************************************************************************
    
    typedef struct MRH_Srv_MessageChunk_t
    {
        MRH_Uint8 u8_Type;
        MRH_Uint32 u32_Part;
        char* p_Buffer; // Size = connection message size
        
    }MRH_Srv_MessageChunk;
    
    typedef struct MRH_Srv_Message_t
    {
        MRH_Uint32 u32_ID;
        size_t us_Chunks;
        MRH_Srv_MessageChunk** p_Chunks;
        
    }MRH_Srv_Message;
    
    typedef struct MRH_Srv_MessageContainer_t
    {
        size_t us_Messages;
        MRH_Srv_Message** p_Messages;
        
    }MRH_Srv_MessageContainer;
    
    //*************************************************************************************
    // Location
    //*************************************************************************************
    
    typedef struct MRH_Srv_LastLocation_t
    {
        MRH_Sfloat64 f64_Latitude;
        MRH_Sfloat64 f64_Longtitude;
        MRH_Sfloat64 f64_Elevation;
        MRH_Sfloat64 f64_Facing;
        
    }MRH_Srv_LastLocation;
    
    //*************************************************************************************
    // Connection
    //*************************************************************************************

    struct MRH_Srv_Connection_t
    {
        // Address
        char* p_Address;
        int i_Port;
        int i_Socket;
        
        // Connection
        int i_PlatformConnected;
        int i_AppConnected;
        
        // Device
        MRH_Uint8 u8_DeviceType;
        char* p_DeviceKey;
        char* p_Password;
        
        // Message Info
        MRH_Uint32 u32_MessageSize; // Buffer bytes
        MRH_Srv_MessageContainer c_MessageContainer;
        
        // Location
        MRH_Srv_LastLocation c_LastLocation;
        
        // Timings
        MRH_Uint32 u32_ConnectionTimeoutS;
        MRH_Uint32 u32_HeartbeatTimerS; // The timer to add after a heartbeat
        MRH_Uint64 u64_NextHeartbeatS; // Timepoint in seconds by system time
    };

#ifdef __cplusplus
}
#endif


#endif /* MRH_ServerConnectionInternal_h */

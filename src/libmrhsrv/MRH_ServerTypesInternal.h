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

#ifndef MRH_ServerTypesInternal_h
#define MRH_ServerTypesInternal_h

// C

// External

// Project
#include "../../include/libmrhsrv/libmrhsrv/MRH_ServerTypes.h"
#include "../../include/libmrhsrv/libmrhsrv/MRH_ServerSizes.h"
#include "./Communication/MsQuic/MRH_MsQuicContext.h"

// Pre-defined
#define MRH_SRV_CONNECTION_SERVER_POS 0
#define MRH_SRV_PORT_INVALID -1


#ifdef __cplusplus
extern "C"
{
#endif
    
    //*************************************************************************************
    // Server
    //*************************************************************************************
    
    typedef struct MRH_Srv_Server_t
    {
        // Address
        char p_Address[MRH_SRV_SIZE_SERVER_ADDRESS];
        int i_Port;
        
        // Client
        uint8_t u8_DeviceType;
        
        // Connection context
        MRH_MsQuicConnection* p_MsQuic;
        
        // Timings
        int i_TimeoutMS;
        
    }MRH_Srv_Server;
    
    //*************************************************************************************
    // Connection
    //*************************************************************************************

    struct MRH_Srv_Context_t
    {
        // MsQuic
        const QUIC_API_TABLE* p_MsQuicAPI;
        HQUIC p_MsQuicRegistration;
        HQUIC p_MsQuicConfiguration;
        
        // Server
        int i_ServerMax;
        int i_ServerCur;
        
        // Client
        uint8_t u8_DeviceType;
        
        // Timings
        int i_TimeoutMS;
    };

#ifdef __cplusplus
}
#endif


#endif /* MRH_ServerTypesInternal_h */

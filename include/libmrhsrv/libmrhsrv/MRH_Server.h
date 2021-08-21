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

#ifndef MRH_Server_h
#define MRH_Server_h

// C
#include <stdlib.h>

// External
#include <MRH_Typedefs.h>

// Project
#include "./MRH_ServerConnection.h"
#include "./MRH_ServerActor.h"

// Pre-defined
#ifndef MRH_SRV_DEFAULT_MESSAGE_SIZE
    #define MRH_SRV_DEFAULT_MESSAGE_SIZE 1024
#endif
#ifndef MRH_SRV_DEFAULT_TIMEOUT_S
    #define MRH_SRV_DEFAULT_TIMEOUT_S 30
#endif
#ifndef MRH_SRV_DEFAULT_PORT
    #define MRH_SRV_DEFAULT_PORT 16096
#endif


#ifdef __cplusplus
extern "C"
{
#endif

    //*************************************************************************************
    // Connect
    //*************************************************************************************

    /**
     *  Connect a client to the server using the default message size and connection timeout.
     *  This call will block the calling thread until the communication server has been
     *  reached.
     *
     *  \param e_Client The client type.
     *  \param p_Address The connection server address.
     *  \param p_DeviceKey The device key of the platform device to connect with.
     *  \param p_Password The device password for the platform device to connect with.
     *
     *  \return The established connection on success, NULL on failure.
     */
    
    extern MRH_ServerConnection* MRH_SRV_Connect(MRH_Srv_Actor e_Client, const char* p_Address, const char* p_DeviceKey, const char* p_Password);
    
    /**
     *  Connect a client to the server using a custom message size and connection timeout.
     *  This call will block the calling thread until the communication server has been
     *  reached.
     *
     *  \param e_Client The client type.
     *  \param p_Address The connection server address.
     *  \param i_Port The connection server port.
     *  \param p_DeviceKey The device key of the platform device to connect with.
     *  \param p_Password The device password for the platform device to connect with.
     *  \param u32_MessageSize The size of each sent message in bytes.
     *  \param u32_TimeoutS The connection timeout in seconds.
     *
     *  \return The established connection on success, NULL on failure.
     */
    
    extern MRH_ServerConnection* MRH_SRV_ConnectEx(MRH_Srv_Actor e_Client, const char* p_Address, int i_Port, const char* p_DeviceKey, const char* p_Password, MRH_Uint32 u32_MessageSize, MRH_Uint32 u32_TimeoutS);
    
    //*************************************************************************************
    // Disconnect
    //*************************************************************************************
    
    /**
     *  Disconnect a client from the server.
     *
     *  \param p_Connection The connection to disconnect.
     *
     *  \return Always NULL.
     */
    
    extern MRH_ServerConnection* MRH_SRV_Disconnect(MRH_ServerConnection* p_Connection);
    
#ifdef __cplusplus
}
#endif

#endif /* MRH_Server_h */

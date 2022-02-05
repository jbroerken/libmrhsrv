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

#ifndef MRH_Server_h
#define MRH_Server_h

// C
#include <stdlib.h>

// External

// Project
#include "./MRH_ServerTypes.h"
#include "./MRH_ServerSizes.h"

// Pre-defined
#ifndef MRH_SRV_DEFAULT_TIMEOUT_MS
    #define MRH_SRV_DEFAULT_TIMEOUT_MS 60 * 1000
#endif
#ifndef MRH_SRV_DEFAULT_PORT
    #define MRH_SRV_DEFAULT_PORT 16096
#endif


#ifdef __cplusplus
extern "C"
{
#endif
    
    //*************************************************************************************
    // Context
    //*************************************************************************************

    /**
     *  Initialize the server connection object to use.
     *
     *  \param e_Client The client type.
     *  \param i_MaxServerCount The maximum number of servers creatable.
     *  \param i_TimeoutMS The connection timeout in milliseconds.
     *
     *  \return The connection object on success, NULL on failure.
     */
    
    extern MRH_Srv_Context* MRH_SRV_Init(MRH_Srv_Actor e_Client, int i_MaxServerCount, int i_TimeoutMS);
    
    /**
     *  Destroy a library context object.
     *
     *  \param p_Context The context to destroy.
     *
     *  \return Always NULL.
     */
    
    extern MRH_Srv_Context* MRH_SRV_Destroy(MRH_Srv_Context* p_Context);
    
    //*************************************************************************************
    // Server
    //*************************************************************************************
    
    /**
     *  Create a new server object.
     *
     *  \param p_Context The context to use.
     *
     *  \return The new server object on success, NULL on failure.
     */
    
    extern MRH_Srv_Server* MRH_SRV_CreateServer(MRH_Srv_Context* p_Context);
    
    /**
     *  Destroy a server object. The server will disconnect before destruction.
     *
     *  \param p_Context The context to use.
     *  \param p_Server The server to destroy.
     *
     *  \return Always NULL.
     */
    
    extern MRH_Srv_Server* MRH_SRV_DestroyServer(MRH_Srv_Context* p_Context, MRH_Srv_Server* p_Server);
    
#ifdef __cplusplus
}
#endif

#endif /* MRH_Server_h */

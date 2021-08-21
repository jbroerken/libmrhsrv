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

#ifndef MRH_CommunicationAuth_h
#define MRH_CommunicationAuth_h

// C

// External

// Project
#include "../MRH_ServerConnectionInternal.h"


#ifdef __cplusplus
extern "C"
{
#endif

    //*************************************************************************************
    // Connect
    //*************************************************************************************
    
    /**
     *  Connect to the connection server.
     *
     *  \param p_Connection The connection to connect with.
     *
     *  \return 0 on success, -1 on failure.
     */
    
    extern int MRH_ATH_ConnectConnectionServer(MRH_ServerConnection* p_Connection);
    
    /**
     *  Connect to the communication server.
     *
     *  \param p_Connection The connection to connect with.
     *
     *  \return 0 on success, -1 on failure.
     */
    
    extern int MRH_ATH_ConnectCommunicationServer(MRH_ServerConnection* p_Connection);
    
    //*************************************************************************************
    // Disconnect
    //*************************************************************************************
    
    /**
     *  Disconnect the current connection.
     *
     *  \param p_Connection The connect to disconnect.
     *
     *  \return 0 on success, -1 on failure.
     */
    
    extern int MRH_ATH_Disconnect(MRH_ServerConnection* p_Connection);

#ifdef __cplusplus
}
#endif

#endif /* MRH_CommunicationAuth_h */

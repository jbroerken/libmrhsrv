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
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// External

// Project
#include "./MRH_CommunicationAuth.h"
#include "../../../include/libmrhsrv/libmrhsrv/Communication/MRH_CommunicationOpCode.h"


//*************************************************************************************
// Connect
//*************************************************************************************

static int MRH_ATH_ConnectServer(MRH_ServerConnection* p_Connection)
{
    return -1;
}

int MRH_ATH_ConnectConnectionServer(MRH_ServerConnection* p_Connection)
{
    // @TODO: Connect to connection server, perform auth, read com server, return
    return -1;
}

int MRH_ATH_ConnectCommunicationServer(MRH_ServerConnection* p_Connection)
{
    // @TODO: Connecto to com server, perform auth, return
    return -1;
}

//*************************************************************************************
// Disconnect
//*************************************************************************************

static int MRH_ATG_DisconnectServer(MRH_Srv_Server* p_Server)
{
    if (p_Server->i_Socket == MRH_SRV_SOCKET_INVALID)
    {
        // Success, already closed
        return 0;
    }
    
    // @TODO: Send Disconnect, wait for sending complete (but not for result)
    
    // Now notified, close
    if (close(p_Server->i_Socket) < 0)
    {
        return -1;
    }
    
    p_Server->i_Socket = MRH_SRV_SOCKET_INVALID;
    return 0;
}

int MRH_ATH_Disconnect(MRH_ServerConnection* p_Connection)
{
    // Connection server connected?
    if (p_Connection->p_ConnectionServer.i_Socket != MRH_SRV_SOCKET_INVALID)
    {
        if (MRH_ATG_DisconnectServer(&(p_Connection->p_ConnectionServer)) != 0)
        {
            return -1;
        }
    }
    
    // Disconnect from all communication channels
    for (int i = 0; i < MRH_SRV_SIZE_CHANNEL_COUNT; ++i)
    {
        if (MRH_ATG_DisconnectServer(&(p_Connection->p_Channels[i])) != 0)
        {
            return -1;
        }
    }
    
    return 0;
}

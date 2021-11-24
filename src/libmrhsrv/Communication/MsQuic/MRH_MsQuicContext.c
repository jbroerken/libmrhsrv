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

// External

// Project
#include "./MRH_MsQuic.h"


//*************************************************************************************
// Connection
//*************************************************************************************

MRH_MsQuicConnection* MRH_MsQuicCreateConnection(const QUIC_API_TABLE* p_MsQuicAPI)
{
    MRH_MsQuicConnection* p_Connection = (MRH_MsQuicConnection*)malloc(sizeof(MRH_MsQuicConnection));
    
    if (p_Connection == NULL)
    {
        return NULL;
    }
    
    p_Connection->p_MsQuicAPI = p_MsQuicAPI;
    p_Connection->p_Connection = NULL;

    int i_Failed = -1;
    
    for (size_t i = 0; i < MRH_SRV_MESSAGE_BUFFER_COUNT; ++i)
    {
        p_Connection->p_Recieved[i].p_MsQuicAPI = p_MsQuicAPI;
        p_Connection->p_Recieved[i].p_Buffer = NULL;
        p_Connection->p_Recieved[i].us_SizeCur = 0;
        p_Connection->p_Recieved[i].us_SizeMax = 0;
        atomic_init(&(p_Connection->p_Recieved[i].i_State), MRH_MSQ_MESSAGE_FREE);
        
        p_Connection->p_Send[i].p_MsQuicAPI = p_MsQuicAPI;
        p_Connection->p_Send[i].p_Buffer = NULL;
        p_Connection->p_Send[i].us_SizeCur = 0;
        p_Connection->p_Send[i].us_SizeMax = 0;
        atomic_init(&(p_Connection->p_Send[i].i_State), MRH_MSQ_MESSAGE_FREE);
    }
    
    if (i_Failed == 0)
    {
        return MRH_MsQuicDestroyConnection(p_Connection);
    }
    
    return p_Connection;
}

MRH_MsQuicConnection* MRH_MsQuicDestroyConnection(MRH_MsQuicConnection* p_Connection)
{
    if (p_Connection->p_Connection != NULL)
    {
        p_Connection->p_MsQuicAPI->ConnectionShutdown(p_Connection->p_Connection,
                                                      QUIC_CONNECTION_SHUTDOWN_FLAG_NONE,
                                                      0);
        while (p_Connection->p_Connection != NULL) // Callback sets this, signals us that we can start deletion
        {
            sleep(1);
        }
    }
    
    // Streams are all closed after shutdown, so simply delete
    for (size_t i = 0; i < MRH_SRV_MESSAGE_BUFFER_COUNT; ++i)
    {
        if (p_Connection->p_Recieved[i].p_Buffer != NULL)
        {
            free(p_Connection->p_Recieved[i].p_Buffer);
        }
        
        if (p_Connection->p_Send[i].p_Buffer != NULL)
        {
            free(p_Connection->p_Send[i].p_Buffer);
        }
    }
    
    free(p_Connection);
    return NULL;
}

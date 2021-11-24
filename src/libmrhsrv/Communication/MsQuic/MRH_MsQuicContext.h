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

#ifndef MRH_MsQuicContext_h
#define MRH_MsQuicContext_h

// C
#include <stdatomic.h>

// External
#include <msquic.h>

// Project
#include "../../../../include/libmrhsrv/libmrhsrv/MRH_ServerSizes.h"

// Pre-defined
#ifndef MRH_SRV_MESSAGE_BUFFER_COUNT
    #define MRH_SRV_MESSAGE_BUFFER_COUNT 32
#endif


//*************************************************************************************
// Message
//*************************************************************************************

typedef enum
{
    MRH_MSQ_MESSAGE_FREE = 0,
    MRH_MSQ_MESSAGE_IN_USE = 1,
    MRH_MSQ_MESSAGE_COMPLETE = 2
    
}MRH_MSQ_MessageState;

typedef struct MRH_MsQuicMessage_t
{
    const QUIC_API_TABLE* p_MsQuicAPI;
    
    uint8_t* p_Buffer;
    size_t us_SizeCur;
    size_t us_SizeMax;
    
    _Atomic(int) i_State;
    
}MRH_MsQuicMessage;

//*************************************************************************************
// Connection
//*************************************************************************************

typedef struct MRH_MsQuicConnection_t
{
    const QUIC_API_TABLE* p_MsQuicAPI;
    
    _Atomic(HQUIC) p_Connection;
    
    struct MRH_MsQuicMessage_t p_Recieved[MRH_SRV_MESSAGE_BUFFER_COUNT];
    struct MRH_MsQuicMessage_t p_Send[MRH_SRV_MESSAGE_BUFFER_COUNT];
    
}MRH_MsQuicConnection;

/**
 *  Create a new empty conntection.
 *
 *  \param p_MsQuicAPI The api to hand to the context.
 *
 *  \return The connection on success, NULL on failure.
 */

extern MRH_MsQuicConnection* MRH_MsQuicCreateConnection(const QUIC_API_TABLE* p_MsQuicAPI);

/**
 *  Destrpy a conntection. The connection will be disconnected if active.
 *
 *  \param p_Connection The connection to destroy.
 *
 *  \return Always NULL.
 */

extern MRH_MsQuicConnection* MRH_MsQuicDestroyConnection(MRH_MsQuicConnection* p_Connection);


#endif /* MRH_MsQuicContext_h */

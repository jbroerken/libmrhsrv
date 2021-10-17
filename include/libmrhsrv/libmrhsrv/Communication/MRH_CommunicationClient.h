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

#ifndef MRH_CommunicationClient_h
#define MRH_CommunicationClient_h

// C

// External
#include <MRH_Typedefs.h>

// Project
#include "../MRH_ServerConnection.h"
#include "./MRH_CommunicationOpCode.h"


#ifdef __cplusplus
extern "C"
{
#endif
    
    //*************************************************************************************
    // Types
    //*************************************************************************************
    
    typedef struct MRH_Srv_RecievedOpCode_t
    {
        MRH_Srv_OpCode e_OpCode;
        void* p_OpCodeData;
        
    }MRH_Srv_RecievedOpCode;
    
    //*************************************************************************************
    // Send
    //*************************************************************************************

    /**
     *  Send a heartbeat to the server.
     *
     *  \param p_Connection The connection to send on.
     *
     *  \return 0 on success, -1 on failure.
     */

    extern int MRH_CC_SendHeartbeat(MRH_ServerConnection* p_Connection);
    
    /**
     *  Send a opcode to the server.
     *
     *  \param p_Connection The connection to send on.
     *  \param e_OpCode The OpCode type to send.
     *  \param p_OpCodeData The OpCode data to send.
     *  \param us_OpCodeSize The size of the opcode data to send.
     *
     *  \return 0 on success, -1 on failure.
     */
    
    extern int MRH_CC_SendOpCode(MRH_ServerConnection* p_Connection, MRH_Srv_OpCode e_OpCode, void* p_OpCodeData, size_t us_OpCodeSize);
    
    //*************************************************************************************
    // Recieve
    //*************************************************************************************
    
    /**
     *  Wait for data to be recieved.
     *
     *  \param p_Connection The connection to recieve on.
     *  \param u32_TimeoutMS The recieve timeout in milliseconds.
     *
     *  \return The recieved data on success, NULL on failure or no data recieved.
     */
    
    extern MRH_Srv_RecievedOpCode* MRH_CC_RecieveData(MRH_ServerConnection* p_Connection, MRH_Uint32 u32_TimeoutMS);
    
    //*************************************************************************************
    // Getters
    //*************************************************************************************
    
    /**
     *  Get the time in seconds until the next heartbeat has to be sent.
     *
     *  \param p_Connection The connection to check.
     *
     *  \return The heartbeat time in seconds.
     */
    
    extern MRH_Uint64 MRH_CC_GetNextHeartbeatS(const MRH_ServerConnection* p_Connection);

#ifdef __cplusplus
}
#endif

#endif /* MRH_CommunicationClient_h */

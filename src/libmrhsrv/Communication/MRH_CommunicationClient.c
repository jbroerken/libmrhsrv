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
#include <time.h>

// External

// Project
#include "../../../include/libmrhsrv/libmrhsrv/Communication/MRH_CommunicationClient.h"
#include "../MRH_ServerConnectionInternal.h"


//*************************************************************************************
// Send
//*************************************************************************************

int MRH_CC_SendHeartbeat(MRH_ServerConnection* p_Connection)
{
    return -1;
}

int MRH_CC_SendOpCode(MRH_ServerConnection* p_Connection, MRH_Srv_OpCode e_OpCode, void* p_OpCodeData, size_t us_OpCodeSize)
{
    return -1;
}

//*************************************************************************************
// Recieve
//*************************************************************************************

int MRH_CC_RecieveData(MRH_ServerConnection* p_Connection, MRH_Uint32 u32_TimeoutMS)
{
    return -1;
}

//*************************************************************************************
// Getters
//*************************************************************************************

MRH_Uint64 MRH_CC_GetNextHeartbeatS(const MRH_ServerConnection* p_Connection)
{
    if (time(NULL) >= p_Connection->u64_NextHeartbeatS)
    {
        return 0;
    }
    
    return p_Connection->u64_NextHeartbeatS - time(NULL);
}

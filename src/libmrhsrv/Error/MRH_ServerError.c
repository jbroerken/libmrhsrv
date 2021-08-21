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

// External

// Project
#include "./MRH_ServerErrorInternal.h"


//*************************************************************************************
// Error Data
//*************************************************************************************

// Last error
static MRH_Server_Error_Type e_LastError = MRH_SERVER_ERROR_NONE;

//*************************************************************************************
// Reset
//*************************************************************************************

void MRH_ERR_ServerReset(void)
{
    e_LastError = MRH_SERVER_ERROR_NONE;
}

//*************************************************************************************
// Getters
//*************************************************************************************

MRH_Server_Error_Type MRH_ERR_GetServerError(void)
{
    return e_LastError;
}

const char* MRH_ERR_GetServerErrorString(void)
{
    switch (e_LastError)
    {
        // None
        case MRH_SERVER_ERROR_NONE:
            return "No error";
            
        // Unknown
        case MRH_SERVER_ERROR_UNK:
            return "Unknown error";
            
        // General
        case MRH_SERVER_ERROR_GENERAL_INVALID_PARAM:
            return "Invalid parameters";
        case MRH_SERVER_ERROR_GENERAL_MALLOC:
            return "Memory allocation failed";
            
        default:
            return NULL;
    }
}

//*************************************************************************************
// Setters
//*************************************************************************************

void MRH_ERR_SetServiceError(MRH_Server_Error_Type e_Error)
{
    if (e_Error >= MRH_SERVER_ERROR_NONE && e_Error <= MRH_SERVER_ERROR_TYPE_MAX)
    {
        e_LastError = e_Error;
    }
    else
    {
        e_LastError = MRH_SERVER_ERROR_UNK;
    }
}

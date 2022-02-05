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

#ifndef MRH_ServerErrorInternal_h
#define MRH_ServerErrorInternal_h

// C

// External

// Project
#include "../../../include/libmrhsrv/libmrhsrv/Error/MRH_ServerError.h"


#ifdef __cplusplus
extern "C"
{
#endif

    //*************************************************************************************
    // Setters
    //*************************************************************************************

    /**
     *  Set library error.
     *
     *  \param e_Error The current library error.
     */

    extern void MRH_ERR_SetServerError(MRH_Server_Error_Type e_Error);

#ifdef __cplusplus
}
#endif

#endif /* MRH_ServerErrorInternal_h */

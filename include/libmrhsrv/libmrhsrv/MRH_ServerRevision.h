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

#ifndef MRH_ServerRevision_h
#define MRH_ServerRevision_h

// C

// External

// Project


#ifdef __cplusplus
extern "C"
{
#endif

    //*************************************************************************************
    // Library Version
    //*************************************************************************************

    /**
     *  Get the library major version number.
     *
     *  \return The library major version number.
     */

    extern int MRH_GetServerLibraryVersionMajor(void);

    /**
     *  Get the library minor version number.
     *
     *  \return The library minor version number.
     */

    extern int MRH_GetServerLibraryVersionMinor(void);

    /**
     *  Get the library patch version number.
     *
     *  \return The library patch version number.
     */

    extern int MRH_GetServerLibraryVersionPatch(void);

#ifdef __cplusplus
}
#endif

//*************************************************************************************
// Revision
//*************************************************************************************

#define MRH_SERVER_LIB_VERSION_MAJOR 1
#define MRH_SERVER_LIB_VERSION_MINOR 0
#define MRH_SERVER_LIB_VERSION_PATCH 0


#endif /* MRH_ServerRevision_h */

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

#ifndef MRH_CommunicationApp_h
#define MRH_CommunicationApp_h

// C

// External

// Project
#include "./MRH_CommunicationClient.h"


#ifdef __cplusplus
extern "C"
{
#endif
    
    //*************************************************************************************
    // Types
    //*************************************************************************************

    typedef struct MRH_CA_ClientLocation_t
    {
        MRH_Sfloat64 f64_Latitude;
        MRH_Sfloat64 f64_Longtitude;
        MRH_Sfloat64 f64_Elevation;
        MRH_Sfloat64 f64_Facing;
        
    }MRH_CA_ClientLocation;
    
    //*************************************************************************************
    // Send
    //*************************************************************************************

    /**
     *  Send location data to the server.
     *
     *  \param p_Connection The connection to send on.
     *  \param p_Location The location data to send.
     *
     *  \return 0 on success, -1 on failure.
     */
    
    extern int MRH_CA_SendLocation(MRH_ServerConnection* p_Connection, const MRH_CA_ClientLocation* p_Location);

#ifdef __cplusplus
}
#endif

#endif /* MRH_CommunicationApp_h */

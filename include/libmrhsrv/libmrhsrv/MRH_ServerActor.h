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

#ifndef MRH_ServerActor_h
#define MRH_ServerActor_h

// C

// External

// Project


#ifdef __cplusplus
extern "C"
{
#endif

    //*************************************************************************************
    // Actors
    //*************************************************************************************
    
    typedef enum
    {
        MRH_SRV_CLIENT_APP = 0,
        
        MRH_SRV_CLIENT_PLATFORM_LISTEN = 1,
        MRH_SRV_CLIENT_PLATFORM_SAY = 2,
        MRH_SRV_CLIENT_PLATFORM_USER = 3,
        
        MRH_SRV_SERVER_CONNECTION = 4,
        MRH_SRV_SERVER_COMMUNICATION = 5,
        
        MRH_SRV_ACTOR_MAX = MRH_SRV_SERVER_COMMUNICATION,
        
        MRH_SRV_ACTOR_COUNT = MRH_SRV_ACTOR_MAX + 1
        
    }MRH_Srv_Actor;
    
#ifdef __cplusplus
}
#endif

#endif /* MRH_ServerActor_h */

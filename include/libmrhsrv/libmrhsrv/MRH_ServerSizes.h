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

#ifndef MRH_ServerSizes_h
#define MRH_ServerSizes_h

// C

// External

// Project

// Pre-defined
#define MRH_SRV_SIZE_SERVER_ADDRESS 512
#define MRH_SRV_SIZE_SERVER_CHANNEL 256

#define MRH_SRV_SIZE_DEVICE_KEY 512
#define MRH_SRV_SIZE_DEVICE_PASSWORD 1024

#define MRH_SRV_SIZE_CHANNEL_COUNT 2 // Excludes connection server

#define MRH_SRV_SIZE_CHANNEL_SPEECH_BUFFER 512


#endif /* MRH_ServerSizes_h */

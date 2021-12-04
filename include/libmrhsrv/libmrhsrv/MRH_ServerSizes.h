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
#include <sodium.h>

// Project

// Pre-defined
#define MRH_SRV_SIZE_SERVER_ADDRESS 256
#define MRH_SRV_SIZE_SERVER_CHANNEL 64

#define MRH_SRV_SIZE_ACCOUNT_MAIL 256
#define MRH_SRV_SIZE_ACCOUNT_PASSWORD crypto_secretbox_KEYBYTES // Max key length
#define MRH_SRV_SIZE_ACCOUNT_PASSWORD_SALT crypto_pwhash_SALTBYTES // Salt used for pw hash

#define MRH_SRV_SIZE_NONCE_HASH crypto_secretbox_NONCEBYTES + crypto_secretbox_MACBYTES + sizeof(uint32_t) // Hashed nonce bytes

#define MRH_SRV_SIZE_DEVICE_KEY 64
#define MRH_SRV_SIZE_DEVICE_PASSWORD 256

#define MRH_SRV_SIZE_MESSAGE_BUFFER 2048 // Recieve / send size


#endif /* MRH_ServerSizes_h */

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

#ifndef MRH_ServerSizes_h
#define MRH_ServerSizes_h

// C

// External

// Project

// Pre-defined
#define MRH_SRV_SIZE_SERVER_ADDRESS 256

#define MRH_SRV_SIZE_ACCOUNT_MAIL 128
#define MRH_SRV_SIZE_ACCOUNT_PASSWORD 32 // Max key length, equals crypto_secretbox_KEYBYTES and crypto_box_SEEDBYTES
#define MRH_SRV_SIZE_ACCOUNT_PASSWORD_SALT 16 // Salt used for pw hash (crypto_pwhash_SALTBYTES)

#define MRH_SRV_SIZE_NONCE_HASH 24 + 16 + sizeof(uint32_t) // Hashed nonce bytes (crypto_secretbox_NONCEBYTES + crypto_secretbox_MACBYTES + 4)

#define MRH_SRV_SIZE_DEVICE_KEY 25
#define MRH_SRV_SIZE_DEVICE_PASSWORD MRH_SRV_SIZE_ACCOUNT_PASSWORD // Uses same size for sodium

#define MRH_SRV_SIZE_MESSAGE_BUFFER_MAX 1024 // Recieve / send size

#define MRH_SRV_SIZE_TEXT_STRING MRH_SRV_SIZE_MESSAGE_BUFFER_MAX - 9 // Type and time stamp
#define MRH_SRV_SIZE_CUSTOM_BUFFER MRH_SRV_SIZE_MESSAGE_BUFFER_MAX - 1
#define MRH_SRV_SIZE_NOTIFICATION_STRING 256


#endif /* MRH_ServerSizes_h */

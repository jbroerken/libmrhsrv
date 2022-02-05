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

#ifndef MRH_NetMessage_h
#define MRH_NetMessage_h

// C
#include <stdint.h>

// External

// Project
#include "../MRH_ServerSizes.h"

// Pre-defined
#define MRH_SRV_NET_MESSAGE_VERSION 1


#ifdef __cplusplus
extern "C"
{
#endif

    //*************************************************************************************
    // Message List
    //*************************************************************************************
    
    typedef enum
    {
        /**
         *  Net Message Version 1
         */
        
        // Unknown
        MRH_SRV_MSG_UNK = 0,                        // Unknown data
        
        // Server Auth
        MRH_SRV_MSG_AUTH_REQUEST = 1,               // Request authentication
        MRH_SRV_MSG_AUTH_CHALLENGE = 2,             // Challenge client to provide auth data
        MRH_SRV_MSG_AUTH_PROOF = 3,                 // Provide proof of valid auth data
        MRH_SRV_MSG_AUTH_RESULT = 4,                // Authentication result
        
        // Communication
        MRH_SRV_MSG_DATA_AVAIL = 5,                 // Client requests data
        MRH_SRV_MSG_NO_DATA = 6,                    // No data available
        MRH_SRV_MSG_TEXT = 7,                       // Text data
        MRH_SRV_MSG_LOCATION,                       // Location data
        MRH_SRV_MSG_NOTIFICATION,                   // Push Notification
        MRH_SRV_MSG_CUSTOM,                         // Custom data
        
        // Bounds
        MRH_SRV_NET_MESSAGE_MAX = MRH_SRV_MSG_CUSTOM,
        
        MRH_SRV_NET_MESSAGE_COUNT = MRH_SRV_NET_MESSAGE_MAX + 1
        
    }MRH_Srv_NetMessage;
    
    //*************************************************************************************
    // NetMessage Errors
    //*************************************************************************************
    
    typedef enum
    {
        /**
         *  Net Message Version 1
         */
        
        // No Error
        MRH_SRV_NET_MESSAGE_ERR_NONE = 0,                       // No error
        
        // Unk
        MRH_SRV_NET_MESSAGE_ERR_UNK = 1,                        // ???
        
        // Server General
        MRH_SRV_NET_MESSAGE_ERR_SG_ERROR = 2,                   // Internal server error
        
        // Server Auth
        MRH_SRV_NET_MESSAGE_ERR_SA_VERSION = 3,                 // Wrong OpCode Version
        MRH_SRV_NET_MESSAGE_ERR_SA_ACCOUNT = 4,                 // Account data given was wrong
        MRH_SRV_NET_MESSAGE_ERR_SA_ALREADY_CONNECTED = 5,       // Connection already exists
        MRH_SRV_NET_MESSAGE_ERR_SA_MAINTENANCE = 6,             // Temporary downtime
        MRH_SRV_NET_MESSAGE_ERR_SA_UNK_ACTOR = 7,               // Unknown actor type
        MRH_SRV_NET_MESSAGE_ERR_SA_NO_DEVICE,                   // No device found for connection
        
        // Bounds
        MRH_SRV_NET_MESSAGE_ERROR_MAX = MRH_SRV_NET_MESSAGE_ERR_SA_EXPIRED,
        
        MRH_SRV_NET_MESSAGE_ERROR_COUNT = MRH_SRV_NET_MESSAGE_ERROR_MAX + 1
        
    }MRH_Srv_NetMessage_Error;
    
    //*************************************************************************************
    // NetMessage Data
    //*************************************************************************************

    // @NOTE: Messages without data are not listed!
    
    /**
     *  NetMessage Version 1
     */
    
    //
    //  Server Auth
    //
    
    typedef struct MRH_SRV_MSG_AUTH_REQUEST_DATA_t
    {
        char p_Mail[MRH_SRV_SIZE_ACCOUNT_MAIL]; // The account mail
        char p_DeviceKey[MRH_SRV_SIZE_DEVICE_KEY]; // Device valid for server
        uint8_t u8_ClientType;  // Which type of client (platform or app)
        uint8_t u8_Version; // NetMessage version in use
        
    }MRH_SRV_MSG_AUTH_REQUEST_DATA;
    
    typedef struct MRH_SRV_S_MSG_AUTH_CHALLENGE_DATA_t
    {
        char p_Salt[MRH_SRV_SIZE_ACCOUNT_PASSWORD_SALT]; // Salt to use for pw hash
        uint32_t u32_Nonce; // Nonce to hash
        uint8_t u8_HashType;
        
    }MRH_SRV_MSG_AUTH_CHALLENGE_DATA;
    
    typedef struct MRH_SRV_C_MSG_AUTH_PROOF_DATA_t
    {
        uint8_t p_NonceHash[MRH_SRV_SIZE_NONCE_HASH]; // Created hash
        
    }MRH_SRV_MSG_AUTH_PROOF_DATA;
    
    typedef struct MRH_SRV_S_MSG_AUTH_RESULT_DATA_t
    {
        uint8_t u8_Result; // The result of the auth state
        
    }MRH_SRV_MSG_AUTH_RESULT_DATA;
    
    //
    //  Communication
    //
    
    typedef struct MRH_SRV_MSG_DATA_AVAIL_DATA_t
    {
        uint8_t u8_Data; // The type of data requested
        
    }MRH_SRV_MSG_DATA_AVAIL_DATA;
    
    typedef struct MRH_SRV_MSG_NO_DATA_DATA_t
    {
        uint8_t u8_Data; // The type of data requested
        
    }MRH_SRV_MSG_NO_DATA_DATA;
    
    typedef struct MRH_SRV_MSG_TEXT_DATA_t
    {
        char p_String[MRH_SRV_SIZE_TEXT_STRING]; // UTF-8
        uint64_t u64_TimestampS;
        
    }MRH_SRV_MSG_TEXT_DATA;
    
    typedef struct MRH_SRV_MSG_LOCATION_DATA_t
    {
        float f32_Latitude;
        float f32_Longtitude;
        float f32_Elevation;
        float f32_Facing;
        uint64_t u64_TimestampS;
        
    }MRH_SRV_MSG_LOCATION_DATA;
    
    typedef struct MRH_SRV_MSG_NOTIFICATION_DATA_t
    {
        char p_String[MRH_SRV_SIZE_NOTIFICATION_STRING]; // UTF-8
        
    }MRH_SRV_MSG_NOTIFICATION_DATA;
    
    typedef struct MRH_SRV_MSG_CUSTOM_DATA_t
    {
        uint8_t p_Buffer[MRH_SRV_SIZE_CUSTOM_BUFFER];
        
    }MRH_SRV_MSG_CUSTOM_DATA;
    
#ifdef __cplusplus
}
#endif

#endif /* MRH_NetMessage_h */

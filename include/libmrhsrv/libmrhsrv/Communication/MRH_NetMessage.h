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
        MRH_SRV_CS_MSG_UNK = 0,                     // Unknown data
        
        // Availability
        MRH_SRV_C_MSG_HELLO = 1,                    // Client - Signal availability
        MRH_SRV_S_MSG_PARTNER_CONNECTED = 2,        // Server - Partner client connected
        
        // Server Auth
        MRH_SRV_C_MSG_AUTH_REQUEST = 3,             // Client - Request authentication
        MRH_SRV_S_MSG_AUTH_CHALLENGE = 4,           // Server - Challenge client to provide auth data
        MRH_SRV_C_MSG_AUTH_PROOF = 5,               // Client - Provide proof of valid auth data
        MRH_SRV_S_MSG_AUTH_RESULT = 6,              // Server - Proof check result
        
        // Device Auth
        MRH_SRV_C_MSG_PAIR_CHALLENGE = 7,           // Client - Give nonce etc to partner to hash
        MRH_SRV_C_MSG_PAIR_PROOF,                   // Client - Return the hashed nonce and info
        MRH_SRV_C_MSG_PAIR_RESULT,                  // Client - Result of pair comparison
        
        // Channel
        MRH_SRV_C_MSG_CHANNEL_REQUEST,              // Client - Request channel info
        MRH_SRV_S_MSG_CHANNEL_RESPONSE,             // Server - Provide channel info
        
        // Text
        MRH_SRV_C_MSG_TEXT,                         // Client - Send text string
        
        // Location
        MRH_SRV_C_MSG_LOCATION,                     // Client - Send location data
        
        // Custom
        MRH_SRV_C_MSG_CUSTOM,                       // Client - Custom message
        MRH_SRV_S_MSG_CUSTOM,                       // Server - Custom message
        
        // Bounds
        MRH_SRV_NET_MESSAGE_MAX = MRH_SRV_S_MSG_CUSTOM,
        
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
        
        // Server Auth
        MRH_SRV_NET_MESSAGE_ERR_SA_NO_DEVICE = 2,               // No device found for device key
        MRH_SRV_NET_MESSAGE_ERR_SA_VERSION = 3,                 // Wrong OpCode Version
        MRH_SRV_NET_MESSAGE_ERR_SA_UNK_ACTOR = 4,               // Unknown actor id
        MRH_SRV_NET_MESSAGE_ERR_SA_ACCOUNT = 5,                 // Account data given was wrong
        MRH_SRV_NET_MESSAGE_ERR_SA_ALREADY_CONNECTED = 6,       // A connecting for this already exists
        MRH_SRV_NET_MESSAGE_ERR_SA_FULL = 7,                    // Server is full
        MRH_SRV_NET_MESSAGE_ERR_SA_NO_PLATFORM,                 // No platform client found for app client
        MRH_SRV_NET_MESSAGE_ERR_SA_MAINTENANCE,                 // Temporary downtime
        
        // Device Auth
        MRH_SRV_NET_MESSAGE_ERR_DA_PAIR,                        // Device pairing failed
        
        // Bounds
        MRH_SRV_NET_MESSAGE_ERROR_MAX = MRH_SRV_NET_MESSAGE_ERR_DA_PAIR,
        
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
    
    typedef struct MRH_SRV_C_MSG_AUTH_REQUEST_DATA_t
    {
        uint8_t u8_Actor;  // Which type of client (platform or app)
        uint8_t u8_Version; // NetMessage version in use
        
    }MRH_SRV_C_MSG_AUTH_REQUEST_DATA;
    
    typedef struct MRH_SRV_S_MSG_AUTH_CHALLENGE_DATA_t
    {
        char p_Salt[MRH_SRV_SIZE_ACCOUNT_PASSWORD_SALT]; // Salt to use for pw hash
        uint32_t u32_Nonce; // Nonce to hash
        
    }MRH_SRV_S_MSG_AUTH_CHALLENGE_DATA;
    
    typedef struct MRH_SRV_C_MSG_AUTH_PROOF_DATA_t
    {
        char p_Mail[MRH_SRV_SIZE_ACCOUNT_MAIL]; // The account mail
        char p_NonceHash[MRH_SRV_SIZE_NONCE_HASH]; // Created hash
        char p_DeviceKey[MRH_SRV_SIZE_DEVICE_KEY]; // Device valid for server
        
    }MRH_SRV_C_MSG_AUTH_PROOF_DATA;
    
    typedef struct MRH_SRV_S_MSG_AUTH_RESULT_DATA_t
    {
        uint8_t u8_Result; // The result of the auth
        
    }MRH_SRV_S_MSG_AUTH_RESULT_DATA;
    
    //
    //  Device Auth
    //
    
    typedef struct MRH_SRV_C_MSG_PAIR_CHALLENGE_DATA_t
    {
        uint32_t u32_Nonce;
        
    }MRH_SRV_C_MSG_PAIR_CHALLENGE_DATA;
    
    typedef struct MRH_SRV_C_MSG_PAIR_PROOF_DATA_t
    {
        uint8_t u8_Actor; // Type of partner to pair with
        char p_NonceHash[MRH_SRV_SIZE_NONCE_HASH]; // Password hashed nonce
        char p_DeviceKey[MRH_SRV_SIZE_DEVICE_KEY]; // Target verification
        
    }MRH_SRV_C_MSG_PAIR_PROOF_DATA;
    
    typedef struct MRH_SRV_C_MSG_PAIR_RESULT_DATA_t
    {
        uint8_t u8_Result; // Result of device pair
        
    }MRH_SRV_C_MSG_PAIR_RESULT_DATA;
    
    //
    //  Channel
    //
    
    typedef struct MRH_SRV_C_MSG_CHANNEL_REQUEST_DATA_t
    {
        uint8_t u8_Actor; // Platform = Get free, App = Get Platform
        char p_Channel[MRH_SRV_SIZE_SERVER_CHANNEL]; // Requested channel
        
    }MRH_SRV_C_MSG_CHANNEL_REQUEST_DATA;
    
    typedef struct MRH_SRV_S_MSG_CHANNEL_RESPONSE_DATA_t
    {
        char p_Channel[MRH_SRV_SIZE_SERVER_CHANNEL]; // Requested channel
        char p_Address[MRH_SRV_SIZE_SERVER_ADDRESS];
        uint16_t u16_Port;
        
    }MRH_SRV_S_MSG_CHANNEL_RESPONSE_DATA;
    
    //
    //  Text
    //
    
    typedef struct MRH_SRV_C_MSG_TEXT_DATA_t
    {
        char p_String[MRH_SRV_SIZE_MESSAGE_BUFFER]; // UTF-8
        
    }MRH_SRV_C_MSG_TEXT_DATA;
    
    //
    // Location
    //
    
    typedef struct MRH_SRV_C_MSG_LOCATION_DATA_t
    {
        float f32_Latitude;
        float f32_Longtitude;
        float f32_Elevation;
        float f32_Facing;
        
    }MRH_SRV_C_MSG_LOCATION_DATA;
    
    //
    // Custom
    //
    
    typedef struct MRH_SRV_C_MSG_CUSTOM_DATA_t
    {
        char p_Buffer[MRH_SRV_SIZE_MESSAGE_BUFFER]; // Whatever the client sent
        
    }MRH_SRV_C_MSG_CUSTOM_DATA;
    
    typedef struct MRH_SRV_S_MSG_CUSTOM_DATA_t
    {
        char p_Buffer[MRH_SRV_SIZE_MESSAGE_BUFFER]; // Whatever the server sent
        
    }MRH_SRV_S_MSG_CUSTOM_DATA;
    
#ifdef __cplusplus
}
#endif

#endif /* MRH_NetMessage_h */

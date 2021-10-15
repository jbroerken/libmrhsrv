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

#ifndef MRH_CommunicationOpCode_h
#define MRH_CommunicationOpCode_h

// C

// External
#include <MRH_Typedefs.h>

// Project

// Pre-defined
#define MRH_SRV_OPCODE_VERSION 1

#define MRH_SRV_OPCODE_COM_CHANNEL_SIZE 128
#define MRH_SRV_OPCODE_DEVICE_ID_SIZE 64
#define MRH_SRV_OPCODE_ADDRESS_SIZE 512


#ifdef __cplusplus
extern "C"
{
#endif

    //*************************************************************************************
    // OpCodes
    //*************************************************************************************
    
    typedef enum
    {
        /**
         *  OpCode Version 1
         */
        
        // Unknown
        MRH_SRV_UNK = 0,                            // Unknown data
        
        // Connection
        MRH_SRV_CONNECTED = 1,                      // Inform other of actor connected
        MRH_SRV_DISCONNECTED = 2,                   // Inform others of actor disconnected (includes server actors)
        
        // Heartbeat
        MRH_SRV_HEARTBEAT = 3,                      // Connection signals active
        MRH_SRV_HEARTBEAT_CHANGE = 4,               // Heartbeat interval change
        
        // Connection Server Auth
        MRH_SRV_CON_AUTH_REQUEST = 5,               // Client requests authentication to connection server
        MRH_SRV_CON_AUTH_CHALLENGE = 6,             // Challenge new connected client to provide valid connection details
        MRH_SRV_CON_AUTH_PROOF = 7,                 // Proof for valid client connection to connection server
        MRH_SRV_CON_AUTH_RESULT,                    // Authentification result of the connection server
        MRH_SRV_CON_COM_TARGET,                     // The address and port of the communication server to connect to
        
        // Communication Server Auth
        MRH_SRV_COM_AUTH_REQUEST,                   // Client requests authentification to communication server
        MRH_SRV_COM_AUTH_CHALLENGE,                 // Challenge new connected client to provide valid connection details
        MRH_SRV_COM_AUTH_PROOF,                     // Proof for valid client connection to communication server
        MRH_SRV_COM_AUTH_RESULT,                    // Authentification result of the communication server
        MRH_SRV_COM_AUTH_NONCE_PLATFORM,            // Send a unencrypted nonce from the platform to the app
        MRH_SRV_COM_AUTH_NONCE_APP,                 // Return a password encrypted nonce from the app to the platform
        MRH_SRV_COM_AUTH_NONCE_RESULT,              // Send the nonce result to the app and server
        
        // Text Messages
        MRH_SRV_MSG_CHUNK,                          // Text message chunk
        
        // Location
        MRH_SRV_LOCATION,                           // Current user location
        
        // Bounds
        MRH_SRV_OPCODE_MAX = MRH_SRV_LOCATION,
        
        MRH_SRV_OPCODE_COUNT = MRH_SRV_OPCODE_MAX + 1
        
    }MRH_Srv_OpCode;
    
    //*************************************************************************************
    // OpCode Errors
    //*************************************************************************************
    
    typedef enum
    {
        /**
         *  OpCode Version 1
         */
        
        MRH_SRV_OPCODE_ERR_NONE = 0,                    // No error
        
        MRH_SRV_OPCODE_ERR_UNK = 1,                     // ???
        
        MRH_SRV_OPCODE_ERR_NO_DEVICE = 2,               // No device found for device key
        MRH_SRV_OPCODE_ERR_WRONG_ANSWER = 3,            // Answer to question was wrong
        MRH_SRV_OPCODE_ERR_VERSION = 3,                 // Wrong OpCode Version
        MRH_SRV_OPCODE_ERR_UNK_ACTOR = 4,               // Unknown actor id
        MRH_SRV_OPCODE_ERR_MSG_SIZE = 5,                // Wrong / invalid message size
        MRH_SRV_OPCODE_ERR_NONCE = 6,                   // Nonce was wrong
        MRH_SRV_OPCODE_ERR_ALREADY_CONNECTED = 7,       // A connector for this already exists
        MRH_SRV_OPCODE_ERR_FULL = 8,                    // Server is full
        MRH_SRV_OPCODE_ERR_MAINTENANCE,                 // Temporary downtime
        
        // Bounds
        MRH_SRV_OPCODE_ERROR_MAX = MRH_SRV_OPCODE_ERR_MAINTENANCE,
        
        MRH_SRV_OPCODE_ERROR_COUNT = MRH_SRV_OPCODE_ERROR_MAX + 1
        
    }MRH_Srv_Opcode_Error;
    
    //*************************************************************************************
    // OpCode Data
    //*************************************************************************************

    /**
     *  OpCode Version 1
     */
    
    //
    // Unk
    //
    
    // MRH_SRV_UNK - No Data
    
    //
    // Connection
    //
    
    // MRH_SRV_CONNECTED
    typedef struct MRH_SRV_OPCODE_CONNECTED_t
    {
        MRH_Uint8 u8_Actor; // Which actor connected
        
    }MRH_SRV_OPCODE_CONNECTED;
    
    // MRH_SRV_DISCONNECTED
    typedef struct MRH_SRV_OPCODE_DISCONNECTED_t
    {
        MRH_Uint8 u8_Actor; // Which actor disconnected
        
    }MRH_SRV_OPCODE_DISCONNECTED;
    
    //
    // Heartbeat
    //
    
    // MRH_SRV_HEARTBEAT
    typedef struct MRH_SRV_OPCODE_HEARTBEAT_t
    {
        MRH_Uint32 u32_TimeS; // Max time until the next heartbeat is sent
        
    }MRH_SRV_OPCODE_HEARTBEAT;
    
    // MRH_SRV_HEARTBEAT_CHANGE
    typedef struct MRH_SRV_OPCODE_HEARTBEAT_CHANGE_t
    {
        MRH_Uint32 u32_TimeS; // New timeframe in which a heartbeat has to be sent
        
    }MRH_SRV_OPCODE_HEARTBEAT_CHANGE;
    
    //
    // Connection Server Auth
    //
    
    // MRH_SRV_CON_AUTH_REQUEST
    typedef struct MRH_SRV_OPCODE_CON_AUTH_REQUEST_t
    {
        MRH_Uint8 u8_Actor; // Which type of client (platform or app)
        char p_Channel[MRH_SRV_OPCODE_COM_CHANNEL_SIZE]; // Which type of communication server to target
        
    }MRH_SRV_OPCODE_CON_AUTH_REQUEST;
    
    // MRH_SRV_CON_AUTH_CHALLENGE - No Data
    
    // MRH_SRV_CON_AUTH_PROOF
    typedef struct MRH_SRV_OPCODE_CON_AUTH_PROOF_t
    {
        MRH_Uint32 u32_OpCodeVer; // Ignore outdated or future
        char p_DeviceID[MRH_SRV_OPCODE_DEVICE_ID_SIZE]; // Needed to get the right communication server
        
    }MRH_SRV_OPCODE_CON_AUTH_PROOF;
    
    // MRH_SRV_CON_AUTH_RESULT
    typedef struct MRH_SRV_OPCODE_CON_AUTH_RESULT_t
    {
        MRH_Uint32 u32_Error; // Exact error id, 0 is OK
        
    }MRH_SRV_OPCODE_CON_AUTH_Result;
    
    // MRH_SRV_CON_COM_TARGET
    typedef struct MRH_SRV_OPCODE_CON_COM_TARGET_t
    {
        char p_Address[MRH_SRV_OPCODE_ADDRESS_SIZE];
        MRH_Uint32 u32_Port;
        
    }MRH_SRV_OPCODE_CON_COM_TARGET;
    
    //
    // Communication Server Auth
    //
    
    // MRH_SRV_COM_AUTH_REQUEST - No Data
    
    // MRH_SRV_COM_AUTH_CHALLENGE - No Data
    
    // MRH_SRV_COM_AUTH_PROOF
    typedef struct MRH_SRV_OPCODE_COM_AUTH_PROOF_t
    {
        MRH_Uint8 u8_Actor; // Which type of client (platform or app)
        // @NOTE: Opcode Ver checked by connection server
        char p_DeviceID[MRH_SRV_OPCODE_DEVICE_ID_SIZE]; // Needed to get the right communication server
        MRH_Uint32 u32_MessageSize; // The server message sizes
        
    }MRH_SRV_OPCODE_COM_AUTH_PROOF_CON;
    
    // MRH_SRV_COM_AUTH_RESULT
    typedef struct MRH_SRV_OPCODE_COM_AUTH_RESULT_t
    {
        MRH_Uint32 u32_Error; // Exact error id, 0 is OK
        
    }MRH_SRV_OPCODE_COM_AUTH_Result;
    
    // MRH_SRV_COM_AUTH_NONCE_PLATFORM
    typedef struct MRH_SRV_OPCODE_COM_AUTH_NONCE_PLATFORM_t
    {
        MRH_Uint64 u64_Nonce; // Nonce to encrypt with password
        
    }MRH_SRV_OPCODE_COM_AUTH_NONCE_PLATFORM;
    
    // MRH_SRV_COM_AUTH_NONCE_APP
    typedef struct MRH_SRV_OPCODE_COM_AUTH_NONCE_APP_t
    {
        char* p_Buffer; // Size given on auth proof
        
    }MRH_SRV_OPCODE_COM_AUTH_NONCE_APP;
    
    // MRH_SRV_COM_AUTH_NONCE_RESULT
    typedef struct MRH_SRV_OPCODE_COM_AUTH_NONCE_RESULT_t
    {
        MRH_Uint32 u32_Error; // Exact error id, 0 is OK
        
    }MRH_SRV_OPCODE_COM_AUTH_NONCE_RESULT;
    
    //
    // Text Messages
    //
    
    // MRH_SRV_MSG_CHUNK
    typedef struct MRH_SRV_OPCODE_MSG_CHUNK_t
    {
        MRH_Uint8 u8_Type; // 0 = Chunk, 1 = End
        MRH_Uint32 u32_ID; // 0 - XX
        MRH_Uint32 u32_Part; // 0 - XX
        char* p_Buffer; // Size given on auth proof
        
    }MRH_SRV_OPCODE_MSG_CHUNK;
    
    //
    // Location
    //
    
    // MRH_SRV_LOCATION
    typedef struct MRH_SRV_OPCODE_LOCATION_t
    {
        MRH_Sfloat64 f64_Latitude;
        MRH_Sfloat64 f64_Longtitude;
        MRH_Sfloat64 f64_Elevation;
        MRH_Sfloat64 f64_Facing;
        
    }MRH_SRV_OPCODE_LOCATION;
    
#ifdef __cplusplus
}
#endif

#endif /* MRH_CommunicationOpCode_h */

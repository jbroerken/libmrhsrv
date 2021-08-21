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

#ifndef MRH_ServerOpCode_h
#define MRH_ServerOpCode_h

// C

// External
#include <MRH_Typedefs.h>

// Project

// Pre-defined
#define MRH_SRV_OPCODE_VERSION 1

#define MRH_SRV_OPCODE_QA_SIZE 32
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
        
        // Unknown (all)
        MRH_SRV_UNK = 0,                        // Unknown data
        
        // Heartbeat (all)
        MRH_SRV_HEARTBEAT = 1,                  // Connection signals active
        
        // Client
        MRH_SRV_C_AUTH_REQUEST = 2,             // Client requests authentication to server
        MRH_SRV_C_AUTH_PROOF = 3,               // Proof for valid client connection for server
        
        MRH_SRV_C_AUTH_NONCE_PLATFORM = 4,      // Send a unencrypted nonce from the platform to the app
        MRH_SRV_C_AUTH_NONCE_APP = 5,           // Return a password encrypted nonce from the app to the platform
        MRH_SRV_C_AUTH_NONCE_RESULT = 6,        // Send the nonce result to the app and server
        
        MRH_SRV_C_LOCATION = 7,                 // Send user location
        MRH_SRV_C_MSG_CHUNK = 8,                // Send message chunk
        
        MRH_SRV_C_DISCONNECT,                   // Client will disconnect
        
        // Server
        MRH_SRV_S_AUTH_CHALLENGE,               // Challenge new connected client to provide valid connection details
        MRH_SRV_S_AUTH_RESULT,                  // Authentification result
        
        MRH_SRV_S_COM_TARGET,                   // The address and port of the communication server to connect to
        
        MRH_SRV_S_AUTH_NONCE_PLATFORM,          // Send a unencrypted nonce from the platform to the app
        MRH_SRV_S_AUTH_NONCE_APP,               // Return a password encrypted nonce from the app to the platform
        MRH_SRV_S_AUTH_NONCE_RESULT,            // Send the nonce result to the app and server
        
        MRH_SRV_S_HEARTBEAT_CHANGE,             // Heartbeat interval change
        
        MRH_SRV_S_LOCATION,                     // Pass on client sent user location
        MRH_SRV_S_MSG_CHUNK,                    // Pass on client sent message chunk
        
        MRH_SRV_S_CONNECTED,                    // Inform other of actor connected
        MRH_SRV_S_DISCONNECTED,                 // Inform others of actor disconnected (includes server actors)
        
        // Bounds
        MRH_SRV_OPCODE_MAX = MRH_SRV_S_DISCONNECTED,
        
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
    // Unknown (all)
    //
    typedef struct MRH_SRV_OPCODE_UNK_t
    {
        MRH_Uint8 u8_Actor;
        
    }MRH_SRV_OPCODE_UNK;
    
    //
    // Heartbeat (all)
    //
    typedef struct MRH_SRV_OPCODE_HEARTBEAT_t
    {
        MRH_Uint32 u32_TimeS; // Max time until the next heartbeat is sent
        
    }MRH_SRV_OPCODE_HEARTBEAT;
    
    //
    // Client
    //
    typedef struct MRH_SRV_OPCODE_C_AUTH_REQUEST_t
    {
        MRH_Uint8 u8_Actor; // Which type of client actor connects
        MRH_Uint8 u8_Target; // Which server to target
        
    }MRH_SRV_OPCODE_C_AUTH_REQUEST;
    
    typedef struct MRH_SRV_OPCODE_C_AUTH_PROOF_t
    {
        MRH_Uint8 u8_Actor;
        MRH_Uint32 u32_OpCodeVer; // Ignore outdated or future
        char p_Answer[MRH_SRV_OPCODE_QA_SIZE]; // Just something to identify correct clients
        char p_DeviceID[MRH_SRV_OPCODE_DEVICE_ID_SIZE]; // Needed to get the right communication server
        MRH_Uint32 u32_MessageSize; // The server message sizes
        
    }MRH_SRV_OPCODE_C_AUTH_PROOF_CON;
    
    typedef struct MRH_SRV_OPCODE_C_AUTH_NONCE_PLATFORM_t
    {
        MRH_Uint64 u64_Nonce; // Nonce to encrypt with password
        
    }MRH_SRV_OPCODE_C_AUTH_NONCE_PLATFORM;
    
    typedef struct MRH_SRV_OPCODE_C_AUTH_NONCE_APP_t
    {
        char* p_Buffer; // Size given on auth proof
        
    }MRH_SRV_OPCODE_C_AUTH_NONCE_APP;
    
    typedef struct MRH_SRV_OPCODE_C_AUTH_NONCE_RESULT_t
    {
        MRH_Uint32 u32_Error; // Exact error id, 0 is OK
        
    }MRH_SRV_OPCODE_C_AUTH_NONCE_RESULT;
    
    typedef struct MRH_SRV_OPCODE_C_LOCATION_t
    {
        MRH_Sfloat64 f64_Latitude;
        MRH_Sfloat64 f64_Longtitude;
        MRH_Sfloat64 f64_Elevation;
        MRH_Sfloat64 f64_Facing;
        
    }MRH_SRV_OPCODE_C_LOCATION;
    
    typedef struct MRH_SRV_OPCODE_C_MSG_CHUNK_t
    {
        MRH_Uint8 u8_Type; // 0 = Chunk, 1 = End
        MRH_Uint32 u32_ID; // 0 - XX
        MRH_Uint32 u32_Part; // 0 - XX
        char* p_Buffer; // Size given on auth proof
        
    }MRH_SRV_OPCODE_C_MSG_CHUNK;
    
    typedef struct MRH_SRV_OPCODE_C_DISCONNECT_t
    {
        MRH_Uint8 u8_Actor; // Which actor disconnected
        
    }MRH_SRV_OPCODE_C_DISCONNECT;
    
    //
    // Server
    //
    typedef struct MRH_SRV_OPCODE_S_AUTH_CHALLENGE_t
    {
        MRH_Uint8 u8_Actor; // Tell server actor to target just so we know its the connection server or coms
        char p_Question[MRH_SRV_OPCODE_QA_SIZE];
        
    }MRH_SRV_OPCODE_S_AUTH_CHALLENGE;
    
    typedef struct MRH_SRV_OPCODE_S_AUTH_RESULT_t
    {
        MRH_Uint8 u8_Actor; // Who sent this auth result
        MRH_Uint32 u32_Error; // Exact error id, 0 is OK
        
    }MRH_SRV_OPCODE_S_AUTH_Result;
    
    typedef struct MRH_SRV_OPCODE_S_COM_TARGET_t
    {
        char p_Address[MRH_SRV_OPCODE_ADDRESS_SIZE];
        MRH_Uint32 u32_Port;
        
    }MRH_SRV_OPCODE_S_COM_TARGET;
    
    typedef struct MRH_SRV_OPCODE_S_HEARTBEAT_CHANGE_t
    {
        MRH_Uint32 u32_TimeS; // New timeframe in which a heartbeat has to be sent
        
    }MRH_SRV_OPCODE_S_HEARTBEAT_CHANGE;
    
    typedef struct MRH_SRV_OPCODE_S_CONNECTED_t
    {
        MRH_Uint8 u8_Actor; // Which actor connected
        
    }MRH_SRV_OPCODE_S_CONNECTED;
    
    // Pass on messages take the same structure!
    typedef MRH_SRV_OPCODE_C_AUTH_NONCE_PLATFORM MRH_SRV_OPCODE_S_AUTH_NONCE_PLATFORM;
    typedef MRH_SRV_OPCODE_C_AUTH_NONCE_APP MRH_SRV_OPCODE_S_AUTH_NONCE_APP;
    typedef MRH_SRV_OPCODE_C_AUTH_NONCE_RESULT MRH_SRV_OPCODE_S_AUTH_NONCE_RESULT;
    
    typedef MRH_SRV_OPCODE_C_LOCATION MRH_SRV_OPCODE_S_LOCATION;
    typedef MRH_SRV_OPCODE_C_MSG_CHUNK MRH_SRV_OPCODE_S_MSG_CHUNK;
    
    typedef MRH_SRV_OPCODE_C_DISCONNECT MRH_SRV_OPCODE_S_DISCONNECTED;
    
#ifdef __cplusplus
}
#endif

#endif /* MRH_ServerOpCode_h */

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

// C
#include <string.h>
#ifdef __APPLE__
    #include <libkern/OSByteOrder.h>
#else
    #include <byteswap.h>
#endif

// External

// Project
#include "./MRH_NetMessageV1.h"

// Pre-defined
#define IS_BIG_ENDIAN (*(uint16_t *)"\0\xff" < 0x100)
#ifdef __APPLE__
    #define bswap_16(x) OSSwapInt16(x)
    #define bswap_32(x) OSSwapInt32(x)
    #define bswap_64(x) OSSwapInt64(x)
#endif


//*************************************************************************************
// Server Auth
//*************************************************************************************

void FROM_MRH_SRV_C_MSG_AUTH_REQUEST(uint8_t* p_Buffer, const MRH_SRV_C_MSG_AUTH_REQUEST_DATA* p_NetMessage)
{
    memset(p_Buffer,
           '\0',
           MRH_SRV_SIZE_MESSAGE_BUFFER);
    
    p_Buffer[0] = MRH_SRV_C_MSG_AUTH_REQUEST;
    ++p_Buffer;
    
    memcpy(p_Buffer,
           &(p_NetMessage->p_Mail[0]),
           MRH_SRV_SIZE_ACCOUNT_MAIL);
    
    p_Buffer[MRH_SRV_SIZE_ACCOUNT_MAIL] = p_NetMessage->u8_Actor;
    p_Buffer[MRH_SRV_SIZE_ACCOUNT_MAIL + 1] = p_NetMessage->u8_Version;
}

void TO_MRH_SRV_S_MSG_AUTH_CHALLENGE(MRH_SRV_S_MSG_AUTH_CHALLENGE_DATA* p_NetMessage, const uint8_t* p_Buffer)
{
    ++p_Buffer;
    
    memcpy(&(p_NetMessage->p_Salt[0]),
           p_Buffer,
           MRH_SRV_SIZE_ACCOUNT_PASSWORD_SALT);
    memcpy(&(p_NetMessage->u32_Nonce),
           &(p_Buffer[MRH_SRV_SIZE_ACCOUNT_PASSWORD_SALT]),
           sizeof(uint32_t));
    
    if (IS_BIG_ENDIAN)
    {
        p_NetMessage->u32_Nonce = bswap_32(p_NetMessage->u32_Nonce);
    }
    
    p_NetMessage->u8_HashType = p_Buffer[MRH_SRV_SIZE_ACCOUNT_PASSWORD_SALT + sizeof(uint32_t)];
}

void FROM_MRH_SRV_C_MSG_AUTH_PROOF(uint8_t* p_Buffer, const MRH_SRV_C_MSG_AUTH_PROOF_DATA* p_NetMessage)
{
    memset(p_Buffer,
           '\0',
           MRH_SRV_SIZE_MESSAGE_BUFFER);
    
    p_Buffer[0] = MRH_SRV_C_MSG_AUTH_PROOF;
    ++p_Buffer;
    
    memcpy(p_Buffer,
           &(p_NetMessage->p_NonceHash[0]),
           MRH_SRV_SIZE_NONCE_HASH);
    memcpy(&(p_Buffer[MRH_SRV_SIZE_NONCE_HASH]),
           &(p_NetMessage->p_DeviceKey[0]),
           MRH_SRV_SIZE_DEVICE_KEY);
}

void TO_MRH_SRV_S_MSG_AUTH_RESULT(MRH_SRV_S_MSG_AUTH_RESULT_DATA* p_NetMessage, const uint8_t* p_Buffer)
{
    ++p_Buffer;
    
    p_NetMessage->u8_Result = p_Buffer[0];
}

//*************************************************************************************
// Device Auth
//*************************************************************************************

void FROM_MRH_SRV_C_MSG_PAIR_CHALLENGE(uint8_t* p_Buffer, const MRH_SRV_C_MSG_PAIR_CHALLENGE_DATA* p_NetMessage)
{
    memset(p_Buffer,
           '\0',
           MRH_SRV_SIZE_MESSAGE_BUFFER);
    
    p_Buffer[0] = MRH_SRV_C_MSG_PAIR_CHALLENGE;
    ++p_Buffer;
    
    if (IS_BIG_ENDIAN)
    {
        uint32_t u32_Nonce = bswap_32(p_NetMessage->u32_Nonce);
        memcpy(p_Buffer,
               &u32_Nonce,
               sizeof(uint32_t));
    }
    else
    {
        memcpy(p_Buffer,
               &(p_NetMessage->u32_Nonce),
               sizeof(uint32_t));
    }
    
    p_Buffer[sizeof(uint32_t)] = p_NetMessage->u8_Actor;
}

void TO_MRH_SRV_C_MSG_PAIR_CHALLENGE(MRH_SRV_C_MSG_PAIR_CHALLENGE_DATA* p_NetMessage, const uint8_t* p_Buffer)
{
    ++p_Buffer;
    
    memcpy(&(p_NetMessage->u32_Nonce),
           p_Buffer,
           sizeof(uint32_t));
    
    p_NetMessage->u8_Actor = p_Buffer[sizeof(uint32_t)];
    
    if (IS_BIG_ENDIAN)
    {
        p_NetMessage->u32_Nonce = bswap_32(p_NetMessage->u32_Nonce);
    }
}

void FROM_MRH_SRV_C_MSG_PAIR_PROOF(uint8_t* p_Buffer, const MRH_SRV_C_MSG_PAIR_PROOF_DATA* p_NetMessage)
{
    memset(p_Buffer,
           '\0',
           MRH_SRV_SIZE_MESSAGE_BUFFER);
    
    p_Buffer[0] = MRH_SRV_C_MSG_PAIR_PROOF;
    ++p_Buffer;
    
    memcpy(&(p_Buffer[1]),
           &(p_NetMessage->p_NonceHash[0]),
           MRH_SRV_SIZE_NONCE_HASH);
    memcpy(&(p_Buffer[1 + MRH_SRV_SIZE_NONCE_HASH]),
           &(p_NetMessage->p_DeviceKey[0]),
           MRH_SRV_SIZE_DEVICE_KEY);
}

void TO_MRH_SRV_C_MSG_PAIR_PROOF(MRH_SRV_C_MSG_PAIR_PROOF_DATA* p_NetMessage, const uint8_t* p_Buffer)
{
    ++p_Buffer;
    
    memcpy(&(p_NetMessage->p_NonceHash[0]),
           &(p_Buffer[1]),
           MRH_SRV_SIZE_NONCE_HASH);
    memcpy(&(p_NetMessage->p_DeviceKey[0]),
           &(p_Buffer[1 + MRH_SRV_SIZE_NONCE_HASH]),
           MRH_SRV_SIZE_DEVICE_KEY);
}

void FROM_MRH_SRV_C_MSG_PAIR_RESULT(uint8_t* p_Buffer, const MRH_SRV_C_MSG_PAIR_RESULT_DATA* p_NetMessage)
{
    memset(p_Buffer,
           '\0',
           MRH_SRV_SIZE_MESSAGE_BUFFER);
    
    p_Buffer[0] = MRH_SRV_C_MSG_PAIR_RESULT;
    ++p_Buffer;
    
    p_Buffer[0] = p_NetMessage->u8_Result;
}

void TO_MRH_SRV_C_MSG_PAIR_RESULT(MRH_SRV_C_MSG_PAIR_RESULT_DATA* p_NetMessage, const uint8_t* p_Buffer)
{
    ++p_Buffer;
    
    p_NetMessage->u8_Result = p_Buffer[0];
}

//*************************************************************************************
// Channel
//*************************************************************************************

void FROM_MRH_SRV_C_MSG_CHANNEL_REQUEST(uint8_t* p_Buffer, const MRH_SRV_C_MSG_CHANNEL_REQUEST_DATA* p_NetMessage)
{
    memset(p_Buffer,
           '\0',
           MRH_SRV_SIZE_MESSAGE_BUFFER);
    
    p_Buffer[0] = MRH_SRV_C_MSG_CHANNEL_REQUEST;
    ++p_Buffer;
    
    memcpy(p_Buffer,
           &(p_NetMessage->p_Channel[0]),
           MRH_SRV_SIZE_SERVER_CHANNEL);
}

void TO_MRH_SRV_S_MSG_CHANNEL_RESPONSE(MRH_SRV_S_MSG_CHANNEL_RESPONSE_DATA* p_NetMessage, const uint8_t* p_Buffer)
{
    ++p_Buffer;
    
    memcpy(&(p_NetMessage->p_Channel[0]),
           p_Buffer,
           MRH_SRV_SIZE_SERVER_CHANNEL);
    memcpy(&(p_NetMessage->p_Address[0]),
           &(p_Buffer[MRH_SRV_SIZE_SERVER_CHANNEL]),
           MRH_SRV_SIZE_SERVER_ADDRESS);
    memcpy(&(p_NetMessage->u32_Port),
           &(p_Buffer[MRH_SRV_SIZE_SERVER_CHANNEL + MRH_SRV_SIZE_SERVER_ADDRESS]),
           sizeof(uint32_t));
    
    if (IS_BIG_ENDIAN)
    {
        p_NetMessage->u32_Port = bswap_32(p_NetMessage->u32_Port);
    }
    
    p_NetMessage->u8_Result = p_Buffer[MRH_SRV_SIZE_SERVER_CHANNEL + MRH_SRV_SIZE_SERVER_ADDRESS + sizeof(uint32_t)];
}

//*************************************************************************************
// Text
//*************************************************************************************

void FROM_MRH_SRV_C_MSG_TEXT(uint8_t* p_Buffer, const MRH_SRV_C_MSG_TEXT_DATA* p_NetMessage)
{
    p_Buffer[0] = MRH_SRV_C_MSG_TEXT;
    ++p_Buffer;
    
    memcpy(p_Buffer,
           &(p_NetMessage->p_String[0]),
           MRH_SRV_SIZE_MESSAGE_BUFFER - 1);
}

void TO_MRH_SRV_C_MSG_TEXT(MRH_SRV_C_MSG_TEXT_DATA* p_NetMessage, const uint8_t* p_Buffer)
{
    ++p_Buffer;
    
    memcpy(&(p_NetMessage->p_String[0]),
           p_Buffer,
           MRH_SRV_SIZE_MESSAGE_BUFFER - 1);
}

//*************************************************************************************
// Location
//*************************************************************************************

static inline float SwapFloatBytes(float f32_Source)
{
    char p_Result[4];
    char* p_Source = (char*)&f32_Source;
    
    p_Result[0] = p_Source[3];
    p_Result[1] = p_Source[2];
    p_Result[2] = p_Source[1];
    p_Result[3] = p_Source[0];
    
    return *((float*)p_Result);
}

void FROM_MRH_SRV_C_MSG_LOCATION(uint8_t* p_Buffer, const MRH_SRV_C_MSG_LOCATION_DATA* p_NetMessage)
{
    memset(p_Buffer,
           '\0',
           MRH_SRV_SIZE_MESSAGE_BUFFER);
    
    p_Buffer[0] = MRH_SRV_C_MSG_LOCATION;
    ++p_Buffer;
    
    if (IS_BIG_ENDIAN)
    {
        float f32_Latitude = SwapFloatBytes(p_NetMessage->f32_Latitude);
        float f32_Longtitude = SwapFloatBytes(p_NetMessage->f32_Longtitude);
        float f32_Elevation = SwapFloatBytes(p_NetMessage->f32_Elevation);
        float f32_Facing = SwapFloatBytes(p_NetMessage->f32_Facing);
        
        memcpy(p_Buffer,
               &f32_Latitude,
               sizeof(float));
        memcpy(&(p_Buffer[sizeof(float)]),
               &f32_Longtitude,
               sizeof(float));
        memcpy(&(p_Buffer[sizeof(float) * 2]),
               &f32_Elevation,
               sizeof(float));
        memcpy(&(p_Buffer[sizeof(float) * 3]),
               &f32_Facing,
               sizeof(float));
    }
    else
    {
        memcpy(p_Buffer,
               &(p_NetMessage->f32_Latitude),
               sizeof(float));
        memcpy(&(p_Buffer[sizeof(float)]),
               &(p_NetMessage->f32_Longtitude),
               sizeof(float));
        memcpy(&(p_Buffer[sizeof(float) * 2]),
               &(p_NetMessage->f32_Elevation),
               sizeof(float));
        memcpy(&(p_Buffer[sizeof(float) * 3]),
               &(p_NetMessage->f32_Facing),
               sizeof(float));
    }
}

void TO_MRH_SRV_C_MSG_LOCATION(MRH_SRV_C_MSG_LOCATION_DATA* p_NetMessage, const uint8_t* p_Buffer)
{
    ++p_Buffer;
    
    memcpy(&(p_NetMessage->f32_Latitude),
           p_Buffer,
           sizeof(float));
    memcpy(&(p_NetMessage->f32_Longtitude),
           &(p_Buffer[sizeof(float)]),
           sizeof(float));
    memcpy(&(p_NetMessage->f32_Elevation),
           &(p_Buffer[sizeof(float) * 2]),
           sizeof(float));
    memcpy(&(p_NetMessage->f32_Facing),
           &(p_Buffer[sizeof(float) * 3]),
           sizeof(float));
    
    if (IS_BIG_ENDIAN)
    {
        p_NetMessage->f32_Latitude = SwapFloatBytes(p_NetMessage->f32_Latitude);
        p_NetMessage->f32_Longtitude = SwapFloatBytes(p_NetMessage->f32_Longtitude);
        p_NetMessage->f32_Elevation = SwapFloatBytes(p_NetMessage->f32_Elevation);
        p_NetMessage->f32_Facing = SwapFloatBytes(p_NetMessage->f32_Facing);
    }
}

//*************************************************************************************
// Custom
//*************************************************************************************

void FROM_MRH_SRV_C_MSG_CUSTOM(uint8_t* p_Buffer, const MRH_SRV_C_MSG_CUSTOM_DATA* p_NetMessage)
{
    p_Buffer[0] = MRH_SRV_C_MSG_CUSTOM;
    ++p_Buffer;
    
    memcpy(p_Buffer,
           &(p_NetMessage->p_Buffer[0]),
           MRH_SRV_SIZE_MESSAGE_BUFFER - 1);
}

void TO_MRH_SRV_C_MSG_CUSTOM(MRH_SRV_C_MSG_CUSTOM_DATA* p_NetMessage, const uint8_t* p_Buffer)
{
    ++p_Buffer;
    
    memcpy(p_NetMessage->p_Buffer,
           p_Buffer,
           MRH_SRV_SIZE_MESSAGE_BUFFER - 1);
}

void TO_MRH_SRV_S_MSG_CUSTOM(MRH_SRV_S_MSG_CUSTOM_DATA* p_NetMessage, const uint8_t* p_Buffer)
{
    ++p_Buffer;
    
    memcpy(&(p_NetMessage->p_Buffer[0]),
           p_Buffer,
           MRH_SRV_SIZE_MESSAGE_BUFFER - 1);
}

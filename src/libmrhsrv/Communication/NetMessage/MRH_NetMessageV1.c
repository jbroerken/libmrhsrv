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

size_t FROM_MRH_SRV_MSG_AUTH_REQUEST(uint8_t* p_Buffer, const MRH_SRV_MSG_AUTH_REQUEST_DATA* p_NetMessage)
{
    memcpy(p_Buffer,
           &(p_NetMessage->p_Mail[0]),
           MRH_SRV_SIZE_ACCOUNT_MAIL);
    
    memcpy(&(p_Buffer[MRH_SRV_SIZE_ACCOUNT_MAIL]),
           &(p_NetMessage->p_DeviceKey[0]),
           MRH_SRV_SIZE_DEVICE_KEY);
    
    p_Buffer[MRH_SRV_SIZE_ACCOUNT_MAIL + MRH_SRV_SIZE_DEVICE_KEY] = p_NetMessage->u8_ClientType;
    p_Buffer[MRH_SRV_SIZE_ACCOUNT_MAIL + MRH_SRV_SIZE_DEVICE_KEY + 1] = p_NetMessage->u8_Version;
    
    return (MRH_SRV_SIZE_ACCOUNT_MAIL + MRH_SRV_SIZE_DEVICE_KEY + 2);
}

void TO_MRH_SRV_MSG_AUTH_CHALLENGE(MRH_SRV_MSG_AUTH_CHALLENGE_DATA* p_NetMessage, const uint8_t* p_Buffer)
{
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

size_t FROM_MRH_SRV_MSG_AUTH_PROOF(uint8_t* p_Buffer, const MRH_SRV_MSG_AUTH_PROOF_DATA* p_NetMessage)
{
    memcpy(p_Buffer,
           &(p_NetMessage->p_NonceHash[0]),
           MRH_SRV_SIZE_NONCE_HASH);
    
    return MRH_SRV_SIZE_NONCE_HASH;
}

void TO_MRH_SRV_MSG_AUTH_RESULT(MRH_SRV_MSG_AUTH_RESULT_DATA* p_NetMessage, const uint8_t* p_Buffer)
{
    p_NetMessage->u8_Result = p_Buffer[0];
}

//*************************************************************************************
// Communication
//*************************************************************************************

size_t FROM_MRH_SRV_MSG_TEXT(uint8_t* p_Buffer, const MRH_SRV_MSG_TEXT_DATA* p_NetMessage)
{
    if (IS_BIG_ENDIAN)
    {
        uint64_t u64_TimestampS = bswap_64(p_NetMessage->u64_TimestampS);
        
        memcpy(p_Buffer,
               &(u64_TimestampS),
               sizeof(uint64_t));
    }
    else
    {
        memcpy(p_Buffer,
               &(p_NetMessage->u64_TimestampS),
               sizeof(uint64_t));
    }
    
    size_t us_StringLen = strnlen(p_NetMessage->p_String, MRH_SRV_SIZE_TEXT_STRING);
    
    memcpy(&(p_Buffer[sizeof(uint64_t)]),
           &(p_NetMessage->p_String[0]),
           us_StringLen);
    
    return (us_StringLen + sizeof(uint64_t));
}

void TO_MRH_SRV_MSG_TEXT(MRH_SRV_MSG_TEXT_DATA* p_NetMessage, const uint8_t* p_Buffer)
{
    size_t us_StringLen = strnlen((const char*)&(p_Buffer[sizeof(uint64_t)]), MRH_SRV_SIZE_TEXT_STRING);
    
    memcpy(&(p_NetMessage->u64_TimestampS),
           p_Buffer,
           sizeof(uint64_t));
    memcpy(&(p_NetMessage->p_String[0]),
           &(p_Buffer[sizeof(uint64_t)]),
           us_StringLen);
    
    if (IS_BIG_ENDIAN)
    {
        p_NetMessage->u64_TimestampS = bswap_64(p_NetMessage->u64_TimestampS);
    }
}

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

size_t FROM_MRH_SRV_MSG_LOCATION(uint8_t* p_Buffer, const MRH_SRV_MSG_LOCATION_DATA* p_NetMessage)
{
    if (IS_BIG_ENDIAN)
    {
        float f32_Latitude = SwapFloatBytes(p_NetMessage->f32_Latitude);
        float f32_Longtitude = SwapFloatBytes(p_NetMessage->f32_Longtitude);
        float f32_Elevation = SwapFloatBytes(p_NetMessage->f32_Elevation);
        float f32_Facing = SwapFloatBytes(p_NetMessage->f32_Facing);
        uint64_t u64_TimestampS = bswap_64(p_NetMessage->u64_TimestampS);
        
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
        memcpy(&(p_Buffer[sizeof(float) * 4]),
               &u64_TimestampS,
               sizeof(uint64_t));
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
        memcpy(&(p_Buffer[sizeof(float) * 4]),
               &(p_NetMessage->u64_TimestampS),
               sizeof(uint64_t));
    }
    
    return ((sizeof(float) * 4) + sizeof(uint64_t));
}

void TO_MRH_SRV_MSG_LOCATION(MRH_SRV_MSG_LOCATION_DATA* p_NetMessage, const uint8_t* p_Buffer)
{
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
    memcpy(&(p_NetMessage->u64_TimestampS),
           &(p_Buffer[MRH_SRV_SIZE_TEXT_STRING]),
           sizeof(uint64_t));
    
    if (IS_BIG_ENDIAN)
    {
        p_NetMessage->f32_Latitude = SwapFloatBytes(p_NetMessage->f32_Latitude);
        p_NetMessage->f32_Longtitude = SwapFloatBytes(p_NetMessage->f32_Longtitude);
        p_NetMessage->f32_Elevation = SwapFloatBytes(p_NetMessage->f32_Elevation);
        p_NetMessage->f32_Facing = SwapFloatBytes(p_NetMessage->f32_Facing);
        p_NetMessage->u64_TimestampS = bswap_64(p_NetMessage->u64_TimestampS);
    }
}

size_t FROM_MRH_SRV_MSG_NOTIFICATION(uint8_t* p_Buffer, const MRH_SRV_MSG_NOTIFICATION_DATA* p_NetMessage)
{
    size_t us_StringLen = strnlen(p_NetMessage->p_String, MRH_SRV_SIZE_NOTIFICATION_STRING);
    
    memcpy(p_Buffer,
           &(p_NetMessage->p_String[0]),
           us_StringLen);
    
    return us_StringLen;
}

size_t FROM_MRH_SRV_MSG_CUSTOM(uint8_t* p_Buffer, const MRH_SRV_MSG_CUSTOM_DATA* p_NetMessage)
{
    memcpy(p_Buffer,
           &(p_NetMessage->p_Buffer[0]),
           MRH_SRV_SIZE_CUSTOM_BUFFER);
    
    return MRH_SRV_SIZE_CUSTOM_BUFFER;
}

void TO_MRH_SRV_MSG_CUSTOM(MRH_SRV_MSG_CUSTOM_DATA* p_NetMessage, const uint8_t* p_Buffer)
{
    memcpy(p_NetMessage->p_Buffer,
           p_Buffer,
           MRH_SRV_SIZE_CUSTOM_BUFFER);
}

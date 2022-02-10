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

#ifndef MRH_NetMessageV1_h
#define MRH_NetMessageV1_h

// C

// External

// Project
#include "../../../../include/libmrhsrv/libmrhsrv/Communication/MRH_NetMessage.h"


//*************************************************************************************
// Server Auth
//*************************************************************************************

/**
 *  Set the message buffer for a given TO_MRH_SRV_MSG_AUTH_REQUEST net message.
 *
 *  \param p_Buffer The buffer to set.
 *  \param p_NetMessage The net message to use.
 *
 *  \return The message buffer size in bytes.
 */

extern size_t FROM_MRH_SRV_MSG_AUTH_REQUEST(uint8_t* p_Buffer, const MRH_SRV_MSG_AUTH_REQUEST_DATA* p_NetMessage);

/**
 *  Set the data for a given TO_MRH_SRV_MSG_AUTH_CHALLENGE net message with a given buffer.
 *
 *  \param p_NetMessage The net message to set.
 *  \param p_Buffer The buffer to use.
 */

extern void TO_MRH_SRV_MSG_AUTH_CHALLENGE(MRH_SRV_MSG_AUTH_CHALLENGE_DATA* p_NetMessage, const uint8_t* p_Buffer);

/**
 *  Set the message buffer for a given TO_MRH_SRV_MSG_AUTH_PROOF net message.
 *
 *  \param p_Buffer The buffer to set.
 *  \param p_NetMessage The net message to use.
 *
 *  \return The message buffer size in bytes.
 */

extern size_t FROM_MRH_SRV_MSG_AUTH_PROOF(uint8_t* p_Buffer, const MRH_SRV_MSG_AUTH_PROOF_DATA* p_NetMessage);

/**
 *  Set the data for a given TO_MRH_SRV_MSG_AUTH_RESULT net message with a given buffer.
 *
 *  \param p_NetMessage The net message to set.
 *  \param p_Buffer The buffer to use.
 */

extern void TO_MRH_SRV_MSG_AUTH_RESULT(MRH_SRV_MSG_AUTH_RESULT_DATA* p_NetMessage, const uint8_t* p_Buffer);

//*************************************************************************************
// Communication
//*************************************************************************************

/**
 *  Set the message buffer for a given MRH_SRV_MSG_TEXT net message.
 *
 *  \param p_Buffer The buffer to set.
 *  \param p_NetMessage The net message to use.
 *
 *  \return The message buffer size in bytes.
 */

extern size_t FROM_MRH_SRV_MSG_TEXT(uint8_t* p_Buffer, const MRH_SRV_MSG_TEXT_DATA* p_NetMessage);

/**
 *  Set the data for a given MRH_SRV_MSG_TEXT net message with a given buffer.
 *
 *  \param p_NetMessage The net message to set.
 *  \param p_Buffer The buffer to use.
 */

extern void TO_MRH_SRV_MSG_TEXT(MRH_SRV_MSG_TEXT_DATA* p_NetMessage, const uint8_t* p_Buffer);

/**
 *  Set the message buffer for a given MRH_SRV_MSG_LOCATION net message.
 *
 *  \param p_Buffer The buffer to set.
 *  \param p_NetMessage The net message to use.
 *
 *  \return The message buffer size in bytes.
 */

extern size_t FROM_MRH_SRV_MSG_LOCATION(uint8_t* p_Buffer, const MRH_SRV_MSG_LOCATION_DATA* p_NetMessage);

/**
 *  Set the message buffer for a given MRH_SRV_MSG_NOTIFICATION net message.
 *
 *  \param p_Buffer The buffer to set.
 *  \param p_NetMessage The net message to use.
 *
 *  \return The message buffer size in bytes.
 */

extern size_t FROM_MRH_SRV_MSG_NOTIFICATION(uint8_t* p_Buffer, const MRH_SRV_MSG_NOTIFICATION_DATA* p_NetMessage);

/**
 *  Set the data for a given MRH_SRV_MSG_LOCATION net message with a given buffer.
 *
 *  \param p_NetMessage The net message to set.
 *  \param p_Buffer The buffer to use.
 */

extern void TO_MRH_SRV_MSG_LOCATION(MRH_SRV_MSG_LOCATION_DATA* p_NetMessage, const uint8_t* p_Buffer);

/**
 *  Set the message buffer for a given MRH_SRV_MSG_CUSTOM net message.
 *
 *  \param p_Buffer The buffer to set.
 *  \param p_NetMessage The net message to use.
 *
 *  \return The message buffer size in bytes.
 */

extern size_t FROM_MRH_SRV_MSG_CUSTOM(uint8_t* p_Buffer, const MRH_SRV_MSG_CUSTOM_DATA* p_NetMessage);

/**
 *  Set the data for a given MRH_SRV_MSG_CUSTOM net message with a given buffer.
 *
 *  \param p_NetMessage The net message to set.
 *  \param p_Buffer The buffer to use.
 */

extern void TO_MRH_SRV_MSG_CUSTOM(MRH_SRV_MSG_CUSTOM_DATA* p_NetMessage, const uint8_t* p_Buffer);


#endif /* MRH_NetMessageV1_h */

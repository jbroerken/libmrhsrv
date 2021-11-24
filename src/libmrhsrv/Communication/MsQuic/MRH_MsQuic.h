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

#ifndef MRH_MsQuic_h
#define MRH_MsQuic_h

// C

// External

// Project
#include "./MRH_MsQuicContext.h"


//*************************************************************************************
// Connection Callback
//*************************************************************************************

/**
 *  MsQuic connection callback.
 *
 *  \param Connection The connection for the callback.
 *  \param Context The provided connection context.
 *  \param Event The recieved connection event.
 *
 *  \return The callback result.
 */

extern
_IRQL_requires_max_(DISPATCH_LEVEL)
_Function_class_(QUIC_CONNECTION_CALLBACK)
QUIC_STATUS QUIC_API MRH_MsQuicConnectionCallback(_In_ HQUIC Connection, _In_opt_ void* Context, _Inout_ QUIC_CONNECTION_EVENT* Event);

//*************************************************************************************
// Stream Callback
//*************************************************************************************

/**
 *  MsQuic stream callback.
 *
 *  \param Stream The stream for the callback.
 *  \param Context The provided stream context.
 *  \param Event The recieved stream event.
 *
 *  \return The callback result.
 */

extern
_IRQL_requires_max_(DISPATCH_LEVEL)
_Function_class_(QUIC_STREAM_CALLBACK)
QUIC_STATUS QUIC_API MRH_MsQuicStreamCallback(_In_ HQUIC Stream, _In_opt_ void* Context, _Inout_ QUIC_STREAM_EVENT* Event);


#endif /* MRH_MsQuic_h */

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

// External

// Project
#include "./MRH_MsQuic.h"


//*************************************************************************************
// Connection Callback
//*************************************************************************************

_IRQL_requires_max_(DISPATCH_LEVEL)
_Function_class_(QUIC_CONNECTION_CALLBACK)
QUIC_STATUS QUIC_API MRH_MsQuicConnectionCallback(_In_ HQUIC Connection, _In_opt_ void* Context, _Inout_ QUIC_CONNECTION_EVENT* Event)
{
    MRH_MsQuicConnection* p_MsQuic = (MRH_MsQuicConnection*)Context;
    
    switch (Event->Type)
    {
        case QUIC_CONNECTION_EVENT_CONNECTED:
        {
            // Clear messages
            for (size_t i = 0; i < MRH_SRV_MESSAGE_BUFFER_COUNT; ++i)
            {
                //p_MsQuicConnection->p_Recieved[i]->i_DataSet = -1;
                p_MsQuic->p_Send[i].i_State = MRH_MSQ_MESSAGE_COMPLETE;
            }
            
            // Set connection
            atomic_init(&(p_MsQuic->p_Connection), Connection);
            break;
        }
            
        case QUIC_CONNECTION_EVENT_SHUTDOWN_COMPLETE:
        {
            if (p_MsQuic->p_Connection != NULL)
            {
                p_MsQuic->p_MsQuicAPI->ConnectionClose(Connection);
                p_MsQuic->p_Connection = NULL;
            }
            break;
        }
            
        case QUIC_CONNECTION_EVENT_PEER_STREAM_STARTED:
        {
            // Find a free message
            MRH_MsQuicMessage* p_Message = NULL;
            
            for (size_t i = 0; i < MRH_SRV_MESSAGE_BUFFER_COUNT; ++i)
            {
                if (p_MsQuic->p_Recieved[i].i_State == MRH_MSQ_MESSAGE_FREE)
                {
                    p_Message = &(p_MsQuic->p_Recieved[i]);
                    p_Message->i_State = MRH_MSQ_MESSAGE_IN_USE;
                    p_Message->us_SizeCur = 0; // Reset to 0, new message
                    break;
                }
            }
            
            if (p_Message != NULL)
            {
                p_MsQuic->p_MsQuicAPI->SetCallbackHandler(Event->PEER_STREAM_STARTED.Stream,
                                                          (void*)MRH_MsQuicStreamCallback,
                                                          p_Message);
            }
            else
            {
                p_MsQuic->p_MsQuicAPI->StreamShutdown(Event->PEER_STREAM_STARTED.Stream,
                                                      QUIC_STREAM_SHUTDOWN_FLAG_ABORT,
                                                      0);
            }
            break;
        }
        
        case QUIC_CONNECTION_EVENT_SHUTDOWN_INITIATED_BY_PEER:
        case QUIC_CONNECTION_EVENT_SHUTDOWN_INITIATED_BY_TRANSPORT:
        {
            if (p_MsQuic->p_Connection != NULL)
            {
                p_MsQuic->p_MsQuicAPI->ConnectionShutdown(Connection,
                                                          QUIC_CONNECTION_SHUTDOWN_FLAG_NONE,
                                                          0);
            }
            break;
        }
            
        case QUIC_CONNECTION_EVENT_RESUMED: { break; }
        default: { break; }
    }
    
    return QUIC_STATUS_SUCCESS;
}

//*************************************************************************************
// Stream Callback
//*************************************************************************************

_IRQL_requires_max_(DISPATCH_LEVEL)
_Function_class_(QUIC_STREAM_CALLBACK)
QUIC_STATUS QUIC_API MRH_MsQuicStreamCallback(_In_ HQUIC Stream, _In_opt_ void* Context, _Inout_ QUIC_STREAM_EVENT* Event)
{
    MRH_MsQuicMessage* p_MsQuic = (MRH_MsQuicMessage*)Context;
    
    switch (Event->Type)
    {
        case QUIC_STREAM_EVENT_SEND_COMPLETE:
        {
            p_MsQuic->us_SizeCur = 0;
            p_MsQuic->i_State = MRH_MSQ_MESSAGE_FREE;
            
            p_MsQuic->p_MsQuicAPI->StreamShutdown(Stream,
                                                  QUIC_STREAM_SHUTDOWN_FLAG_GRACEFUL,
                                                  0);
            break;
        }
            
        case QUIC_STREAM_EVENT_RECEIVE:
        {
            for (uint32_t i = 0; i < Event->RECEIVE.BufferCount; ++i)
            {
                // Do we need to expand?
                size_t us_NextSize = p_MsQuic->us_SizeCur + Event->RECEIVE.Buffers[i].Length;
                
                if (us_NextSize > p_MsQuic->us_SizeMax)
                {
                    if ((p_MsQuic->p_Buffer = (uint8_t*)realloc(p_MsQuic->p_Buffer, us_NextSize)) == NULL)
                    {
                        break;
                    }
                    
                    p_MsQuic->us_SizeMax = us_NextSize;
                }
                
                memcpy(&(p_MsQuic->p_Buffer[p_MsQuic->us_SizeCur]),
                       Event->RECEIVE.Buffers[i].Buffer,
                       Event->RECEIVE.Buffers[i].Length);
                
                p_MsQuic->us_SizeCur = us_NextSize;
            }
            break;
        }
            
        case QUIC_STREAM_EVENT_PEER_SEND_ABORTED:
        {
            p_MsQuic->i_State = MRH_MSQ_MESSAGE_FREE;
            p_MsQuic->p_MsQuicAPI->StreamShutdown(Stream,
                                                  QUIC_STREAM_SHUTDOWN_FLAG_ABORT,
                                                  0);
            break;
        }
            
        case QUIC_STREAM_EVENT_PEER_SEND_SHUTDOWN:
        {
            p_MsQuic->i_State = MRH_MSQ_MESSAGE_COMPLETE;
            p_MsQuic->p_MsQuicAPI->StreamShutdown(Stream,
                                                  QUIC_STREAM_SHUTDOWN_FLAG_GRACEFUL,
                                                  0);
            break;
        }
                
        case QUIC_STREAM_EVENT_SHUTDOWN_COMPLETE:
        {
            p_MsQuic->p_MsQuicAPI->StreamClose(Stream);
            break;
        }
            
        default: { break; }
    }
    
    return QUIC_STATUS_SUCCESS;
}

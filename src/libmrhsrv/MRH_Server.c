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
#include <stdlib.h>
#include <string.h>

// External

// Project
#include "../../include/libmrhsrv/libmrhsrv/Communication/MRH_ServerCommunication.h"
#include "./Error/MRH_ServerErrorInternal.h"
#include "./MRH_ServerTypesInternal.h"

// Pre-defined
#ifndef MRH_SRV_REGISTRATION_NAME
    #define MRH_SRV_REGISTRATION_NAME "mrh_srv_reg"
#endif
#ifndef MRH_SRV_ALPN_NAME
    #define MRH_SRV_ALPN_NAME "mrh_srv_alpn"
#endif


//*************************************************************************************
// Context
//*************************************************************************************

MRH_Srv_Context* MRH_SRV_Init(MRH_Srv_Actor e_Client, int i_MaxServerCount, int i_TimeoutMS)
{
    if (e_Client == MRH_SRV_SERVER_CONNECTION ||
        e_Client == MRH_SRV_SERVER_COMMUNICATION)
    {
        MRH_ERR_SetServerError(MRH_SERVER_ERROR_GENERAL_INVALID_PARAM);
        return NULL;
    }
    
    // Sodium
    if (sodium_init() != 0)
    {
        MRH_ERR_SetServerError(MRH_SERVER_ERROR_ENCRYPTION_INIT);
        return NULL;
    }
    
    //
    //  MsQuic
    //
    
    const QUIC_API_TABLE* p_MsQuicAPI;
    HQUIC p_MsQuicRegistration;
    HQUIC p_MsQuicConfiguration;
    
    QUIC_STATUS ui_Status;
    
    QUIC_REGISTRATION_CONFIG c_RegistrationConfig =
    {
        MRH_SRV_REGISTRATION_NAME, // @TODO: Config
        QUIC_EXECUTION_PROFILE_LOW_LATENCY
    };
    
    QUIC_BUFFER c_Alpn =
    {
        sizeof(MRH_SRV_ALPN_NAME) - 1,
        (uint8_t*)MRH_SRV_ALPN_NAME
    };
    
    QUIC_SETTINGS c_Settings = { 0 };
    QUIC_CREDENTIAL_CONFIG c_CredConfig;
    
    // Setup MsQuic api info
    if (QUIC_FAILED(ui_Status = MsQuicOpen(&p_MsQuicAPI)))
    {
        MRH_ERR_SetServerError(MRH_SERVER_ERROR_MSQUIC_API_TABLE);
        return NULL;
    }
    
    // Now start the registration
    if (QUIC_FAILED(ui_Status = p_MsQuicAPI->RegistrationOpen(&c_RegistrationConfig,
                                                              &p_MsQuicRegistration)))
    {
        MsQuicClose(p_MsQuicAPI);
        MRH_ERR_SetServerError(MRH_SERVER_ERROR_MSQUIC_REGISTRATION);
        return NULL;
    }
    
    // Got registration, setup configuration
    memset(&c_CredConfig, 0, sizeof(c_CredConfig));
    
    c_Settings.PeerUnidiStreamCount = i_MaxServerCount * (MRH_SRV_MESSAGE_BUFFER_COUNT * 2); // Send + Recieve
    c_Settings.IsSet.PeerUnidiStreamCount = TRUE;
    //c_Settings.PeerBidiStreamCount = 1024;
    //c_Settings.IsSet.PeerBidiStreamCount = TRUE;
    c_Settings.IdleTimeoutMs = i_TimeoutMS;
    c_Settings.IsSet.IdleTimeoutMs = TRUE;

    c_CredConfig.Type = QUIC_CREDENTIAL_TYPE_NONE;
    c_CredConfig.Flags = QUIC_CREDENTIAL_FLAG_CLIENT;
    c_CredConfig.Flags |= QUIC_CREDENTIAL_FLAG_NO_CERTIFICATE_VALIDATION;

    if (QUIC_FAILED(ui_Status = p_MsQuicAPI->ConfigurationOpen(p_MsQuicRegistration,
                                                               &c_Alpn,
                                                               1,
                                                               &c_Settings,
                                                               sizeof(c_Settings),
                                                               NULL,
                                                               &p_MsQuicConfiguration)))
    {
        p_MsQuicAPI->RegistrationClose(p_MsQuicRegistration);
        MsQuicClose(p_MsQuicAPI);
        MRH_ERR_SetServerError(MRH_SERVER_ERROR_MSQUIC_CONFIGURATION);
        return NULL;
    }
    else if (QUIC_FAILED(ui_Status = p_MsQuicAPI->ConfigurationLoadCredential(p_MsQuicConfiguration,
                                                                              &c_CredConfig)))
    {
        p_MsQuicAPI->ConfigurationClose(p_MsQuicConfiguration);
        p_MsQuicAPI->RegistrationClose(p_MsQuicRegistration);
        MsQuicClose(p_MsQuicAPI);
        MRH_ERR_SetServerError(MRH_SERVER_ERROR_MSQUIC_CONFIGURATION);
        return NULL;
    }
    
    //
    //  Connection
    //
    
    // First, build the connection info
    MRH_Srv_Context* p_Context = (struct MRH_Srv_Context_t*)malloc(sizeof(struct MRH_Srv_Context_t));
    
    if (p_Context == NULL)
    {
        p_MsQuicAPI->ConfigurationClose(p_MsQuicConfiguration);
        p_MsQuicAPI->RegistrationClose(p_MsQuicRegistration);
        MsQuicClose(p_MsQuicAPI);
        MRH_ERR_SetServerError(MRH_SERVER_ERROR_GENERAL_MALLOC);
        return NULL;
    }
    
    // Set MsQuic
    p_Context->p_MsQuicAPI = p_MsQuicAPI;
    p_Context->p_MsQuicRegistration = p_MsQuicRegistration;
    p_Context->p_MsQuicConfiguration = p_MsQuicConfiguration;
    
    // Set connection info
    p_Context->i_ServerMax = i_MaxServerCount;
    p_Context->i_ServerCur = 0;
    p_Context->u8_DeviceType = (MRH_Uint8)e_Client;
    p_Context->i_TimeoutMS = i_TimeoutMS;
    
    // All done, now usable!
    return p_Context;
}

MRH_Srv_Context* MRH_SRV_Destroy(MRH_Srv_Context* p_Context)
{
    if (p_Context == NULL)
    {
        MRH_ERR_SetServerError(MRH_SERVER_ERROR_GENERAL_INVALID_PARAM);
        return NULL;
    }
    
    if (p_Context->p_MsQuicAPI != NULL)
    {
        if (p_Context->p_MsQuicConfiguration != NULL)
        {
            p_Context->p_MsQuicAPI->ConfigurationClose(p_Context->p_MsQuicConfiguration);
        }
        
        if (p_Context->p_MsQuicRegistration != NULL)
        {
            p_Context->p_MsQuicAPI->RegistrationClose(p_Context->p_MsQuicRegistration);
        }
        
        MsQuicClose(p_Context->p_MsQuicAPI);
    }
    
    free(p_Context);
    
    return NULL;
}

//*************************************************************************************
// Server
//*************************************************************************************

MRH_Srv_Server* MRH_SRV_CreateServer(MRH_Srv_Context* p_Context, const char* p_Channel)
{
    if (p_Context == NULL ||
        p_Channel == NULL || strlen(p_Channel) == 0 || strlen(p_Channel) > MRH_SRV_SIZE_SERVER_CHANNEL ||
        p_Context->i_ServerCur == p_Context->i_ServerMax)
    {
        MRH_ERR_SetServerError(MRH_SERVER_ERROR_GENERAL_INVALID_PARAM);
        return NULL;
    }
    
    MRH_Srv_Server* p_Server = (MRH_Srv_Server*)malloc(sizeof(MRH_Srv_Server));
    
    if (p_Server == NULL)
    {
        MRH_ERR_SetServerError(MRH_SERVER_ERROR_GENERAL_MALLOC);
        return NULL;
    }
    else if ((p_Server->p_MsQuic = MRH_MsQuicCreateConnection(p_Context->p_MsQuicAPI)) == NULL)
    {
        free(p_Server);
        MRH_ERR_SetServerError(MRH_SERVER_ERROR_GENERAL_MALLOC);
        return NULL;
    }
    
    memset(p_Server->p_Channel, '\0', MRH_SRV_SIZE_SERVER_CHANNEL);
    memset(p_Server->p_Address, '\0', MRH_SRV_SIZE_SERVER_ADDRESS);
    strncpy(p_Server->p_Channel, p_Channel, strlen(p_Channel));
    
    p_Server->i_Port = MRH_SRV_PORT_INVALID;
    p_Server->u8_DeviceType = p_Context->u8_DeviceType;
    p_Server->i_TimeoutMS = p_Context->i_TimeoutMS;
    
    p_Context->i_ServerCur += 1;
    
    return p_Server;
}

MRH_Srv_Server* MRH_SRV_DestroyServer(MRH_Srv_Context* p_Context, MRH_Srv_Server* p_Server)
{
    if (p_Context == NULL || p_Server == NULL)
    {
        MRH_ERR_SetServerError(MRH_SERVER_ERROR_GENERAL_INVALID_PARAM);
        return NULL;
    }
    
    // Disconnect first
    MRH_SRV_Disconnect(p_Server);
    
    // Clean up
    MRH_MsQuicDestroyConnection(p_Server->p_MsQuic);
    free(p_Server);
    
    // Reduce server count
    if (p_Context->i_ServerCur > 0)
    {
        p_Context->i_ServerCur -= 1;
    }
    
    return NULL;
}

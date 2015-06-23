
#include "stdafx.h"

extern HTTP_MODULE_ID g_pModuleContext;
extern IHttpServer *g_pHttpServer;

const CModuleConfiguration *iis_lua_get_config(IHttpContext *pHttpContext)
{
    auto pModuleContextContainer = pHttpContext->GetMetadata()->GetModuleContextContainer();
    auto pModuleConfig = reinterpret_cast<CModuleConfiguration *>(pModuleContextContainer->GetModuleContext(g_pModuleContext));

    if (pModuleConfig != NULL)
    {
        return pModuleConfig;
    }

    pModuleConfig = new CModuleConfiguration();

    if (FAILED(pModuleConfig->Initialize(pHttpContext, g_pHttpServer)))
    {
        pModuleConfig->CleanupStoredContext();

        return NULL;
    }

    if (FAILED(pModuleContextContainer->SetModuleContext(pModuleConfig, g_pModuleContext)))
    {
        pModuleConfig->CleanupStoredContext();

        return NULL;
    }

    return pModuleConfig;
}

const PCSTR iis_lua_util_get_status_reason(USHORT status)
{
    switch (status)
    {
    case 200:
        return "OK";
    case 201:
        return "Created";
    case 204:
        return "No Content";
    case 301:
        return "Moved Permanently";
    case 302:
        return "Found";
    case 303:
        return "See Other";
    case 304:
        return "Not Modified";
    case 400:
        return "Bad Request";
    case 401:
        return "Unauthorized";
    case 403:
        return "Forbidden";
    case 404:
        return "Not Found";
    case 405:
        return "Method Not Allowed";
    case 500:
        return "Internal Server Error";
    case 501:
        return "Not Implemented";
    case 502:
        return "Bad Gateway";
    case 503:
        return "Service Unavailable";
    default:
        return "Unknown Reason";
    }
}

#include "stdafx.h"

extern HTTP_MODULE_ID g_pModuleContext;
extern IHttpServer *g_pHttpServer;

CLuaHttpStoredContext *iis_lua_get_stored_context(IHttpContext *pHttpContext)
{
    auto pModuleContextContainer = pHttpContext->GetModuleContextContainer();
    auto pHttpStoredContext = reinterpret_cast<CLuaHttpStoredContext *>(pModuleContextContainer->GetModuleContext(g_pModuleContext));

    if (pHttpStoredContext != NULL)
    {
        return pHttpStoredContext;
    }

    pHttpStoredContext = new CLuaHttpStoredContext();

    if (FAILED(pModuleContextContainer->SetModuleContext(pHttpStoredContext, g_pModuleContext)))
    {
        pHttpStoredContext->CleanupStoredContext();

        return NULL;
    }

    return pHttpStoredContext;
}

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

PCSTR iis_lua_util_get_status_reason(USHORT status)
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

PSTR iis_lua_wstr_to_str(PCWSTR wstr)
{
    auto len = wcslen(wstr);

    size_t i;
    PSTR str = new CHAR[len * 2 + 1];

    wcstombs_s(&i, str, len * 2 + 1, wstr, len);

    return str;
}

PWSTR iis_lua_str_to_wstr(PCSTR str)
{
    auto len = strlen(str);

    size_t i;
    auto wstr = new WCHAR[len + 1];

    mbstowcs_s(&i, wstr, len + 1, str, len);

    return wstr;
}
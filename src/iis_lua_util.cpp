
#include "stdafx.h"

extern HTTP_MODULE_ID g_pModuleContext;
extern IHttpServer *g_pHttpServer;

static PCSTR iis_lua_ctx_key = "__iis_lua_ctx";
static PCSTR iis_lua_result_key = "__iis_lua_result";

static const luaL_Reg iis [] =
{
    { "debug", iis_lua_debug },
    { "exec", iis_lua_exec },
    { "exit", iis_lua_exit },
    { "flush", iis_lua_flush },
    { "headers_sent", iis_lua_headers_sent },
    { "map_path", iis_lua_map_path },
    { "print", iis_lua_print },
    { "redirect", iis_lua_redirect },
    { "say", iis_lua_say },
    { NULL, NULL }
};

static const luaL_Reg iis_req [] =
{
    { "get_headers", iis_lua_req_get_headers },
    { "get_method", iis_lua_req_get_method },
    { "get_remote_addr", iis_lua_req_get_remote_addr },
    { "get_url", iis_lua_req_get_url },
    { "get_url_args", iis_lua_req_get_url_args },
    { "http_version", iis_lua_req_http_version },
    { "set_header", iis_lua_req_set_header },
    { "set_method", iis_lua_req_set_method },
    { "set_url", iis_lua_req_set_url },
    { NULL, NULL }
};

static const luaL_Reg iis_resp [] =
{
    { "clear", iis_lua_resp_clear },
    { "clear_headers", iis_lua_resp_clear_headers },
    { "get_headers", iis_lua_resp_get_headers },
    { "get_status", iis_lua_resp_get_status },
    { "set_header", iis_lua_resp_set_header },
    { "set_status", iis_lua_resp_set_status },
    { NULL, NULL }
};

static const luaL_Reg iis_srv [] =
{
    { "get_variable", iis_lua_srv_get_variable },
    { "set_variable", iis_lua_srv_set_variable },
    { NULL, NULL }
};

lua_State *iis_lua_newstate(IHttpContext *ctx)
{
    auto L = luaL_newstate();

    luaL_openlibs(L);

    luaL_newlib(L, iis);

    // create iis.req
    luaL_newlib(L, iis_req);
    lua_setfield(L, -2, "req");

    // create iis.resp
    luaL_newlib(L, iis_resp);
    lua_setfield(L, -2, "resp");

    // create iis.srv
    luaL_newlib(L, iis_srv);
    lua_setfield(L, -2, "srv");

    // register iis
    lua_setglobal(L, "iis");

    // initialize context
    iis_lua_set_http_ctx(L, ctx);
    iis_lua_set_result(L, RQ_NOTIFICATION_CONTINUE);

    return L;
}

REQUEST_NOTIFICATION_STATUS iis_lua_close(lua_State *L)
{
    auto result = iis_lua_get_result(L);

    lua_close(L);

    return result;
}

IHttpContext *iis_lua_get_http_ctx(lua_State *L)
{
    lua_getglobal(L, iis_lua_ctx_key);

    auto ctx = reinterpret_cast<IHttpContext *>(lua_touserdata(L, -1));

    lua_pop(L, 1);

    return ctx;
}

void iis_lua_set_http_ctx(lua_State *L, IHttpContext *ctx)
{
    lua_pushlightuserdata(L, ctx);
    lua_setglobal(L, iis_lua_ctx_key);
}

REQUEST_NOTIFICATION_STATUS iis_lua_get_result(lua_State *L)
{
    lua_getglobal(L, iis_lua_result_key);

    auto result = static_cast<REQUEST_NOTIFICATION_STATUS>(lua_tointeger(L, -1));

    lua_pop(L, 1);

    return result;
}

void iis_lua_set_result(lua_State *L, REQUEST_NOTIFICATION_STATUS result)
{
    lua_pushinteger(L, result);
    lua_setglobal(L, iis_lua_result_key);
}

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
    case 100:
        return "Continue";
    case 101:
        return "Switching Protocols";
    case 200:
        return "OK";
    case 201:
        return "Created";
    case 202:
        return "Accepted";
    case 204:
        return "No Content";
    case 205:
        return "Reset Content";
    case 206:
        return "Partial Content";
    case 301:
        return "Moved Permanently";
    case 302:
        return "Found";
    case 303:
        return "See Other";
    case 304:
        return "Not Modified";
    case 307:
        return "Temporary Redirect";
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
    case 406:
        return "Not Acceptable";
    case 407:
        return "Proxy Authentication Required";
    case 408:
        return "Request Timeout";
    case 409:
        return "Conflict";
    case 410:
        return "Gone";
    case 415:
        return "Unsupported Media Type";
    case 500:
        return "Internal Server Error";
    case 501:
        return "Not Implemented";
    case 502:
        return "Bad Gateway";
    case 503:
        return "Service Unavailable";
    case 504:
        return "Gateway Timeout";
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
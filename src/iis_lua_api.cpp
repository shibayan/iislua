
#include "stdafx.h"


#define iis_lua_ctx_key "__iis_lua_ctx"
#define iis_lua_handled_key "__iis_lua_handled"

IIS_LUA_API inline IHttpContext *iis_lua_get_http_ctx(lua_State *L)
{
    lua_getglobal(L, iis_lua_ctx_key);

    auto *ctx = reinterpret_cast<IHttpContext *>(lua_touserdata(L, -1));

    lua_pop(L, 1);

    return ctx;
}

IIS_LUA_API inline void iis_lua_set_http_ctx(lua_State *L, IHttpContext *ctx)
{
    lua_pushlightuserdata(L, ctx);
    lua_setglobal(L, iis_lua_ctx_key);
}

IIS_LUA_API inline BOOL iis_lua_get_handled(lua_State *L)
{
    lua_getglobal(L, iis_lua_handled_key);

    auto handled = lua_toboolean(L, -1);

    lua_pop(L, 1);

    return handled;
}

IIS_LUA_API inline void iis_lua_set_handled(lua_State *L, BOOL handled = TRUE)
{
    lua_pushboolean(L, handled);
    lua_setglobal(L, iis_lua_handled_key);
}

IIS_LUA_API int iis_lua_debug(lua_State *L)
{
    auto message = luaL_checkstring(L, 1);

    OutputDebugStringA(message);
    OutputDebugStringA("\n");

    return 0;
}

IIS_LUA_API int iis_lua_exit(lua_State *L)
{
    auto *ctx = iis_lua_get_http_ctx(L);

    int status = luaL_checkinteger(L, 1);

    PCSTR reason;

    switch (status)
    {
    case 200:
        reason = "OK";
        break;
    case 204:
        reason = "Created";
        break;
    case 400:
        reason = "Bad Request";
        break;
    case 500:
        reason = "Internal Server Error";
        break;
    default:
        return 0;
    }

    ctx->GetResponse()->SetStatus(status, reason);
    
    iis_lua_set_handled(L);

    return 0;
}

IIS_LUA_API int iis_lua_headers_sent(lua_State *L)
{
    auto *ctx = iis_lua_get_http_ctx(L);

    lua_pushboolean(L, ctx->GetResponseHeadersSent());

    return 1;
}

IIS_LUA_API int iis_lua_map_path(lua_State *L)
{
    auto *ctx = iis_lua_get_http_ctx(L);

    auto url = luaL_checkstring(L, 1);

    //ctx->MapPath(url, NULL, NULL);

    lua_pushstring(L, "");

    return 1;
}

IIS_LUA_API int iis_lua_print(lua_State *L)
{
    auto *ctx = iis_lua_get_http_ctx(L);

    auto message = luaL_checkstring(L, 1);

    return 0;
}

IIS_LUA_API int iis_lua_redirect(lua_State *L)
{
    auto *ctx = iis_lua_get_http_ctx(L);

    auto url = luaL_checkstring(L, 1);

    ctx->GetResponse()->Redirect(url);

    iis_lua_set_handled(L);

    return 0;
}

IIS_LUA_API int iis_lua_req_get_headers(lua_State *L)
{
    auto *ctx = iis_lua_get_http_ctx(L);

    auto headers = ctx->GetRequest()->GetRawHttpRequest()->Headers;

    lua_createtable(L, 0, headers.UnknownHeaderCount);

    // Append Known Headers
    for (size_t i = 0; i < HttpHeaderRequestMaximum; i++)
    {
        if (headers.KnownHeaders[i].pRawValue != NULL)
        {
            lua_pushstring(L, http_header_id_to_name[i]);
            lua_pushlstring(L, headers.KnownHeaders[i].pRawValue, headers.KnownHeaders[i].RawValueLength);
            lua_settable(L, -3);
        }
    }

    // Append Unknown Headers
    for (size_t i = 0; i < headers.UnknownHeaderCount; i++)
    {
        lua_pushlstring(L, headers.pUnknownHeaders[i].pName, headers.pUnknownHeaders[i].NameLength);
        lua_pushlstring(L, headers.pUnknownHeaders[i].pRawValue, headers.pUnknownHeaders[i].RawValueLength);
        lua_settable(L, -3);
    }

    return 1;
}

IIS_LUA_API int iis_lua_req_get_method(lua_State *L)
{
    auto *ctx = iis_lua_get_http_ctx(L);

    lua_pushstring(L, ctx->GetRequest()->GetHttpMethod());

    return 1;
}

IIS_LUA_API int iis_lua_req_http_version(lua_State *L)
{
    auto *ctx = iis_lua_get_http_ctx(L);

    USHORT major, minor;

    ctx->GetRequest()->GetHttpVersion(&major, &minor);

    char version[4];

    sprintf_s(version, "%d.%d", major, minor);

    lua_pushstring(L, version);

    return 1;
}

IIS_LUA_API int iis_lua_req_set_method(lua_State *L)
{
    auto *ctx = iis_lua_get_http_ctx(L);

    auto method = luaL_checkstring(L, 1);

    ctx->GetRequest()->SetHttpMethod(method);

    return 0;
}

IIS_LUA_API int iis_lua_req_set_url(lua_State *L)
{
    auto *ctx = iis_lua_get_http_ctx(L);

    auto url = luaL_checkstring(L, 1);

    ctx->GetRequest()->SetUrl(url, strlen(url), TRUE);

    return 0;
}

IIS_LUA_API int iis_lua_resp_get_headers(lua_State *L)
{
    auto *ctx = iis_lua_get_http_ctx(L);

    auto headers = ctx->GetResponse()->GetRawHttpResponse()->Headers;

    lua_createtable(L, 0, headers.UnknownHeaderCount);

    // Append Known Headers
    for (size_t i = 0; i < HttpHeaderRequestMaximum; i++)
    {
        if (headers.KnownHeaders[i].pRawValue != NULL)
        {
            // TODO: HTTP_HEADER_ID to HeaderName
            char szTemp[5];
            sprintf_s(szTemp, "H_%d", i);

            lua_pushstring(L, szTemp);
            lua_pushlstring(L, headers.KnownHeaders[i].pRawValue, headers.KnownHeaders[i].RawValueLength);
            lua_settable(L, -3);
        }
    }

    // Append Unknown Headers
    for (size_t i = 0; i < headers.UnknownHeaderCount; i++)
    {
        lua_pushlstring(L, headers.pUnknownHeaders[i].pName, headers.pUnknownHeaders[i].NameLength);
        lua_pushlstring(L, headers.pUnknownHeaders[i].pRawValue, headers.pUnknownHeaders[i].RawValueLength);
        lua_settable(L, -3);
    }

    return 0;
}

IIS_LUA_API int iis_lua_resp_set_status(lua_State *L)
{
    auto *ctx = iis_lua_get_http_ctx(L);

    int status = luaL_checkinteger(L, 1);

    ctx->GetResponse()->SetStatus(status, "OK");

    return 0;
}

IIS_LUA_API int iis_lua_server_get_variables(lua_State *L)
{
    auto *ctx = iis_lua_get_http_ctx(L);

    auto name = luaL_checkstring(L, 1);

    PCSTR value;
    DWORD length;

    ctx->GetServerVariable(name, &value, &length);

    lua_pushlstring(L, value, length);

    return 1;
}
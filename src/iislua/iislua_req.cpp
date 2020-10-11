
#include "stdafx.h"

int iislua_req_get_headers(lua_State* L)
{
    auto ctx = iislua_get_http_ctx(L);

    if (ctx == NULL)
    {
        return luaL_error(L, "context is null");
    }

    auto headers = ctx->GetRequest()->GetRawHttpRequest()->Headers;

    lua_createtable(L, 0, headers.UnknownHeaderCount);

    for (USHORT i = 0; i < HttpHeaderRequestMaximum; i++)
    {
        if (headers.KnownHeaders[i].pRawValue != NULL)
        {
            lua_pushstring(L, iislua_util_get_http_req_header(i));
            lua_pushlstring(L, headers.KnownHeaders[i].pRawValue, headers.KnownHeaders[i].RawValueLength);
            lua_settable(L, -3);
        }
    }

    for (USHORT i = 0; i < headers.UnknownHeaderCount; i++)
    {
        lua_pushlstring(L, headers.pUnknownHeaders[i].pName, headers.pUnknownHeaders[i].NameLength);
        lua_pushlstring(L, headers.pUnknownHeaders[i].pRawValue, headers.pUnknownHeaders[i].RawValueLength);
        lua_settable(L, -3);
    }

    return 1;
}

int iislua_req_get_method(lua_State* L)
{
    auto ctx = iislua_get_http_ctx(L);

    if (ctx == NULL)
    {
        return luaL_error(L, "context is null");
    }

    lua_pushstring(L, ctx->GetRequest()->GetHttpMethod());

    return 1;
}

int iislua_req_get_post_args(lua_State* L)
{
    auto ctx = iislua_get_http_ctx(L);

    if (ctx == NULL)
    {
        return luaL_error(L, "context is null");
    }

    lua_createtable(L, 0, 0);

    return 1;
}

int iislua_req_get_remote_addr(lua_State* L)
{
    auto ctx = iislua_get_http_ctx(L);

    if (ctx == NULL)
    {
        return luaL_error(L, "context is null");
    }

    auto remoteAddr = ctx->GetRequest()->GetRemoteAddress();

    char ipAddress[INET6_ADDRSTRLEN];

    if (remoteAddr->sa_family == AF_INET)
    {
        inet_ntop(AF_INET, &reinterpret_cast<SOCKADDR_IN*>(remoteAddr)->sin_addr, ipAddress, sizeof(ipAddress));
    }
    else
    {
        inet_ntop(AF_INET6, &reinterpret_cast<SOCKADDR_IN6*>(remoteAddr)->sin6_addr, ipAddress, sizeof(ipAddress));
    }

    lua_pushstring(L, ipAddress);

    return 1;
}

int iislua_req_get_url(lua_State* L)
{
    auto ctx = iislua_get_http_ctx(L);

    if (ctx == NULL)
    {
        return luaL_error(L, "context is null");
    }

    auto url = iislua_to_str(ctx->GetRequest()->GetRawHttpRequest()->CookedUrl.pAbsPath);

    lua_pushstring(L, url.c_str());

    return 1;
}

int iislua_req_get_url_args(lua_State* L)
{
    auto ctx = iislua_get_http_ctx(L);

    if (ctx == NULL)
    {
        return luaL_error(L, "context is null");
    }

    lua_createtable(L, 0, 0);

    if (ctx->GetRequest()->GetRawHttpRequest()->CookedUrl.pQueryString != NULL)
    {
        auto queryString = iislua_to_str(ctx->GetRequest()->GetRawHttpRequest()->CookedUrl.pQueryString);
    }

    return 1;
}

int iislua_req_http_version(lua_State* L)
{
    auto ctx = iislua_get_http_ctx(L);

    if (ctx == NULL)
    {
        return luaL_error(L, "context is null");
    }

    USHORT major, minor;
    char version[4];

    ctx->GetRequest()->GetHttpVersion(&major, &minor);

    sprintf_s(version, "%d.%d", major, minor);

    lua_pushstring(L, version);

    return 1;
}

int iislua_req_set_header(lua_State* L)
{
    CHECK_ARGUMENT(L, 2);

    auto ctx = iislua_get_http_ctx(L);

    if (ctx == NULL)
    {
        return luaL_error(L, "context is null");
    }

    auto name = luaL_checkstring(L, 1);
    auto value = luaL_checkstring(L, 2);

    ctx->GetRequest()->SetHeader(name, value, static_cast<USHORT>(strlen(value)), TRUE);

    return 0;
}

int iislua_req_set_method(lua_State* L)
{
    CHECK_ARGUMENT(L, 1);

    auto ctx = iislua_get_http_ctx(L);

    if (ctx == NULL)
    {
        return luaL_error(L, "context is null");
    }

    auto method = luaL_checkstring(L, 1);

    ctx->GetRequest()->SetHttpMethod(method);

    return 0;
}

int iislua_req_set_url(lua_State* L)
{
    CHECK_ARGUMENT(L, 1);

    auto ctx = iislua_get_http_ctx(L);

    if (ctx == NULL)
    {
        return luaL_error(L, "context is null");
    }

    auto url = luaL_checkstring(L, 1);

    ctx->GetRequest()->SetUrl(url, static_cast<DWORD>(strlen(url)), TRUE);

    return 0;
}

#include "stdafx.h"

int iislua_resp_clear(lua_State* L)
{
    auto ctx = iislua_get_http_ctx(L);

    if (ctx == NULL)
    {
        return luaL_error(L, "context is null");
    }

    ctx->GetResponse()->Clear();

    return 0;
}

int iislua_resp_clear_headers(lua_State* L)
{
    auto ctx = iislua_get_http_ctx(L);

    if (ctx == NULL)
    {
        return luaL_error(L, "context is null");
    }

    ctx->GetResponse()->ClearHeaders();

    return 0;
}

int iislua_resp_get_headers(lua_State* L)
{
    auto ctx = iislua_get_http_ctx(L);

    if (ctx == NULL)
    {
        return luaL_error(L, "context is null");
    }

    auto headers = ctx->GetResponse()->GetRawHttpResponse()->Headers;

    lua_createtable(L, 0, headers.UnknownHeaderCount);

    for (USHORT i = 0; i < HttpHeaderResponseMaximum; i++)
    {
        if (headers.KnownHeaders[i].pRawValue != NULL)
        {
            lua_pushstring(L, iislua_util_get_http_resp_header(i));
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

int iislua_resp_get_status(lua_State* L)
{
    auto ctx = iislua_get_http_ctx(L);

    if (ctx == NULL)
    {
        return luaL_error(L, "context is null");
    }

    USHORT status;
    ctx->GetResponse()->GetStatus(&status);

    lua_pushinteger(L, status);

    return 1;
}

int iislua_resp_set_header(lua_State* L)
{
    CHECK_ARGUMENT(L, 2);

    auto ctx = iislua_get_http_ctx(L);

    if (ctx == NULL)
    {
        return luaL_error(L, "context is null");
    }

    auto name = luaL_checkstring(L, 1);
    auto value = luaL_checkstring(L, 2);

    ctx->GetResponse()->SetHeader(name, value, static_cast<USHORT>(strlen(value)), TRUE);

    return 0;
}

int iislua_resp_set_status(lua_State* L)
{
    CHECK_ARGUMENT(L, 1);

    auto ctx = iislua_get_http_ctx(L);

    if (ctx == NULL)
    {
        return luaL_error(L, "context is null");
    }

    auto status = static_cast<USHORT>(luaL_checkinteger(L, 1));

    ctx->GetResponse()->SetStatus(status, iislua_util_get_status_reason(status));

    return 0;
}
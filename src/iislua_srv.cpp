
#include "stdafx.h"

int iislua_srv_get_variable(lua_State *L)
{
    CHECK_ARGUMENT(L, 1);

    auto ctx = iislua_get_http_ctx(L);

    if (ctx == NULL)
    {
        return luaL_error(L, "context is null");
    }

    auto name = luaL_checkstring(L, 1);

    PCSTR value;
    DWORD length;

    ctx->GetServerVariable(name, &value, &length);

    lua_pushlstring(L, value, length);

    return 1;
}

int iislua_srv_set_variable(lua_State *L)
{
    CHECK_ARGUMENT(L, 2);

    auto ctx = iislua_get_http_ctx(L);

    if (ctx == NULL)
    {
        return luaL_error(L, "context is null");
    }

    auto name = luaL_checkstring(L, 1);
    auto value = iislua_to_wstr(luaL_checkstring(L, 2));

    ctx->SetServerVariable(name, value.c_str());

    return 0;
}

#include "stdafx.h"

int iislua_user_get_name(lua_State* L)
{
    auto ctx = iislua_get_http_ctx(L);

    if (ctx == NULL)
    {
        return luaL_error(L, "context is null");
    }

    auto name = iislua_to_str(ctx->GetUser()->GetUserName());

    lua_pushstring(L, name.c_str());

    return 1;
}

int iislua_user_get_type(lua_State* L)
{
    auto ctx = iislua_get_http_ctx(L);

    if (ctx == NULL)
    {
        return luaL_error(L, "context is null");
    }

    auto name = iislua_to_str(ctx->GetUser()->GetAuthenticationType());

    lua_pushstring(L, name.c_str());

    return 1;
}
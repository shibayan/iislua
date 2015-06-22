
#include "stdafx.h"


IIS_LUA_API inline IHttpContext *iis_get_http_ctx(lua_State *L)
{
    lua_getglobal(L, iis_http_ctx_key);

    auto *ctx = reinterpret_cast<IHttpContext *>(lua_touserdata(L, -1));

    lua_pop(L, 1);

    return ctx;
}

IIS_LUA_API inline void iis_set_http_ctx(lua_State *L, IHttpContext *ctx)
{
    lua_pushlightuserdata(L, ctx);
    lua_setglobal(L, iis_http_ctx_key);
}

IIS_LUA_API int iis_http_exit(lua_State *L)
{
    auto *ctx = iis_get_http_ctx(L);

    int status = luaL_checkinteger(L, 1);

    ctx->GetResponse()->SetStatus(status, "OK");
    ctx->SetRequestHandled();

    return 0;
}

IIS_LUA_API int iis_http_headers_sent(lua_State *L)
{
    auto *ctx = iis_get_http_ctx(L);

    lua_pushboolean(L, ctx->GetResponseHeadersSent());

    return 1;
}

IIS_LUA_API int iis_http_map_path(lua_State *L)
{
    auto *ctx = iis_get_http_ctx(L);

    auto url = luaL_checkstring(L, 1);

    //ctx->MapPath(url, NULL, NULL);

    lua_pushstring(L, "");

    return 1;
}

IIS_LUA_API int iis_http_print(lua_State *L)
{
    auto *ctx = iis_get_http_ctx(L);

    auto message = luaL_checkstring(L, 1);

    return 0;
}

IIS_LUA_API int iis_http_redirect(lua_State *L)
{
    auto *ctx = iis_get_http_ctx(L);

    auto url = luaL_checkstring(L, 1);

    ctx->GetResponse()->Redirect(url);
    ctx->SetRequestHandled();

    return 0;
}

IIS_LUA_API int iis_http_req_get_headers(lua_State *L)
{
    auto *ctx = iis_get_http_ctx(L);

    return 0;
}

IIS_LUA_API int iis_http_req_get_method(lua_State *L)
{
    auto *ctx = iis_get_http_ctx(L);

    lua_pushstring(L, ctx->GetRequest()->GetHttpMethod());

    return 1;
}

IIS_LUA_API int iis_http_req_http_version(lua_State *L)
{
    auto *ctx = iis_get_http_ctx(L);

    USHORT major, minor;

    ctx->GetRequest()->GetHttpVersion(&major, &minor);

    char version[4];

    sprintf_s(version, "%d.%d", major, minor);

    lua_pushstring(L, version);

    return 1;
}

IIS_LUA_API int iis_http_req_set_method(lua_State *L)
{
    auto *ctx = iis_get_http_ctx(L);

    auto method = luaL_checkstring(L, 1);

    ctx->GetRequest()->SetHttpMethod(method);

    return 0;
}

IIS_LUA_API int iis_http_req_set_url(lua_State *L)
{
    auto *ctx = iis_get_http_ctx(L);

    auto url = luaL_checkstring(L, 1);

    ctx->GetRequest()->SetUrl(url, strlen(url), TRUE);

    return 0;
}

IIS_LUA_API int iis_http_resp_get_headers(lua_State *L)
{
    auto *ctx = iis_get_http_ctx(L);

    return 0;
}

IIS_LUA_API int iis_http_resp_set_status(lua_State *L)
{
    auto *ctx = iis_get_http_ctx(L);

    int status = luaL_checkinteger(L, 1);

    ctx->GetResponse()->SetStatus(status, "OK");

    return 0;
}

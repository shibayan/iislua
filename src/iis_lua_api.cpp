
#include "stdafx.h"

static const PCSTR iis_lua_ctx_key = "__iis_lua_ctx";
static const PCSTR iis_lua_handled_key = "__iis_lua_handled";

IIS_LUA_API inline IHttpContext *iis_lua_get_http_ctx(lua_State *L)
{
    lua_getglobal(L, iis_lua_ctx_key);

    auto ctx = reinterpret_cast<IHttpContext *>(lua_touserdata(L, -1));

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

    OutputDebugString(message);
    OutputDebugString("\n");

    return 0;
}

IIS_LUA_API int iis_lua_exec(lua_State *L)
{
    return 0;
}

IIS_LUA_API int iis_lua_exit(lua_State *L)
{
    auto ctx = iis_lua_get_http_ctx(L);

    auto status = static_cast<USHORT>(luaL_checkinteger(L, 1));

    ctx->GetResponse()->SetStatus(status, iis_lua_util_get_status_reason(status));
    
    iis_lua_set_handled(L);

    return 0;
}

IIS_LUA_API int iis_lua_flush(lua_State *L)
{
    auto ctx = iis_lua_get_http_ctx(L);

    DWORD sent;

    ctx->GetResponse()->Flush(FALSE, TRUE, &sent);

    return 0;
}

IIS_LUA_API int iis_lua_headers_sent(lua_State *L)
{
    auto ctx = iis_lua_get_http_ctx(L);

    lua_pushboolean(L, ctx->GetResponseHeadersSent());

    return 1;
}

IIS_LUA_API int iis_lua_map_path(lua_State *L)
{
    auto ctx = iis_lua_get_http_ctx(L);

    auto url = luaL_checkstring(L, 1);

    //ctx->MapPath(url, NULL, NULL);

    lua_pushstring(L, "");

    return 1;
}

IIS_LUA_API int iis_lua_print(lua_State *L)
{
    auto *ctx = iis_lua_get_http_ctx(L);

    auto message = luaL_checkstring(L, 1);

    DWORD sent;
    HTTP_DATA_CHUNK dataChunk;

    dataChunk.DataChunkType = HttpDataChunkFromMemory;
    dataChunk.FromMemory.pBuffer = const_cast<char *>(message);
    dataChunk.FromMemory.BufferLength = static_cast<ULONG>(strlen(message));

    ctx->GetResponse()->WriteEntityChunks(&dataChunk, 1, FALSE, TRUE, &sent);

    return 0;
}

IIS_LUA_API int iis_lua_redirect(lua_State *L)
{
    auto ctx = iis_lua_get_http_ctx(L);

    auto url = luaL_checkstring(L, 1);

    ctx->GetResponse()->Redirect(url);

    iis_lua_set_handled(L);

    return 0;
}

IIS_LUA_API int iis_lua_req_get_headers(lua_State *L)
{
    auto ctx = iis_lua_get_http_ctx(L);

    auto headers = ctx->GetRequest()->GetRawHttpRequest()->Headers;

    lua_createtable(L, 0, headers.UnknownHeaderCount);

    // Append Known Headers
    for (size_t i = 0; i < HttpHeaderRequestMaximum; i++)
    {
        if (headers.KnownHeaders[i].pRawValue != NULL)
        {
            lua_pushstring(L, http_header_id_to_req_name[i]);
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
    auto ctx = iis_lua_get_http_ctx(L);

    lua_pushstring(L, ctx->GetRequest()->GetHttpMethod());

    return 1;
}

IIS_LUA_API int iis_lua_req_http_version(lua_State *L)
{
    auto ctx = iis_lua_get_http_ctx(L);

    USHORT major, minor;
    char version[4];

    ctx->GetRequest()->GetHttpVersion(&major, &minor);

    sprintf_s(version, "%d.%d", major, minor);

    lua_pushstring(L, version);

    return 1;
}

IIS_LUA_API int iis_lua_req_set_header(lua_State *L)
{
    auto ctx = iis_lua_get_http_ctx(L);

    auto name = luaL_checkstring(L, 1);
    auto value = luaL_checkstring(L, 2);

    ctx->GetRequest()->SetHeader(name, value, static_cast<USHORT>(strlen(value)), TRUE);

    return 0;
}

IIS_LUA_API int iis_lua_req_set_method(lua_State *L)
{
    auto ctx = iis_lua_get_http_ctx(L);

    auto method = luaL_checkstring(L, 1);

    ctx->GetRequest()->SetHttpMethod(method);

    return 0;
}

IIS_LUA_API int iis_lua_req_set_url(lua_State *L)
{
    auto ctx = iis_lua_get_http_ctx(L);

    auto url = luaL_checkstring(L, 1);

    ctx->GetRequest()->SetUrl(url, static_cast<DWORD>(strlen(url)), TRUE);

    return 0;
}

IIS_LUA_API int iis_lua_resp_get_headers(lua_State *L)
{
    auto ctx = iis_lua_get_http_ctx(L);

    auto headers = ctx->GetResponse()->GetRawHttpResponse()->Headers;

    lua_createtable(L, 0, headers.UnknownHeaderCount);

    // Append Known Headers
    for (size_t i = 0; i < HttpHeaderResponseMaximum; i++)
    {
        if (headers.KnownHeaders[i].pRawValue != NULL)
        {
            lua_pushstring(L, http_header_id_to_resp_name[i]);
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

IIS_LUA_API int iis_lua_resp_get_status(lua_State *L)
{
    auto ctx = iis_lua_get_http_ctx(L);

    USHORT status;
    ctx->GetResponse()->GetStatus(&status);

    lua_pushinteger(L, status);

    return 0;
}

IIS_LUA_API int iis_lua_resp_set_header(lua_State *L)
{
    auto ctx = iis_lua_get_http_ctx(L);

    auto name = luaL_checkstring(L, 1);
    auto value = luaL_checkstring(L, 2);

    ctx->GetResponse()->SetHeader(name, value, static_cast<USHORT>(strlen(value)), TRUE);

    return 0;
}

IIS_LUA_API int iis_lua_resp_set_status(lua_State *L)
{
    auto ctx = iis_lua_get_http_ctx(L);

    auto status = static_cast<USHORT>(luaL_checkinteger(L, 1));

    ctx->GetResponse()->SetStatus(status, iis_lua_util_get_status_reason(status));

    return 0;
}

IIS_LUA_API int iis_lua_server_get_variables(lua_State *L)
{
    auto ctx = iis_lua_get_http_ctx(L);

    auto name = luaL_checkstring(L, 1);

    PCSTR value;
    DWORD length;

    ctx->GetServerVariable(name, &value, &length);

    lua_pushlstring(L, value, length);

    return 1;
}
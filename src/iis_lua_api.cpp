
#include "stdafx.h"

static const PCSTR iis_lua_ctx_key = "__iis_lua_ctx";
static const PCSTR iis_lua_result_key = "__iis_lua_result";

static const struct luaL_Reg iis [] =
{
    { "debug", iis_lua_debug },
    { "exec", iis_lua_exec },
    { "exit", iis_lua_exit },
    { "flush", iis_lua_flush },
    { "headers_sent", iis_lua_headers_sent },
    { "map_path", iis_lua_map_path },
    { "print", iis_lua_print },
    { "redirect", iis_lua_redirect },
    { NULL, NULL }
};

static const struct luaL_Reg iis_req [] =
{
    { "get_headers", iis_lua_req_get_headers },
    { "get_method", iis_lua_req_get_method },
    { "get_url", iis_lua_req_get_url },
    { "get_url_args", iis_lua_req_get_url_args },
    { "http_version", iis_lua_req_http_version },
    { "set_header", iis_lua_req_set_header },
    { "set_method", iis_lua_req_set_method },
    { "set_url", iis_lua_req_set_url },
    { NULL, NULL }
};

static const struct luaL_Reg iis_resp [] =
{
    { "clear", iis_lua_resp_clear },
    { "clear_headers", iis_lua_resp_clear_headers },
    { "get_headers", iis_lua_resp_get_headers },
    { "get_status", iis_lua_resp_get_status },
    { "set_header", iis_lua_resp_set_header },
    { "set_status", iis_lua_resp_set_status },
    { NULL, NULL }
};

IIS_LUA_API inline lua_State *iis_lua_newstate()
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

    // register iis
    lua_setglobal(L, "iis");

    return L;
}

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

IIS_LUA_API inline REQUEST_NOTIFICATION_STATUS iis_lua_get_result(lua_State *L)
{
    lua_getglobal(L, iis_lua_result_key);

    auto result = static_cast<REQUEST_NOTIFICATION_STATUS>(lua_tointeger(L, -1));

    lua_pop(L, 1);

    return result;
}

IIS_LUA_API inline void iis_lua_set_result(lua_State *L, REQUEST_NOTIFICATION_STATUS result = RQ_NOTIFICATION_FINISH_REQUEST)
{
    lua_pushinteger(L, result);
    lua_setglobal(L, iis_lua_result_key);
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
    auto ctx = iis_lua_get_http_ctx(L);

    auto url = luaL_checkstring(L, 1);

    IHttpContext *childCtx;
    BOOL completionExpected;

    ctx->CloneContext(CLONE_FLAG_BASICS | CLONE_FLAG_ENTITY | CLONE_FLAG_HEADERS, &childCtx);

    childCtx->GetRequest()->SetUrl(url, static_cast<DWORD>(strlen(url)), FALSE);

    ctx->ExecuteRequest(TRUE, childCtx, 0, ctx->GetUser(), &completionExpected);

    if (completionExpected)
    {
        // FIX: async completion 
        iis_lua_set_result(L, RQ_NOTIFICATION_PENDING);

        return 0;
    }

    childCtx->ReleaseClonedContext();

    return 0;
}

IIS_LUA_API int iis_lua_exit(lua_State *L)
{
    auto ctx = iis_lua_get_http_ctx(L);

    auto status = static_cast<USHORT>(luaL_checkinteger(L, 1));

    ctx->GetResponse()->SetStatus(status, iis_lua_util_get_status_reason(status));
    
    iis_lua_set_result(L);

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

    auto url = iis_lua_str_to_wstr(luaL_checkstring(L, 1));

    DWORD length = 0;

    // calculate size
    ctx->MapPath(url, NULL, &length);

    auto physicalPath = new WCHAR[length + 1];

    // convert path
    ctx->MapPath(url, physicalPath, &length);

    auto path = iis_lua_wstr_to_str(physicalPath);

    lua_pushstring(L, path);

    // clean up
    delete [] path;
    delete [] physicalPath;
    delete [] url;

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

    if (lua_gettop(L) == 2)
    {
        auto status = static_cast<USHORT>(luaL_checkinteger(L, 2));

        ctx->GetResponse()->SetStatus(status, iis_lua_util_get_status_reason(status));
    }

    iis_lua_set_result(L);

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

IIS_LUA_API int iis_lua_req_get_url(lua_State *L)
{
    auto ctx = iis_lua_get_http_ctx(L);

    auto url = iis_lua_wstr_to_str(ctx->GetRequest()->GetRawHttpRequest()->CookedUrl.pAbsPath);

    lua_pushstring(L, url);

    delete [] url;

    return 1;
}

IIS_LUA_API int iis_lua_req_get_url_args(lua_State *L)
{
    auto ctx = iis_lua_get_http_ctx(L);

    lua_createtable(L, 0, 0);

    if (ctx->GetRequest()->GetRawHttpRequest()->CookedUrl.pQueryString != NULL)
    {
        auto queryString = iis_lua_wstr_to_str(ctx->GetRequest()->GetRawHttpRequest()->CookedUrl.pQueryString);

        delete [] queryString;
    }

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

IIS_LUA_API int iis_lua_resp_clear(lua_State *L)
{
    auto ctx = iis_lua_get_http_ctx(L);

    ctx->GetResponse()->Clear();

    return 0;
}

IIS_LUA_API int iis_lua_resp_clear_headers(lua_State *L)
{
    auto ctx = iis_lua_get_http_ctx(L);

    ctx->GetResponse()->ClearHeaders();

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

    return 1;
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
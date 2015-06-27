
#include "stdafx.h"

int iis_lua_debug(lua_State *L)
{
    auto message = luaL_checkstring(L, 1);

    OutputDebugString(message);
    OutputDebugString("\n");

    return 0;
}

int iis_lua_exec(lua_State *L)
{
    auto ctx = iis_lua_get_http_ctx(L);

    auto url = luaL_checkstring(L, 1);

    IHttpContext *childContext;
    BOOL completionExpected;

    ctx->CloneContext(CLONE_FLAG_BASICS | CLONE_FLAG_ENTITY | CLONE_FLAG_HEADERS, &childContext);

    childContext->GetRequest()->SetUrl(url, static_cast<DWORD>(strlen(url)), FALSE);

    ctx->ExecuteRequest(TRUE, childContext, 0, ctx->GetUser(), &completionExpected);

    if (completionExpected)
    {
        // for async operation
        auto storedContext = iis_lua_get_stored_context(ctx);

        storedContext->SetChildContext(childContext);

        iis_lua_set_result(L, RQ_NOTIFICATION_PENDING);

        return 0;
    }

    childContext->ReleaseClonedContext();

    return 0;
}

int iis_lua_exit(lua_State *L)
{
    auto ctx = iis_lua_get_http_ctx(L);

    auto status = static_cast<USHORT>(luaL_checkinteger(L, 1));

    ctx->GetResponse()->SetStatus(status, iis_lua_util_get_status_reason(status));
    
    iis_lua_set_result(L);

    return 0;
}

int iis_lua_flush(lua_State *L)
{
    auto ctx = iis_lua_get_http_ctx(L);

    DWORD sent;

    ctx->GetResponse()->Flush(FALSE, TRUE, &sent);

    return 0;
}

int iis_lua_headers_sent(lua_State *L)
{
    auto ctx = iis_lua_get_http_ctx(L);

    lua_pushboolean(L, ctx->GetResponseHeadersSent());

    return 1;
}

int iis_lua_map_path(lua_State *L)
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

int iis_lua_print(lua_State *L)
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

int iis_lua_redirect(lua_State *L)
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

int iis_lua_say(lua_State *L)
{
    auto *ctx = iis_lua_get_http_ctx(L);

    auto message = luaL_checkstring(L, 1);

    DWORD sent;
    HTTP_DATA_CHUNK dataChunk[2];

    dataChunk[0].DataChunkType = HttpDataChunkFromMemory;
    dataChunk[0].FromMemory.pBuffer = const_cast<char *>(message);
    dataChunk[0].FromMemory.BufferLength = static_cast<ULONG>(strlen(message));

    dataChunk[1].DataChunkType = HttpDataChunkFromMemory;
    dataChunk[1].FromMemory.pBuffer = "\r\n";
    dataChunk[1].FromMemory.BufferLength = 2;

    ctx->GetResponse()->WriteEntityChunks(dataChunk, 2, FALSE, TRUE, &sent);

    return 0;
}

int iis_lua_req_get_headers(lua_State *L)
{
    auto ctx = iis_lua_get_http_ctx(L);

    auto headers = ctx->GetRequest()->GetRawHttpRequest()->Headers;

    lua_createtable(L, 0, headers.UnknownHeaderCount);

    for (USHORT i = 0; i < HttpHeaderRequestMaximum; i++)
    {
        if (headers.KnownHeaders[i].pRawValue != NULL)
        {
            lua_pushstring(L, iis_lua_util_get_http_req_header(i));
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

int iis_lua_req_get_method(lua_State *L)
{
    auto ctx = iis_lua_get_http_ctx(L);

    lua_pushstring(L, ctx->GetRequest()->GetHttpMethod());

    return 1;
}

int iis_lua_req_get_remote_addr(lua_State *L)
{
    auto ctx = iis_lua_get_http_ctx(L);

    auto remoteAddr = ctx->GetRequest()->GetRemoteAddress();

    char ipAddress[INET6_ADDRSTRLEN];

    if (remoteAddr->sa_family == AF_INET)
    {
        inet_ntop(AF_INET, &reinterpret_cast<SOCKADDR_IN *>(remoteAddr)->sin_addr, ipAddress, sizeof(ipAddress));
    }
    else
    {
        inet_ntop(AF_INET6, &reinterpret_cast<SOCKADDR_IN6 *>(remoteAddr)->sin6_addr, ipAddress, sizeof(ipAddress));
    }

    lua_pushstring(L, ipAddress);

    return 1;
}

int iis_lua_req_get_url(lua_State *L)
{
    auto ctx = iis_lua_get_http_ctx(L);

    auto url = iis_lua_wstr_to_str(ctx->GetRequest()->GetRawHttpRequest()->CookedUrl.pAbsPath);

    lua_pushstring(L, url);

    delete [] url;

    return 1;
}

int iis_lua_req_get_url_args(lua_State *L)
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

int iis_lua_req_http_version(lua_State *L)
{
    auto ctx = iis_lua_get_http_ctx(L);

    USHORT major, minor;
    char version[4];

    ctx->GetRequest()->GetHttpVersion(&major, &minor);

    sprintf_s(version, "%d.%d", major, minor);

    lua_pushstring(L, version);

    return 1;
}

int iis_lua_req_set_header(lua_State *L)
{
    auto ctx = iis_lua_get_http_ctx(L);

    auto name = luaL_checkstring(L, 1);
    auto value = luaL_checkstring(L, 2);

    ctx->GetRequest()->SetHeader(name, value, static_cast<USHORT>(strlen(value)), TRUE);

    return 0;
}

int iis_lua_req_set_method(lua_State *L)
{
    auto ctx = iis_lua_get_http_ctx(L);

    auto method = luaL_checkstring(L, 1);

    ctx->GetRequest()->SetHttpMethod(method);

    return 0;
}

int iis_lua_req_set_url(lua_State *L)
{
    auto ctx = iis_lua_get_http_ctx(L);

    auto url = luaL_checkstring(L, 1);

    ctx->GetRequest()->SetUrl(url, static_cast<DWORD>(strlen(url)), TRUE);

    return 0;
}

int iis_lua_resp_clear(lua_State *L)
{
    auto ctx = iis_lua_get_http_ctx(L);

    ctx->GetResponse()->Clear();

    return 0;
}

int iis_lua_resp_clear_headers(lua_State *L)
{
    auto ctx = iis_lua_get_http_ctx(L);

    ctx->GetResponse()->ClearHeaders();

    return 0;
}

int iis_lua_resp_get_headers(lua_State *L)
{
    auto ctx = iis_lua_get_http_ctx(L);

    auto headers = ctx->GetResponse()->GetRawHttpResponse()->Headers;

    lua_createtable(L, 0, headers.UnknownHeaderCount);

    for (USHORT i = 0; i < HttpHeaderResponseMaximum; i++)
    {
        if (headers.KnownHeaders[i].pRawValue != NULL)
        {
            lua_pushstring(L, iis_lua_util_get_http_resp_header(i));
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

int iis_lua_resp_get_status(lua_State *L)
{
    auto ctx = iis_lua_get_http_ctx(L);

    USHORT status;
    ctx->GetResponse()->GetStatus(&status);

    lua_pushinteger(L, status);

    return 1;
}

int iis_lua_resp_set_header(lua_State *L)
{
    auto ctx = iis_lua_get_http_ctx(L);

    auto name = luaL_checkstring(L, 1);
    auto value = luaL_checkstring(L, 2);

    ctx->GetResponse()->SetHeader(name, value, static_cast<USHORT>(strlen(value)), TRUE);

    return 0;
}

int iis_lua_resp_set_status(lua_State *L)
{
    auto ctx = iis_lua_get_http_ctx(L);

    auto status = static_cast<USHORT>(luaL_checkinteger(L, 1));

    ctx->GetResponse()->SetStatus(status, iis_lua_util_get_status_reason(status));

    return 0;
}

int iis_lua_srv_get_variable(lua_State *L)
{
    auto ctx = iis_lua_get_http_ctx(L);

    auto name = luaL_checkstring(L, 1);

    PCSTR value;
    DWORD length;

    ctx->GetServerVariable(name, &value, &length);

    lua_pushlstring(L, value, length);

    return 1;
}

int iis_lua_srv_set_variable(lua_State *L)
{
    auto ctx = iis_lua_get_http_ctx(L);

    auto name = luaL_checkstring(L, 1);
    auto value = iis_lua_str_to_wstr(luaL_checkstring(L, 2));

    ctx->SetServerVariable(name, value);

    delete [] value;

    return 0;
}

int iis_lua_user_get_name(lua_State *L)
{
    auto ctx = iis_lua_get_http_ctx(L);

    auto name = iis_lua_wstr_to_str(ctx->GetUser()->GetUserName());

    lua_pushstring(L, name);

    delete [] name;

    return 1;
}

int iis_lua_user_get_Type(lua_State *L)
{
    auto ctx = iis_lua_get_http_ctx(L);

    auto name = iis_lua_wstr_to_str(ctx->GetUser()->GetAuthenticationType());

    lua_pushstring(L, name);

    delete [] name;

    return 1;
}
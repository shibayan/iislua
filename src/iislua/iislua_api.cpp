
#include "stdafx.h"

int iislua_debug(lua_State* L)
{
    auto message = luaL_checkstring(L, 1);

    return 0;
}

int iislua_exec(lua_State* L)
{
    CHECK_ARGUMENT(L, 1);

    auto ctx = iislua_get_http_ctx(L);

    if (ctx == NULL)
    {
        return luaL_error(L, "context is null");
    }

    auto url = luaL_checkstring(L, 1);

    IHttpContext* childContext;
    BOOL completionExpected;

    ctx->CloneContext(CLONE_FLAG_BASICS | CLONE_FLAG_ENTITY | CLONE_FLAG_HEADERS, &childContext);

    childContext->GetRequest()->SetUrl(url, static_cast<DWORD>(strlen(url)), FALSE);

    ctx->ExecuteRequest(TRUE, childContext, 0, ctx->GetUser(), &completionExpected);

    if (completionExpected)
    {
        // for async operation
        auto storedContext = CLuaHttpStoredContext::GetContext(ctx);

        storedContext->SetChildContext(childContext);

        iislua_set_result(L, RQ_NOTIFICATION_PENDING);

        return 0;
    }

    childContext->ReleaseClonedContext();

    return 0;
}

int iislua_exit(lua_State* L)
{
    CHECK_ARGUMENT(L, 1);

    auto ctx = iislua_get_http_ctx(L);

    if (ctx == NULL)
    {
        return luaL_error(L, "context is null");
    }

    auto status = static_cast<USHORT>(luaL_checkinteger(L, 1));

    ctx->GetResponse()->SetStatus(status, iislua_util_get_status_reason(status));

    iislua_set_result(L);

    return 0;
}

int iislua_flush(lua_State* L)
{
    auto ctx = iislua_get_http_ctx(L);

    if (ctx == NULL)
    {
        return luaL_error(L, "context is null");
    }

    DWORD sent;

    ctx->GetResponse()->Flush(FALSE, TRUE, &sent);

    return 0;
}

int iislua_headers_sent(lua_State* L)
{
    auto ctx = iislua_get_http_ctx(L);

    if (ctx == NULL)
    {
        return luaL_error(L, "context is null");
    }

    lua_pushboolean(L, ctx->GetResponseHeadersSent());

    return 1;
}

int iislua_map_path(lua_State* L)
{
    CHECK_ARGUMENT(L, 1);

    auto ctx = iislua_get_http_ctx(L);

    if (ctx == NULL)
    {
        return luaL_error(L, "context is null");
    }

    auto url = iislua_to_wstr(luaL_checkstring(L, 1));

    DWORD length = 0;

    // calculate size
    ctx->MapPath(url.c_str(), NULL, &length);

    auto physicalPath = std::vector<wchar_t>(length + 1);

    // convert path
    ctx->MapPath(url.c_str(), &physicalPath[0], &length);

    auto path = iislua_to_str(&physicalPath[0]);

    lua_pushstring(L, path.c_str());

    return 1;
}

int iislua_print(lua_State* L)
{
    CHECK_ARGUMENT(L, 1);

    auto* ctx = iislua_get_http_ctx(L);

    if (ctx == NULL)
    {
        return luaL_error(L, "context is null");
    }

    auto message = luaL_checkstring(L, 1);

    DWORD sent;
    HTTP_DATA_CHUNK dataChunk;

    dataChunk.DataChunkType = HttpDataChunkFromMemory;
    dataChunk.FromMemory.pBuffer = const_cast<char*>(message);
    dataChunk.FromMemory.BufferLength = static_cast<ULONG>(strlen(message));

    ctx->GetResponse()->WriteEntityChunks(&dataChunk, 1, FALSE, TRUE, &sent);

    return 0;
}

int iislua_redirect(lua_State* L)
{
    if (lua_gettop(L) != 1 && lua_gettop(L) != 2)
    {
        return luaL_error(L, "argument error");
    }

    auto ctx = iislua_get_http_ctx(L);

    if (ctx == NULL)
    {
        return luaL_error(L, "context is null");
    }

    auto url = luaL_checkstring(L, 1);

    ctx->GetResponse()->Redirect(url);

    if (lua_gettop(L) == 2)
    {
        auto status = static_cast<USHORT>(luaL_checkinteger(L, 2));

        ctx->GetResponse()->SetStatus(status, iislua_util_get_status_reason(status));
    }

    iislua_set_result(L);

    return 0;
}

int iislua_say(lua_State* L)
{
    CHECK_ARGUMENT(L, 1);

    auto* ctx = iislua_get_http_ctx(L);

    if (ctx == NULL)
    {
        return luaL_error(L, "context is null");
    }

    auto message = luaL_checkstring(L, 1);
    auto newLine = "\r\n";

    DWORD sent;
    HTTP_DATA_CHUNK dataChunk[2];

    dataChunk[0].DataChunkType = HttpDataChunkFromMemory;
    dataChunk[0].FromMemory.pBuffer = const_cast<char*>(message);
    dataChunk[0].FromMemory.BufferLength = static_cast<ULONG>(strlen(message));

    dataChunk[1].DataChunkType = HttpDataChunkFromMemory;
    dataChunk[1].FromMemory.pBuffer = const_cast<char*>(newLine);
    dataChunk[1].FromMemory.BufferLength = 2;

    ctx->GetResponse()->WriteEntityChunks(dataChunk, 2, FALSE, TRUE, &sent);

    return 0;
}
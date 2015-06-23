
#include "stdafx.h"

REQUEST_NOTIFICATION_STATUS CLuaHttpModule::OnBeginRequest(IN IHttpContext *pHttpContext, IN IHttpEventProvider *pProvider)
{
    UNREFERENCED_PARAMETER(pProvider);

    auto config = iis_lua_get_config(pHttpContext);

    auto L = iis_lua_newstate();

    iis_lua_set_http_ctx(L, pHttpContext);
    iis_lua_set_result(L, RQ_NOTIFICATION_CONTINUE);

    if (luaL_dofile(L, config->GetBeginRequest()))
    {
        auto text = lua_tostring(L, -1);

        OutputDebugString(text);
    }

    auto result = iis_lua_get_result(L);

    lua_close(L);

    return result;
}

REQUEST_NOTIFICATION_STATUS CLuaHttpModule::OnMapPath(IN IHttpContext *pHttpContext, IN IMapPathProvider *pProvider)
{
    UNREFERENCED_PARAMETER(pProvider);

    // correct?
    if (pHttpContext->GetParentContext() != NULL)
    {
        return RQ_NOTIFICATION_CONTINUE;
    }

    auto config = iis_lua_get_config(pHttpContext);

    auto L = iis_lua_newstate();

    iis_lua_set_http_ctx(L, pHttpContext);
    iis_lua_set_result(L, RQ_NOTIFICATION_CONTINUE);

    if (luaL_dofile(L, config->GetMapPath()))
    {
        auto text = lua_tostring(L, -1);

        OutputDebugString(text);
    }

    auto result = iis_lua_get_result(L);

    lua_close(L);

    return result;
}

REQUEST_NOTIFICATION_STATUS CLuaHttpModule::OnAsyncCompletion(IN IHttpContext *pHttpContext, IN DWORD dwNotification, IN BOOL fPostNotification, IN IHttpEventProvider *pProvider, IN IHttpCompletionInfo *pCompletionInfo)
{
    UNREFERENCED_PARAMETER(pHttpContext);
    UNREFERENCED_PARAMETER(dwNotification);
    UNREFERENCED_PARAMETER(fPostNotification);
    UNREFERENCED_PARAMETER(pProvider);
    UNREFERENCED_PARAMETER(pCompletionInfo);

    // TODO: Not Implemented

    return RQ_NOTIFICATION_CONTINUE;
}
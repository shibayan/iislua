
#include "stdafx.h"

REQUEST_NOTIFICATION_STATUS CLuaHttpModule::OnBeginRequest(IN IHttpContext *pHttpContext, IN IHttpEventProvider *pProvider)
{
    UNREFERENCED_PARAMETER(pProvider);

    auto config = iis_lua_get_config(pHttpContext);

    if (config->GetBeginRequest().empty())
    {
        return RQ_NOTIFICATION_CONTINUE;
    }

    auto L = iis_lua_newstate(pHttpContext);

    if (luaL_dofile(L, config->GetBeginRequest().c_str()))
    {
        auto error = lua_tostring(L, -1);

        OutputDebugString(error);

        pHttpContext->GetResponse()->SetStatus(500, "Internal Server Error");

        return RQ_NOTIFICATION_FINISH_REQUEST;
    }

    return iis_lua_close(L);
}

REQUEST_NOTIFICATION_STATUS CLuaHttpModule::OnAuthenticateRequest(IN IHttpContext *pHttpContext, IN IAuthenticationProvider *pProvider)
{
    UNREFERENCED_PARAMETER(pProvider);

    auto config = iis_lua_get_config(pHttpContext);

    if (config->GetAuthenticateRequest().empty())
    {
        return RQ_NOTIFICATION_CONTINUE;
    }

    auto L = iis_lua_newstate(pHttpContext);

    if (luaL_dofile(L, config->GetAuthenticateRequest().c_str()))
    {
        auto error = lua_tostring(L, -1);

        OutputDebugString(error);

        pHttpContext->GetResponse()->SetStatus(500, "Internal Server Error");

        return RQ_NOTIFICATION_FINISH_REQUEST;
    }

    return iis_lua_close(L);
}

REQUEST_NOTIFICATION_STATUS CLuaHttpModule::OnAuthorizeRequest(IN IHttpContext *pHttpContext, IN IHttpEventProvider *pProvider)
{
    UNREFERENCED_PARAMETER(pProvider);

    auto config = iis_lua_get_config(pHttpContext);

    if (config->GetAuthorizeRequest().empty())
    {
        return RQ_NOTIFICATION_CONTINUE;
    }

    auto L = iis_lua_newstate(pHttpContext);

    if (luaL_dofile(L, config->GetAuthorizeRequest().c_str()))
    {
        auto error = lua_tostring(L, -1);

        OutputDebugString(error);

        pHttpContext->GetResponse()->SetStatus(500, "Internal Server Error");

        return RQ_NOTIFICATION_FINISH_REQUEST;
    }

    return iis_lua_close(L);
}

REQUEST_NOTIFICATION_STATUS CLuaHttpModule::OnExecuteRequestHandler(IN IHttpContext *pHttpContext, IN IHttpEventProvider *pProvider)
{
    UNREFERENCED_PARAMETER(pProvider);

    auto config = iis_lua_get_config(pHttpContext);

    if (config->GetExecuteRequest().empty())
    {
        return RQ_NOTIFICATION_CONTINUE;
    }

    auto L = iis_lua_newstate(pHttpContext);

    if (luaL_dofile(L, config->GetExecuteRequest().c_str()))
    {
        auto error = lua_tostring(L, -1);

        OutputDebugString(error);

        pHttpContext->GetResponse()->SetStatus(500, "Internal Server Error");

        return RQ_NOTIFICATION_FINISH_REQUEST;
    }

    return iis_lua_close(L);
}

REQUEST_NOTIFICATION_STATUS CLuaHttpModule::OnLogRequest(IN IHttpContext *pHttpContext, IN IHttpEventProvider *pProvider)
{
    UNREFERENCED_PARAMETER(pProvider);

    auto config = iis_lua_get_config(pHttpContext);

    if (config->GetLogRequest().empty())
    {
        return RQ_NOTIFICATION_CONTINUE;
    }

    auto L = iis_lua_newstate(pHttpContext);

    if (luaL_dofile(L, config->GetLogRequest().c_str()))
    {
        auto error = lua_tostring(L, -1);

        OutputDebugString(error);

        pHttpContext->GetResponse()->SetStatus(500, "Internal Server Error");

        return RQ_NOTIFICATION_FINISH_REQUEST;
    }

    return iis_lua_close(L);
}

REQUEST_NOTIFICATION_STATUS CLuaHttpModule::OnMapPath(IN IHttpContext *pHttpContext, IN IMapPathProvider *pProvider)
{
    UNREFERENCED_PARAMETER(pProvider);

    auto config = iis_lua_get_config(pHttpContext);

    if (config->GetMapPath().empty())
    {
        return RQ_NOTIFICATION_CONTINUE;
    }

    auto L = iis_lua_newstate(pHttpContext);

    if (luaL_dofile(L, config->GetMapPath().c_str()))
    {
        auto error = lua_tostring(L, -1);

        OutputDebugString(error);

        pHttpContext->GetResponse()->SetStatus(500, "Internal Server Error");

        return RQ_NOTIFICATION_FINISH_REQUEST;
    }

    return iis_lua_close(L);
}

REQUEST_NOTIFICATION_STATUS CLuaHttpModule::OnAsyncCompletion(IN IHttpContext *pHttpContext, IN DWORD dwNotification, IN BOOL fPostNotification, IN IHttpEventProvider *pProvider, IN IHttpCompletionInfo *pCompletionInfo)
{
    UNREFERENCED_PARAMETER(pHttpContext);
    UNREFERENCED_PARAMETER(dwNotification);
    UNREFERENCED_PARAMETER(fPostNotification);
    UNREFERENCED_PARAMETER(pProvider);
    UNREFERENCED_PARAMETER(pCompletionInfo);

    auto storedContext = iis_lua_get_stored_context(pHttpContext);

    storedContext->GetChildContext()->ReleaseClonedContext();
    storedContext->SetChildContext(NULL);

    return RQ_NOTIFICATION_CONTINUE;
}
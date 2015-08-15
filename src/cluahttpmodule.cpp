
#include "stdafx.h"

REQUEST_NOTIFICATION_STATUS CLuaHttpModule::OnBeginRequest(IN IHttpContext *pHttpContext, IN IHttpEventProvider *pProvider)
{
    UNREFERENCED_PARAMETER(pProvider);

    auto config = iis_lua_get_config(pHttpContext);

    if (config->GetBeginRequest().empty())
    {
        return RQ_NOTIFICATION_CONTINUE;
    }

    if (!iis_lua_loadfile(config->GetLuaState(), config->GetBeginRequest().c_str(), config->IsEnableCodeCache()))
    {
        return RQ_NOTIFICATION_CONTINUE;
    }

    auto L = iis_lua_newthread(config->GetLuaState());

    iis_lua_set_sandbox(config->GetLuaState(), L);

    iis_lua_set_http_ctx(L, pHttpContext);
    iis_lua_set_result(L, RQ_NOTIFICATION_CONTINUE);

    if (lua_pcall(L, 0, 1, 0))
    {
        auto error = lua_tostring(L, -1);

        OutputDebugString(error);

        pHttpContext->GetResponse()->SetStatus(500, "Internal Server Error");

        return RQ_NOTIFICATION_FINISH_REQUEST;
    }

    return iis_lua_get_result(L);
}

REQUEST_NOTIFICATION_STATUS CLuaHttpModule::OnAuthenticateRequest(IN IHttpContext *pHttpContext, IN IAuthenticationProvider *pProvider)
{
    UNREFERENCED_PARAMETER(pProvider);

    auto config = iis_lua_get_config(pHttpContext);

    if (config->GetAuthenticateRequest().empty())
    {
        return RQ_NOTIFICATION_CONTINUE;
    }

    if (!iis_lua_loadfile(config->GetLuaState(), config->GetAuthenticateRequest().c_str(), config->IsEnableCodeCache()))
    {
        return RQ_NOTIFICATION_CONTINUE;
    }

    auto L = iis_lua_newthread(config->GetLuaState());

    iis_lua_set_sandbox(config->GetLuaState(), L);

    iis_lua_set_http_ctx(L, pHttpContext);
    iis_lua_set_result(L, RQ_NOTIFICATION_CONTINUE);

    if (lua_pcall(L, 0, 1, 0))
    {
        auto error = lua_tostring(L, -1);

        OutputDebugString(error);

        pHttpContext->GetResponse()->SetStatus(500, "Internal Server Error");

        return RQ_NOTIFICATION_FINISH_REQUEST;
    }

    return iis_lua_get_result(L);
}

REQUEST_NOTIFICATION_STATUS CLuaHttpModule::OnAuthorizeRequest(IN IHttpContext *pHttpContext, IN IHttpEventProvider *pProvider)
{
    UNREFERENCED_PARAMETER(pProvider);

    auto config = iis_lua_get_config(pHttpContext);

    if (config->GetAuthorizeRequest().empty())
    {
        return RQ_NOTIFICATION_CONTINUE;
    }

    if (!iis_lua_loadfile(config->GetLuaState(), config->GetAuthorizeRequest().c_str(), config->IsEnableCodeCache()))
    {
        return RQ_NOTIFICATION_CONTINUE;
    }

    auto L = iis_lua_newthread(config->GetLuaState());

    iis_lua_set_sandbox(config->GetLuaState(), L);

    iis_lua_set_http_ctx(L, pHttpContext);
    iis_lua_set_result(L, RQ_NOTIFICATION_CONTINUE);

    if (lua_pcall(L, 0, 1, 0))
    {
        auto error = lua_tostring(L, -1);

        OutputDebugString(error);

        pHttpContext->GetResponse()->SetStatus(500, "Internal Server Error");

        return RQ_NOTIFICATION_FINISH_REQUEST;
    }

    return iis_lua_get_result(L);
}

REQUEST_NOTIFICATION_STATUS CLuaHttpModule::OnExecuteRequestHandler(IN IHttpContext *pHttpContext, IN IHttpEventProvider *pProvider)
{
    UNREFERENCED_PARAMETER(pProvider);

    auto config = iis_lua_get_config(pHttpContext);

    if (config->GetExecuteRequest().empty())
    {
        return RQ_NOTIFICATION_CONTINUE;
    }

    if (!iis_lua_loadfile(config->GetLuaState(), config->GetExecuteRequest().c_str(), config->IsEnableCodeCache()))
    {
        return RQ_NOTIFICATION_CONTINUE;
    }

    auto L = iis_lua_newthread(config->GetLuaState());

    iis_lua_set_sandbox(config->GetLuaState(), L);

    iis_lua_set_http_ctx(L, pHttpContext);
    iis_lua_set_result(L, RQ_NOTIFICATION_CONTINUE);

    if (lua_pcall(L, 0, 1, 0))
    {
        auto error = lua_tostring(L, -1);

        OutputDebugString(error);

        pHttpContext->GetResponse()->SetStatus(500, "Internal Server Error");

        return RQ_NOTIFICATION_FINISH_REQUEST;
    }

    return iis_lua_get_result(L);
}

REQUEST_NOTIFICATION_STATUS CLuaHttpModule::OnLogRequest(IN IHttpContext *pHttpContext, IN IHttpEventProvider *pProvider)
{
    UNREFERENCED_PARAMETER(pProvider);

    auto config = iis_lua_get_config(pHttpContext);

    if (config->GetLogRequest().empty())
    {
        return RQ_NOTIFICATION_CONTINUE;
    }

    if (!iis_lua_loadfile(config->GetLuaState(), config->GetLogRequest().c_str(), config->IsEnableCodeCache()))
    {
        return RQ_NOTIFICATION_CONTINUE;
    }

    auto L = iis_lua_newthread(config->GetLuaState());

    iis_lua_set_sandbox(config->GetLuaState(), L);

    iis_lua_set_http_ctx(L, pHttpContext);
    iis_lua_set_result(L, RQ_NOTIFICATION_CONTINUE);

    if (lua_pcall(L, 0, 1, 0))
    {
        auto error = lua_tostring(L, -1);

        OutputDebugString(error);

        pHttpContext->GetResponse()->SetStatus(500, "Internal Server Error");

        return RQ_NOTIFICATION_FINISH_REQUEST;
    }

    return iis_lua_get_result(L);
}

REQUEST_NOTIFICATION_STATUS CLuaHttpModule::OnEndRequest(IN IHttpContext *pHttpContext, IN IHttpEventProvider *pProvider)
{
    UNREFERENCED_PARAMETER(pProvider);

    auto config = iis_lua_get_config(pHttpContext);

    if (config->GetEndRequest().empty())
    {
        return RQ_NOTIFICATION_CONTINUE;
    }

    if (!iis_lua_loadfile(config->GetLuaState(), config->GetEndRequest().c_str(), config->IsEnableCodeCache()))
    {
        return RQ_NOTIFICATION_CONTINUE;
    }

    auto L = iis_lua_newthread(config->GetLuaState());

    iis_lua_set_sandbox(config->GetLuaState(), L);

    iis_lua_set_http_ctx(L, pHttpContext);
    iis_lua_set_result(L, RQ_NOTIFICATION_CONTINUE);

    if (lua_pcall(L, 0, 1, 0))
    {
        auto error = lua_tostring(L, -1);

        OutputDebugString(error);

        pHttpContext->GetResponse()->SetStatus(500, "Internal Server Error");

        return RQ_NOTIFICATION_FINISH_REQUEST;
    }

    return iis_lua_get_result(L);
}

REQUEST_NOTIFICATION_STATUS CLuaHttpModule::OnMapPath(IN IHttpContext *pHttpContext, IN IMapPathProvider *pProvider)
{
    UNREFERENCED_PARAMETER(pProvider);

    auto config = iis_lua_get_config(pHttpContext);

    if (config->GetMapPath().empty())
    {
        return RQ_NOTIFICATION_CONTINUE;
    }

    if (!iis_lua_loadfile(config->GetLuaState(), config->GetMapPath().c_str(), config->IsEnableCodeCache()))
    {
        return RQ_NOTIFICATION_CONTINUE;
    }

    auto L = iis_lua_newthread(config->GetLuaState());

    iis_lua_set_sandbox(config->GetLuaState(), L);

    iis_lua_set_http_ctx(L, pHttpContext);
    iis_lua_set_result(L, RQ_NOTIFICATION_CONTINUE);

    if (lua_pcall(L, 0, 1, 0))
    {
        auto error = lua_tostring(L, -1);

        OutputDebugString(error);

        pHttpContext->GetResponse()->SetStatus(500, "Internal Server Error");

        return RQ_NOTIFICATION_FINISH_REQUEST;
    }

    return iis_lua_get_result(L);
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
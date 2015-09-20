
#include "stdafx.h"

REQUEST_NOTIFICATION_STATUS CLuaHttpModule::OnBeginRequest(IN IHttpContext *pHttpContext, IN IHttpEventProvider *pProvider)
{
    UNREFERENCED_PARAMETER(pProvider);

    auto config = CModuleConfiguration::GetContext(pHttpContext);

    if (!config || config->GetBeginRequest().empty())
    {
        return RQ_NOTIFICATION_CONTINUE;
    }

    auto L = iislua_loadfile(config, config->GetBeginRequest().c_str());

    if (!L)
    {
        return RQ_NOTIFICATION_CONTINUE;
    }

    iislua_initialize(L, pHttpContext);

    if (lua_pcall(L, 0, 1, 0))
    {
        auto error = lua_tostring(L, -1);

        OutputDebugString(error);

        pHttpContext->GetResponse()->SetStatus(500, "Internal Server Error");

        return RQ_NOTIFICATION_FINISH_REQUEST;
    }

    return iislua_finish_request(L);
}

REQUEST_NOTIFICATION_STATUS CLuaHttpModule::OnAuthenticateRequest(IN IHttpContext *pHttpContext, IN IAuthenticationProvider *pProvider)
{
    UNREFERENCED_PARAMETER(pProvider);

    auto config = CModuleConfiguration::GetContext(pHttpContext);

    if (!config || config->GetAuthenticateRequest().empty())
    {
        return RQ_NOTIFICATION_CONTINUE;
    }

    auto L = iislua_loadfile(config, config->GetAuthenticateRequest().c_str());

    if (!L)
    {
        return RQ_NOTIFICATION_CONTINUE;
    }

    iislua_initialize(L, pHttpContext);

    if (lua_pcall(L, 0, 1, 0))
    {
        auto error = lua_tostring(L, -1);

        OutputDebugString(error);

        pHttpContext->GetResponse()->SetStatus(500, "Internal Server Error");

        return RQ_NOTIFICATION_FINISH_REQUEST;
    }

    return iislua_finish_request(L);
}

REQUEST_NOTIFICATION_STATUS CLuaHttpModule::OnAuthorizeRequest(IN IHttpContext *pHttpContext, IN IHttpEventProvider *pProvider)
{
    UNREFERENCED_PARAMETER(pProvider);

    auto config = CModuleConfiguration::GetContext(pHttpContext);

    if (!config || config->GetAuthorizeRequest().empty())
    {
        return RQ_NOTIFICATION_CONTINUE;
    }

    auto L = iislua_loadfile(config, config->GetAuthorizeRequest().c_str());

    if (!L)
    {
        return RQ_NOTIFICATION_CONTINUE;
    }

    iislua_initialize(L, pHttpContext);

    if (lua_pcall(L, 0, 1, 0))
    {
        auto error = lua_tostring(L, -1);

        OutputDebugString(error);

        pHttpContext->GetResponse()->SetStatus(500, "Internal Server Error");

        return RQ_NOTIFICATION_FINISH_REQUEST;
    }

    return iislua_finish_request(L);
}

REQUEST_NOTIFICATION_STATUS CLuaHttpModule::OnExecuteRequestHandler(IN IHttpContext *pHttpContext, IN IHttpEventProvider *pProvider)
{
    UNREFERENCED_PARAMETER(pProvider);

    auto config = CModuleConfiguration::GetContext(pHttpContext);

    if (!config || config->GetExecuteRequest().empty())
    {
        return RQ_NOTIFICATION_CONTINUE;
    }

    auto L = iislua_loadfile(config, config->GetExecuteRequest().c_str());

    if (!L)
    {
        return RQ_NOTIFICATION_CONTINUE;
    }

    iislua_initialize(L, pHttpContext);

    if (lua_pcall(L, 0, 1, 0))
    {
        auto error = lua_tostring(L, -1);

        OutputDebugString(error);

        pHttpContext->GetResponse()->SetStatus(500, "Internal Server Error");

        return RQ_NOTIFICATION_FINISH_REQUEST;
    }

    return iislua_finish_request(L);
}

REQUEST_NOTIFICATION_STATUS CLuaHttpModule::OnLogRequest(IN IHttpContext *pHttpContext, IN IHttpEventProvider *pProvider)
{
    UNREFERENCED_PARAMETER(pProvider);

    auto config = CModuleConfiguration::GetContext(pHttpContext);

    if (!config || config->GetLogRequest().empty())
    {
        return RQ_NOTIFICATION_CONTINUE;
    }

    auto L = iislua_loadfile(config, config->GetLogRequest().c_str());

    if (!L)
    {
        return RQ_NOTIFICATION_CONTINUE;
    }

    iislua_initialize(L, pHttpContext);

    if (lua_pcall(L, 0, 1, 0))
    {
        auto error = lua_tostring(L, -1);

        OutputDebugString(error);

        pHttpContext->GetResponse()->SetStatus(500, "Internal Server Error");

        return RQ_NOTIFICATION_FINISH_REQUEST;
    }

    return iislua_finish_request(L);
}

REQUEST_NOTIFICATION_STATUS CLuaHttpModule::OnEndRequest(IN IHttpContext *pHttpContext, IN IHttpEventProvider *pProvider)
{
    UNREFERENCED_PARAMETER(pProvider);

    auto config = CModuleConfiguration::GetContext(pHttpContext);

    if (!config || config->GetEndRequest().empty())
    {
        return RQ_NOTIFICATION_CONTINUE;
    }

    auto L = iislua_loadfile(config, config->GetEndRequest().c_str());

    if (!L)
    {
        return RQ_NOTIFICATION_CONTINUE;
    }

    iislua_initialize(L, pHttpContext);

    if (lua_pcall(L, 0, 1, 0))
    {
        auto error = lua_tostring(L, -1);

        OutputDebugString(error);

        pHttpContext->GetResponse()->SetStatus(500, "Internal Server Error");

        return RQ_NOTIFICATION_FINISH_REQUEST;
    }

    return iislua_finish_request(L);
}

REQUEST_NOTIFICATION_STATUS CLuaHttpModule::OnMapPath(IN IHttpContext *pHttpContext, IN IMapPathProvider *pProvider)
{
    UNREFERENCED_PARAMETER(pProvider);

    auto config = CModuleConfiguration::GetContext(pHttpContext);

    if (!config || config->GetMapPath().empty())
    {
        return RQ_NOTIFICATION_CONTINUE;
    }

    auto L = iislua_loadfile(config, config->GetMapPath().c_str());

    if (!L)
    {
        return RQ_NOTIFICATION_CONTINUE;
    }

    iislua_initialize(L, pHttpContext);

    if (lua_pcall(L, 0, 1, 0))
    {
        auto error = lua_tostring(L, -1);

        OutputDebugString(error);

        pHttpContext->GetResponse()->SetStatus(500, "Internal Server Error");

        return RQ_NOTIFICATION_FINISH_REQUEST;
    }

    return iislua_finish_request(L);
}

REQUEST_NOTIFICATION_STATUS CLuaHttpModule::OnAsyncCompletion(IN IHttpContext *pHttpContext, IN DWORD dwNotification, IN BOOL fPostNotification, IN IHttpEventProvider *pProvider, IN IHttpCompletionInfo *pCompletionInfo)
{
    UNREFERENCED_PARAMETER(pHttpContext);
    UNREFERENCED_PARAMETER(dwNotification);
    UNREFERENCED_PARAMETER(fPostNotification);
    UNREFERENCED_PARAMETER(pProvider);
    UNREFERENCED_PARAMETER(pCompletionInfo);

    auto storedContext = CLuaHttpStoredContext::GetContext(pHttpContext);

    storedContext->GetChildContext()->ReleaseClonedContext();
    storedContext->SetChildContext(NULL);

    return RQ_NOTIFICATION_CONTINUE;
}
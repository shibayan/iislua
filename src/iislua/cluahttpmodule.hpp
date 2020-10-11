#pragma once

struct lua_State;

class CLuaStatePool;

class CLuaHttpModule : public CHttpModule
{
    lua_State* L;
    CLuaStatePool* pLuaStatePool;

    REQUEST_NOTIFICATION_STATUS OnExecuteCore(IN IHttpContext* pHttpContext, IN IHttpEventProvider* pProvider, IN const char* name);
public:
    CLuaHttpModule(CLuaStatePool* pLuaStatePool);
    ~CLuaHttpModule();

    REQUEST_NOTIFICATION_STATUS OnBeginRequest(IN IHttpContext* pHttpContext, IN IHttpEventProvider* pProvider)
    {
        return OnExecuteCore(pHttpContext, pProvider, "BeginRequest");
    }

    REQUEST_NOTIFICATION_STATUS OnAuthenticateRequest(IN IHttpContext* pHttpContext, IN IAuthenticationProvider* pProvider)
    {
        return OnExecuteCore(pHttpContext, pProvider, "AuthenticateRequest");
    }

    REQUEST_NOTIFICATION_STATUS OnAuthorizeRequest(IN IHttpContext* pHttpContext, IN IHttpEventProvider* pProvider)
    {
        return OnExecuteCore(pHttpContext, pProvider, "AuthorizeRequest");
    }

    REQUEST_NOTIFICATION_STATUS OnExecuteRequestHandler(IN IHttpContext* pHttpContext, IN IHttpEventProvider* pProvider)
    {
        return OnExecuteCore(pHttpContext, pProvider, "ExecuteRequest");
    }

    REQUEST_NOTIFICATION_STATUS OnEndRequest(IN IHttpContext* pHttpContext, IN IHttpEventProvider* pProvider)
    {
        return OnExecuteCore(pHttpContext, pProvider, "EndRequest");
    }

    REQUEST_NOTIFICATION_STATUS OnSendResponse(IN IHttpContext* pHttpContext, IN ISendResponseProvider* pProvider)
    {
        return OnExecuteCore(pHttpContext, pProvider, "SendResponse");
    }

    REQUEST_NOTIFICATION_STATUS OnMapPath(IN IHttpContext* pHttpContext, IN IMapPathProvider* pProvider)
    {
        return OnExecuteCore(pHttpContext, pProvider, "MapPath");
    }

    REQUEST_NOTIFICATION_STATUS OnAsyncCompletion(IN IHttpContext* pHttpContext, IN DWORD dwNotification, IN BOOL fPostNotification, IN IHttpEventProvider* pProvider, IN IHttpCompletionInfo* pCompletionInfo);
};
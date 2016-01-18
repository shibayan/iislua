#pragma once

struct lua_State;

class CLuaStatePool;

class CLuaHttpModule : public CHttpModule
{
    lua_State *L;
    CLuaStatePool *pLuaStatePool;

    REQUEST_NOTIFICATION_STATUS OnExecuteCore(IN IHttpContext *pHttpContext, IN const char *name);
public:
    CLuaHttpModule(CLuaStatePool *pLuaStatePool);
    ~CLuaHttpModule();

    REQUEST_NOTIFICATION_STATUS OnBeginRequest(IN IHttpContext *pHttpContext, IN IHttpEventProvider *pProvider)
    {
        UNREFERENCED_PARAMETER(pProvider);

        return OnExecuteCore(pHttpContext, "BeginRequest");
    }
    
    REQUEST_NOTIFICATION_STATUS OnAuthenticateRequest(IN IHttpContext *pHttpContext, IN IAuthenticationProvider *pProvider)
    {
        UNREFERENCED_PARAMETER(pProvider);

        return OnExecuteCore(pHttpContext, "AuthenticateRequest");
    }
    
    REQUEST_NOTIFICATION_STATUS OnAuthorizeRequest(IN IHttpContext *pHttpContext, IN IHttpEventProvider *pProvider)
    {
        UNREFERENCED_PARAMETER(pProvider);

        return OnExecuteCore(pHttpContext, "AuthorizeRequest");
    }
    
    REQUEST_NOTIFICATION_STATUS OnExecuteRequestHandler(IN IHttpContext *pHttpContext, IN IHttpEventProvider *pProvider)
    {
        UNREFERENCED_PARAMETER(pProvider);

        return OnExecuteCore(pHttpContext, "ExecuteRequest");
    }
    
    REQUEST_NOTIFICATION_STATUS OnLogRequest(IN IHttpContext *pHttpContext, IN IHttpEventProvider *pProvider)
    {
        UNREFERENCED_PARAMETER(pProvider);

        return OnExecuteCore(pHttpContext, "LogRequest");
    }

    REQUEST_NOTIFICATION_STATUS OnEndRequest(IN IHttpContext *pHttpContext, IN IHttpEventProvider *pProvider)
    {
        UNREFERENCED_PARAMETER(pProvider);

        return OnExecuteCore(pHttpContext, "EndRequest");
    }

    REQUEST_NOTIFICATION_STATUS OnMapPath(IN IHttpContext *pHttpContext, IN IMapPathProvider *pProvider)
    {
        UNREFERENCED_PARAMETER(pProvider);

        return OnExecuteCore(pHttpContext, "MapPath");
    }

    REQUEST_NOTIFICATION_STATUS OnAsyncCompletion(IN IHttpContext *pHttpContext, IN DWORD dwNotification, IN BOOL fPostNotification, IN IHttpEventProvider *pProvider, IN IHttpCompletionInfo *pCompletionInfo);
};
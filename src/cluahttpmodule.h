#pragma once

class CLuaHttpModule : public CHttpModule
{
public:
    REQUEST_NOTIFICATION_STATUS OnBeginRequest(IN IHttpContext *pHttpContext, IN IHttpEventProvider *pProvider);
    REQUEST_NOTIFICATION_STATUS OnAuthenticateRequest(IN IHttpContext *pHttpContext, IN IAuthenticationProvider *pProvider);
    REQUEST_NOTIFICATION_STATUS OnAuthorizeRequest(IN IHttpContext *pHttpContext, IN IHttpEventProvider *pProvider);
    REQUEST_NOTIFICATION_STATUS OnExecuteRequestHandler(IN IHttpContext *pHttpContext, IN IHttpEventProvider *pProvider);
    REQUEST_NOTIFICATION_STATUS OnLogRequest(IN IHttpContext *pHttpContext, IN IHttpEventProvider *pProvider);
    REQUEST_NOTIFICATION_STATUS OnEndRequest(IN IHttpContext *pHttpContext, IN IHttpEventProvider *pProvider);

    REQUEST_NOTIFICATION_STATUS OnMapPath(IN IHttpContext *pHttpContext, IN IMapPathProvider *pProvider);

    REQUEST_NOTIFICATION_STATUS OnAsyncCompletion(IN IHttpContext *pHttpContext, IN DWORD dwNotification, IN BOOL fPostNotification, IN IHttpEventProvider *pProvider, IN IHttpCompletionInfo *pCompletionInfo);
};
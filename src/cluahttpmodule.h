#pragma once

struct lua_State;
class CHttpModule;

class CLuaHttpModule : public CHttpModule
{
public:
    REQUEST_NOTIFICATION_STATUS OnBeginRequest(IN IHttpContext *pHttpContext, IN IHttpEventProvider *pProvider);
    REQUEST_NOTIFICATION_STATUS OnMapPath(IN IHttpContext *pHttpContext, IN IMapPathProvider *pProvider);
    REQUEST_NOTIFICATION_STATUS OnAsyncCompletion(IN IHttpContext *pHttpContext, IN DWORD dwNotification, IN BOOL fPostNotification, IN IHttpEventProvider *pProvider, IN IHttpCompletionInfo *pCompletionInfo);
};
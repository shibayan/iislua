#pragma once

struct lua_State;
class CHttpModule;

class CLuaHttpModule : public CHttpModule
{
private:
    lua_State *L;

public:
    CLuaHttpModule();
    ~CLuaHttpModule();

    REQUEST_NOTIFICATION_STATUS OnBeginRequest(IN IHttpContext *pHttpContext, IN OUT IHttpEventProvider *pProvider);
};
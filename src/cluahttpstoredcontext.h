#pragma once

class CLuaHttpStoredContext : public IHttpStoredContext
{
private:
    IHttpContext *childContext;
public:
    CLuaHttpStoredContext();

    IHttpContext *GetChildContext() const;
    void SetChildContext(IN IHttpContext *context);

    void CleanupStoredContext();
};
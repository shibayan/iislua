#pragma once

class CLuaHttpStoredContext : public IHttpStoredContext
{
private:
    IHttpContext* pHttpChildContext;
public:
    CLuaHttpStoredContext();

    IHttpContext* GetChildContext() const;
    void SetChildContext(IN IHttpContext* pHttpChildContext);

    void CleanupStoredContext();

    static CLuaHttpStoredContext* GetContext(IN IHttpContext* pHttpContext);
};
#pragma once

class IHttpStoredContext;
class IHttpContext;

class CModuleConfiguration : public IHttpStoredContext
{
private:
    PCSTR beginRequest;
    PCSTR mapPath;

    PCSTR GetString(IAppHostElement *section, PCWSTR name);
public:
    CModuleConfiguration();
    ~CModuleConfiguration();

    HRESULT Initialize(IN IHttpContext *pHttpContext, IN IHttpServer *pHttpServer);

    VOID CleanupStoredContext(VOID);

    PCSTR GetBeginRequest() const;
    PCSTR GetMapPath() const;
};
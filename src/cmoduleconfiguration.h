#pragma once

class CModuleConfiguration : public IHttpStoredContext
{
private:
    PCSTR beginRequest;
    PCSTR authenticateRequest;
    PCSTR authorizeRequest;
    PCSTR logRequest;
    PCSTR mapPath;

    IAppHostElement *GetElement(IAppHostElement *section, PCWSTR name);
    PCSTR GetString(IAppHostElement *section, PCWSTR name);
public:
    CModuleConfiguration();
    ~CModuleConfiguration();

    HRESULT Initialize(IN IHttpContext *pHttpContext, IN IHttpServer *pHttpServer);

    void CleanupStoredContext();

    PCSTR GetBeginRequest() const;
    PCSTR GetAuthenticateRequest() const;
    PCSTR GetAuthorizeRequest() const;
    PCSTR GetLogRequest() const;
    PCSTR GetMapPath() const;
};
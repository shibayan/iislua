#pragma once

_COM_SMARTPTR_TYPEDEF(IAppHostElement, __uuidof(IAppHostElement));
_COM_SMARTPTR_TYPEDEF(IAppHostProperty, __uuidof(IAppHostProperty));

class CModuleConfiguration : public IHttpStoredContext
{
private:
    PCSTR beginRequest;
    PCSTR authenticateRequest;
    PCSTR authorizeRequest;
    PCSTR logRequest;
    PCSTR mapPath;

    IAppHostElementPtr &GetElement(IAppHostElementPtr &section, _bstr_t elementName);
    PCSTR GetString(IAppHostElementPtr &section, _bstr_t propertyName);
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
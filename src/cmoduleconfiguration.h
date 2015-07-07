#pragma once

_COM_SMARTPTR_TYPEDEF(IAppHostElement, __uuidof(IAppHostElement));
_COM_SMARTPTR_TYPEDEF(IAppHostProperty, __uuidof(IAppHostProperty));

class CModuleConfiguration : public IHttpStoredContext
{
private:
    bool enableCodeCache;

    std::string beginRequest;
    std::string authenticateRequest;
    std::string authorizeRequest;
    std::string executeRequest;
    std::string logRequest;
    std::string endRequest;
    std::string mapPath;

    IAppHostElementPtr GetElement(IAppHostElementPtr &section, _bstr_t elementName);
    std::string GetString(IAppHostElementPtr &section, _bstr_t propertyName);
public:
    HRESULT Initialize(IN IHttpContext *pHttpContext, IN IHttpServer *pHttpServer);

    void CleanupStoredContext();

    inline bool GetEnableCodeCache() const
    {
        return enableCodeCache;
    };

    inline const std::string& GetBeginRequest() const
    {
        return beginRequest;
    };
    inline const std::string& GetAuthenticateRequest() const
    {
        return authenticateRequest;
    };
    inline const std::string& GetAuthorizeRequest() const
    {
        return authorizeRequest;
    };
    inline const std::string& GetExecuteRequest() const
    {
        return executeRequest;
    };
    inline const std::string& GetLogRequest() const
    {
        return logRequest;
    };
    inline const std::string& GetEndRequest() const
    {
        return endRequest;
    };
    inline const std::string& GetMapPath() const
    {
        return mapPath;
    };
};

#include "stdafx.h"

static const PCWSTR sectionPath = L"system.webServer/iislua";

CModuleConfiguration::CModuleConfiguration()
    : beginRequest(NULL), authenticateRequest(NULL), authorizeRequest(NULL), logRequest(NULL), mapPath(NULL)
{
}

CModuleConfiguration::~CModuleConfiguration()
{
    if (beginRequest != NULL)
    {
        delete [] beginRequest;
        beginRequest = NULL;
    }

    if (authenticateRequest != NULL)
    {
        delete [] authenticateRequest;
        authenticateRequest = NULL;
    }

    if (authorizeRequest != NULL)
    {
        delete [] authorizeRequest;
        authorizeRequest = NULL;
    }

    if (logRequest != NULL)
    {
        delete [] logRequest;
        logRequest = NULL;
    }

    if (mapPath != NULL)
    {
        delete [] mapPath;
        mapPath = NULL;
    }
}

HRESULT CModuleConfiguration::Initialize(IN IHttpContext *pHttpContext, IN IHttpServer *pHttpServer)
{
    // Get IAppHostElement
    IAppHostElement *section = NULL;

    auto path = SysAllocString(pHttpContext->GetMetadata()->GetMetaPath());
    auto sectionName = SysAllocString(sectionPath);

    pHttpServer->GetAdminManager()->GetAdminSection(sectionName, path, &section);

    SysFreeString(sectionName);
    SysFreeString(path);

    sectionName = NULL;
    path = NULL;

    if (section == NULL)
    {
        return S_OK;
    }

    // beginRequest element
    auto beginRequest = GetElement(section, L"beginRequest");

    this->beginRequest = GetString(beginRequest, L"scriptPath");

    beginRequest->Release();
    beginRequest = NULL;

    // authenticateRequest element
    auto authenticateRequest = GetElement(section, L"authenticateRequest");

    this->authenticateRequest = GetString(authenticateRequest, L"scriptPath");

    authenticateRequest->Release();
    authenticateRequest = NULL;

    // authorizeRequest element
    auto authorizeRequest = GetElement(section, L"authorizeRequest");

    this->authorizeRequest = GetString(authorizeRequest, L"scriptPath");

    authorizeRequest->Release();
    authorizeRequest = NULL;

    // logRequest element
    auto logRequest = GetElement(section, L"logRequest");

    this->logRequest = GetString(logRequest, L"scriptPath");

    logRequest->Release();
    logRequest = NULL;

    // mapPath element
    auto mapPath = GetElement(section, L"mapPath");

    this->mapPath = GetString(mapPath, L"scriptPath");

    mapPath->Release();
    mapPath = NULL;

    // release
    section->Release();
    section = NULL;

    return S_OK;
}

void CModuleConfiguration::CleanupStoredContext()
{
    delete this;
}

PCSTR CModuleConfiguration::GetBeginRequest() const
{
    return beginRequest;
}

PCSTR CModuleConfiguration::GetAuthenticateRequest() const
{
    return authenticateRequest;
}

PCSTR CModuleConfiguration::GetAuthorizeRequest() const
{
    return authorizeRequest;
}

PCSTR CModuleConfiguration::GetLogRequest() const
{
    return logRequest;
}

PCSTR CModuleConfiguration::GetMapPath() const
{
    return mapPath;
}

IAppHostElement *CModuleConfiguration::GetElement(IAppHostElement *section, PCWSTR name)
{
    IAppHostElement *element = NULL;

    auto elementName = SysAllocString(name);

    section->GetElementByName(elementName, &element);

    SysFreeString(elementName);

    elementName = NULL;

    return element;
}

PCSTR CModuleConfiguration::GetString(IAppHostElement *section, PCWSTR name)
{
    IAppHostProperty *prop = NULL;
    BSTR propertyValue = NULL;

    auto propertyName = SysAllocString(name);

    section->GetPropertyByName(propertyName, &prop);

    prop->get_StringValue(&propertyValue);

    auto value = iis_lua_wstr_to_str(propertyValue);

    SysFreeString(propertyValue);
    SysFreeString(propertyName);

    prop->Release();

    return value;
}
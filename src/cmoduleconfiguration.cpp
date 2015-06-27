
#include "stdafx.h"

static const _bstr_t sectionPath = L"system.webServer/iislua";

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
    IAppHostElementPtr section;

    _bstr_t path = pHttpContext->GetMetadata()->GetMetaPath();

    pHttpServer->GetAdminManager()->GetAdminSection(sectionPath, path, &section);

    if (section == NULL)
    {
        return S_OK;
    }

    // beginRequest element
    auto beginRequestElement = GetElement(section, L"beginRequest");
    this->beginRequest = GetString(beginRequestElement, L"scriptPath");

    // authenticateRequest element
    auto authenticateRequestElement = GetElement(section, L"authenticateRequest");
    this->authenticateRequest = GetString(authenticateRequestElement, L"scriptPath");

    // authorizeRequest element
    auto authorizeRequestElement = GetElement(section, L"authorizeRequest");
    this->authorizeRequest = GetString(authorizeRequestElement, L"scriptPath");

    // logRequest element
    auto logRequestElement = GetElement(section, L"logRequest");
    this->logRequest = GetString(logRequestElement, L"scriptPath");

    // mapPath element
    auto mapPathElement = GetElement(section, L"mapPath");
    this->mapPath = GetString(mapPathElement, L"scriptPath");

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

IAppHostElementPtr &CModuleConfiguration::GetElement(IAppHostElementPtr &section, _bstr_t elementName)
{
    IAppHostElementPtr element;

    section->GetElementByName(elementName, &element);

    return element;
}

PCSTR CModuleConfiguration::GetString(IAppHostElementPtr &section, _bstr_t propertyName)
{
    IAppHostPropertyPtr prop;

    section->GetPropertyByName(propertyName, &prop);

    BSTR propertyValue = NULL;

    prop->get_StringValue(&propertyValue);

    auto value = iis_lua_wstr_to_str(propertyValue);

    SysFreeString(propertyValue);

    return value;
}
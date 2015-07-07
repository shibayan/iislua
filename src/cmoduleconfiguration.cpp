
#include "stdafx.h"

static const _bstr_t sectionPath = L"system.webServer/iislua";

HRESULT CModuleConfiguration::Initialize(IN IHttpContext *pHttpContext, IN IHttpServer *pHttpServer)
{
    IAppHostElementPtr section;

    _bstr_t path = pHttpContext->GetMetadata()->GetMetaPath();

    pHttpServer->GetAdminManager()->GetAdminSection(sectionPath, path, &section);

    if (section == NULL)
    {
        return S_OK;
    }

    // iislua element
    this->enableCodeCache = false;

    // beginRequest element
    auto beginRequestElement = GetElement(section, L"beginRequest");
    this->beginRequest = GetString(beginRequestElement, L"scriptPath");

    // authenticateRequest element
    auto authenticateRequestElement = GetElement(section, L"authenticateRequest");
    this->authenticateRequest = GetString(authenticateRequestElement, L"scriptPath");

    // authorizeRequest element
    auto authorizeRequestElement = GetElement(section, L"authorizeRequest");
    this->authorizeRequest = GetString(authorizeRequestElement, L"scriptPath");

    // executeRequest element
    auto executeRequestElement = GetElement(section, L"executeRequest");
    this->executeRequest = GetString(executeRequestElement, L"scriptPath");

    // logRequest element
    auto logRequestElement = GetElement(section, L"logRequest");
    this->logRequest = GetString(logRequestElement, L"scriptPath");

    // endRequest element
    auto endRequestElement = GetElement(section, L"endRequest");
    this->endRequest = GetString(endRequestElement, L"scriptPath");

    // mapPath element
    auto mapPathElement = GetElement(section, L"mapPath");
    this->mapPath = GetString(mapPathElement, L"scriptPath");

    return S_OK;
}

void CModuleConfiguration::CleanupStoredContext()
{
    delete this;
}

IAppHostElementPtr CModuleConfiguration::GetElement(IAppHostElementPtr &section, _bstr_t elementName)
{
    IAppHostElementPtr element;

    section->GetElementByName(elementName, &element);

    return element;
}

std::string CModuleConfiguration::GetString(IAppHostElementPtr &section, _bstr_t propertyName)
{
    IAppHostPropertyPtr property;

    section->GetPropertyByName(propertyName, &property);

    _bstr_t propertyValue;

    property->get_StringValue(&propertyValue.GetBSTR());

    return iis_lua_wstr_to_str(propertyValue);
}
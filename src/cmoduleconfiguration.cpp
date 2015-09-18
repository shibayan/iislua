
#include "stdafx.h"

extern HTTP_MODULE_ID g_pModuleContext;
extern IHttpServer *g_pHttpServer;

static const _bstr_t sectionPath = L"system.webServer/iislua";

HRESULT CModuleConfiguration::Initialize(IN IHttpContext *pHttpContext, IN IHttpServer *pHttpServer)
{
    IAppHostElementPtr section;

    _bstr_t path = pHttpContext->GetMetadata()->GetMetaPath();

    pHttpServer->GetAdminManager()->GetAdminSection(sectionPath, path, &section);

    if (!section)
    {
        return S_OK;
    }

    // iislua element
    this->enableCodeCache = GetBoolean(section, L"enableCodeCache");

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

    // root lua state
    this->L = iislua_newstate();

    return S_OK;
}

void CModuleConfiguration::CleanupStoredContext()
{
    iislua_close(this->L);

    delete this;
}

CModuleConfiguration *CModuleConfiguration::GetContext(IN IHttpContext *pHttpContext)
{
    auto pModuleContextContainer = pHttpContext->GetMetadata()->GetModuleContextContainer();
    auto pModuleConfig = reinterpret_cast<CModuleConfiguration *>(pModuleContextContainer->GetModuleContext(g_pModuleContext));

    if (pModuleConfig)
    {
        return pModuleConfig;
    }

    pModuleConfig = new CModuleConfiguration();

    if (FAILED(pModuleConfig->Initialize(pHttpContext, g_pHttpServer)))
    {
        pModuleConfig->CleanupStoredContext();

        return nullptr;
    }

    if (FAILED(pModuleContextContainer->SetModuleContext(pModuleConfig, g_pModuleContext)))
    {
        pModuleConfig->CleanupStoredContext();

        return nullptr;
    }

    return pModuleConfig;
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

    return iislua_to_str(propertyValue);
}

bool CModuleConfiguration::GetBoolean(IAppHostElementPtr &section, _bstr_t propertyName)
{
    IAppHostPropertyPtr property;

    section->GetPropertyByName(propertyName, &property);

    _variant_t propertyValue;

    property->get_Value(&propertyValue.GetVARIANT());

    return propertyValue;
}
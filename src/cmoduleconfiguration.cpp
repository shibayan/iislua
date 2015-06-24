
#include "stdafx.h"

static const PCWSTR sectionPath = L"system.webServer/iislua";

CModuleConfiguration::CModuleConfiguration()
{
}

CModuleConfiguration::~CModuleConfiguration()
{
    if (beginRequest != NULL)
    {
        delete [] beginRequest;
        beginRequest = NULL;
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
    IAppHostElement *section;

    auto path = SysAllocString(pHttpContext->GetMetadata()->GetMetaPath());
    auto elementName = SysAllocString(sectionPath);

    pHttpServer->GetAdminManager()->GetAdminSection(elementName, path, &section);

    SysFreeString(elementName);
    SysFreeString(path);

    // Get Configuration
    beginRequest = GetString(section, L"beginRequest");
    mapPath = GetString(section, L"mapPath");

    section->Release();

    return S_OK;
}

VOID CModuleConfiguration::CleanupStoredContext(VOID)
{
    delete this;
}

PCSTR CModuleConfiguration::GetBeginRequest() const
{
    return beginRequest;
}

PCSTR CModuleConfiguration::GetMapPath() const
{
    return mapPath;
}

PCSTR CModuleConfiguration::GetString(IAppHostElement *section, PCWSTR name)
{
    IAppHostProperty *prop;
    BSTR propertyValue;

    auto propertyName = SysAllocString(name);

    section->GetPropertyByName(propertyName, &prop);

    prop->get_StringValue(&propertyValue);

    auto value = iis_lua_wstr_to_str(propertyValue);

    SysFreeString(propertyValue);
    SysFreeString(propertyName);

    prop->Release();

    return value;
}
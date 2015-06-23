
#include "stdafx.h"

HRESULT CModuleConfiguration::Initialize(IN IHttpContext *pHttpContext, IN IHttpServer *pHttpServer)
{
    // Get IAppHostElement
    IAppHostElement *section;

    auto path = SysAllocString(pHttpContext->GetMetadata()->GetMetaPath());
    auto elementName = SysAllocString(L"system.webServer/iislua");

    pHttpServer->GetAdminManager()->GetAdminSection(elementName, path, &section);

    SysFreeString(elementName);
    SysFreeString(path);

    // Get IAppHostProperty
    IAppHostProperty *prop;
    BSTR propertyValue;

    auto propertyName = SysAllocString(L"beginRequest");

    section->GetPropertyByName(propertyName, &prop);

    prop->get_StringValue(&propertyValue);

    // wchar_t to char
    size_t i;
    wcstombs_s(&i, beginRequest, propertyValue, MAX_PATH);

    SysFreeString(propertyValue);
    SysFreeString(propertyName);

    // Release
    prop->Release();

    section->Release();

    return S_OK;
}

VOID CModuleConfiguration::CleanupStoredContext(VOID)
{
    delete this;
}

const char * CModuleConfiguration::GetBeginRequest() const
{
    return beginRequest;
}
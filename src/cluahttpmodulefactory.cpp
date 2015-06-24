
#include "stdafx.h"

HRESULT CLuaHttpModuleFactory::GetHttpModule(OUT CHttpModule **ppModule, IN IModuleAllocator *pAllocator)
{
    UNREFERENCED_PARAMETER(pAllocator);

    auto pModule = new CLuaHttpModule();

    if (!pModule)
    {
        return HRESULT_FROM_WIN32(ERROR_NOT_ENOUGH_MEMORY);
    }

    *ppModule = pModule;

    return S_OK;
}

void CLuaHttpModuleFactory::Terminate()
{
    delete this;
}
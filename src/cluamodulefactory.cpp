
#include "stdafx.h"

HRESULT CLuaModuleFactory::GetHttpModule(OUT CHttpModule **ppModule, IN IModuleAllocator *pAllocator)
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

void CLuaModuleFactory::Terminate()
{
    delete this;
}
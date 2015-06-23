
#include "stdafx.h"

HRESULT CLuaModuleFactory::GetHttpModule(OUT CHttpModule **ppModule, IN IModuleAllocator *pAllocator)
{
    UNREFERENCED_PARAMETER(pAllocator);

    *ppModule = new CLuaHttpModule();

    return S_OK;
}

void CLuaModuleFactory::Terminate()
{
    delete this;
}

#include "stdafx.h"

CLuaHttpModuleFactory::CLuaHttpModuleFactory()
    : pLuaStatePool(new CLuaStatePool())
{
    WSADATA wsaData;

    WSAStartup(MAKEWORD(2, 2), &wsaData);
}

CLuaHttpModuleFactory::~CLuaHttpModuleFactory()
{
    if (pLuaStatePool != nullptr)
    {
        delete pLuaStatePool;
        pLuaStatePool = nullptr;
    }

    WSACleanup();
}

HRESULT CLuaHttpModuleFactory::GetHttpModule(OUT CHttpModule **ppModule, IN IModuleAllocator *pAllocator)
{
    UNREFERENCED_PARAMETER(pAllocator);

    auto pModule = new CLuaHttpModule(pLuaStatePool);

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
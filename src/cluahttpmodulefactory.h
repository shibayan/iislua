#pragma once

class CLuaHttpModuleFactory : public IHttpModuleFactory
{
public:
    HRESULT GetHttpModule(OUT CHttpModule **ppModule, IN IModuleAllocator *pAllocator);
    void Terminate();
};
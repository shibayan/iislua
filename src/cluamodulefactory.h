#pragma once

class IHttpModuleFactory;

class CLuaModuleFactory : public IHttpModuleFactory
{
public:
    HRESULT GetHttpModule(OUT CHttpModule **ppModule, IN IModuleAllocator *pAllocator);
    void Terminate();
};
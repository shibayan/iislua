#pragma once

class CLuaStatePool;

class CLuaHttpModuleFactory : public IHttpModuleFactory
{
    CLuaStatePool *pLuaStatePool;
public:
    CLuaHttpModuleFactory();
    ~CLuaHttpModuleFactory();

    HRESULT GetHttpModule(OUT CHttpModule **ppModule, IN IModuleAllocator *pAllocator);
    void Terminate();
};
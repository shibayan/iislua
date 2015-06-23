#pragma once

class IHttpModuleFactory;

class CLuaModuleFactory : public IHttpModuleFactory
{
public:
    virtual HRESULT GetHttpModule(OUT CHttpModule **ppModule, IN IModuleAllocator *pAllocator);
    virtual void Terminate();
};
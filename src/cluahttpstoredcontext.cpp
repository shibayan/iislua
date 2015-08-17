
#include "stdafx.h"

extern HTTP_MODULE_ID g_pModuleContext;

CLuaHttpStoredContext::CLuaHttpStoredContext()
    : pHttpChildContext(nullptr)
{
}

IHttpContext *CLuaHttpStoredContext::GetChildContext() const
{
    return pHttpChildContext;
}

void CLuaHttpStoredContext::SetChildContext(IN IHttpContext *pHttpChildContext)
{
    this->pHttpChildContext = pHttpChildContext;
}

void CLuaHttpStoredContext::CleanupStoredContext()
{
    delete this;
}

CLuaHttpStoredContext *CLuaHttpStoredContext::GetContext(IN IHttpContext *pHttpContext)
{
    auto pModuleContextContainer = pHttpContext->GetModuleContextContainer();
    auto pHttpStoredContext = reinterpret_cast<CLuaHttpStoredContext *>(pModuleContextContainer->GetModuleContext(g_pModuleContext));

    if (!pHttpStoredContext)
    {
        return pHttpStoredContext;
    }

    pHttpStoredContext = new CLuaHttpStoredContext();

    if (FAILED(pModuleContextContainer->SetModuleContext(pHttpStoredContext, g_pModuleContext)))
    {
        pHttpStoredContext->CleanupStoredContext();

        return nullptr;
    }

    return pHttpStoredContext;
}

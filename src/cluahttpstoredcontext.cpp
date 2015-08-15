
#include "stdafx.h"

CLuaHttpStoredContext::CLuaHttpStoredContext()
    : pHttpChildContext(NULL)
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
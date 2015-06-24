
#include "stdafx.h"

CLuaHttpStoredContext::CLuaHttpStoredContext()
    : childContext(NULL)
{
}

IHttpContext *CLuaHttpStoredContext::GetChildContext() const
{
    return childContext;
}

void CLuaHttpStoredContext::SetChildContext(IN IHttpContext *context)
{
    childContext = context;
}

void CLuaHttpStoredContext::CleanupStoredContext()
{
    delete this;
}
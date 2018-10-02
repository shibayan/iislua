
#include "stdafx.h"

CLuaHttpModule::CLuaHttpModule(CLuaStatePool *pLuaStatePool)
	: L(nullptr), pLuaStatePool(pLuaStatePool)
{
}

CLuaHttpModule::~CLuaHttpModule()
{
	if (L != nullptr)
	{
		pLuaStatePool->Release(L);
		L = nullptr;
	}
}

REQUEST_NOTIFICATION_STATUS CLuaHttpModule::OnExecuteCore(IN IHttpContext *pHttpContext, IN IHttpEventProvider *pProvider, IN const char *name)
{
	if (L == nullptr)
	{
		L = pLuaStatePool->Acquire(pHttpContext, pProvider);
	}

	lua_getglobal(L, name);

	if (!lua_isfunction(L, -1))
	{
		lua_pop(L, 1);

		return RQ_NOTIFICATION_CONTINUE;
	}

	if (lua_pcall(L, 0, 1, 0))
	{
		auto error = lua_tostring(L, -1);

		pHttpContext->GetResponse()->SetStatus(500, "Internal Server Error");

		return RQ_NOTIFICATION_FINISH_REQUEST;
	}

	return iislua_finish_request(L);
}

REQUEST_NOTIFICATION_STATUS CLuaHttpModule::OnAsyncCompletion(IN IHttpContext *pHttpContext, IN DWORD dwNotification, IN BOOL fPostNotification, IN IHttpEventProvider *pProvider, IN IHttpCompletionInfo *pCompletionInfo)
{
	UNREFERENCED_PARAMETER(pHttpContext);
	UNREFERENCED_PARAMETER(dwNotification);
	UNREFERENCED_PARAMETER(fPostNotification);
	UNREFERENCED_PARAMETER(pProvider);
	UNREFERENCED_PARAMETER(pCompletionInfo);

	auto storedContext = CLuaHttpStoredContext::GetContext(pHttpContext);

	storedContext->GetChildContext()->ReleaseClonedContext();
	storedContext->SetChildContext(nullptr);

	return RQ_NOTIFICATION_CONTINUE;
}
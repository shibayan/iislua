
#include "stdafx.h"

CLuaStatePool::CLuaStatePool()
    : count(0), maxPoolSize(64), pListHead(nullptr)
{
    pListHead = static_cast<PSLIST_HEADER>(_aligned_malloc(sizeof(SLIST_HEADER), MEMORY_ALLOCATION_ALIGNMENT));

    InitializeSListHead(pListHead);
}

CLuaStatePool::~CLuaStatePool()
{
    while (true)
    {
        auto pListEntry = InterlockedPopEntrySList(pListHead);

        if (pListEntry == nullptr)
        {
            break;
        }

        auto pLuaStateEntry = reinterpret_cast<PLUA_STATE_ENTRY>(pListEntry);

        iislua_close(pLuaStateEntry->L);

        _aligned_free(pListEntry);
    }

    _aligned_free(pListHead);
}

lua_State *CLuaStatePool::Acquire(IHttpContext *pHttpContext, IHttpEventProvider *pProvider)
{
    lua_State *L;

    auto pListEntry = InterlockedPopEntrySList(pListHead);

    if (pListEntry == nullptr)
    {
        L = iislua_newstate();

        // load from file
        auto config = CModuleConfiguration::GetConfig(pHttpContext);

        iislua_load_file(L, "BeginRequest", config->GetBeginRequest().c_str());
        iislua_load_file(L, "AuthenticateRequest", config->GetAuthenticateRequest().c_str());
        iislua_load_file(L, "AuthorizeRequest", config->GetAuthorizeRequest().c_str());
        iislua_load_file(L, "ExecuteRequest", config->GetExecuteRequest().c_str());
        iislua_load_file(L, "LogRequest", config->GetLogRequest().c_str());
        iislua_load_file(L, "EndRequest", config->GetEndRequest().c_str());
        iislua_load_file(L, "MapPath", config->GetMapPath().c_str());

        this->codeCacheEnabled = config->IsCodeCacheEnabled();
    }
    else
    {
        auto pLuaStateEntry = reinterpret_cast<PLUA_STATE_ENTRY>(pListEntry);

        L = pLuaStateEntry->L;

        InterlockedDecrement(&count);

        _aligned_free(pListEntry);
    }

    iislua_init(L, pHttpContext, pProvider);

    return L;
}

void CLuaStatePool::Release(lua_State *L)
{
    if (count >= maxPoolSize || !this->codeCacheEnabled)
    {
        iislua_close(L);

        return;
    }

    auto pLuaStateEntry = static_cast<PLUA_STATE_ENTRY>(_aligned_malloc(sizeof(LUA_STATE_ENTRY), MEMORY_ALLOCATION_ALIGNMENT));

    pLuaStateEntry->L = L;

    InterlockedPushEntrySList(pListHead, &pLuaStateEntry->listEntry);

    InterlockedIncrement(&count);
}
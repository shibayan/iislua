#pragma once

struct lua_State;

class CLuaStatePool
{
    typedef struct _LUA_STATE_ENTRY
    {
        SLIST_ENTRY listEntry;
        lua_State *L;
    } LUA_STATE_ENTRY, *PLUA_STATE_ENTRY;

    PSLIST_HEADER pListHead;
    unsigned int count;
    unsigned int maxPoolSize;
    bool enableCodeCache;
public:
    CLuaStatePool();
    ~CLuaStatePool();

    lua_State *Acquire(IHttpContext *pHttpContext, IHttpEventProvider *pProvider);
    void Release(lua_State *L);
};
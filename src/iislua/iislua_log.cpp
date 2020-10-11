
#include "stdafx.h"

int iislua_log_read_entry(lua_State* L)
{
    auto provider = reinterpret_cast<ISendResponseProvider*>(iislua_get_http_provider(L));

    if (!provider->GetReadyToLogData())
    {
        return 0;
    }

    auto pLogData = reinterpret_cast<PHTTP_LOG_FIELDS_DATA>(provider->GetLogData());

    auto name = luaL_checkstring(L, 1);

    lua_pushstring(L, pLogData->ClientIp);

    return 0;
}

int iislua_log_write_entry(lua_State* L)
{
    auto ctx = iislua_get_http_ctx(L);
    auto provider = reinterpret_cast<ISendResponseProvider*>(iislua_get_http_provider(L));

    if (!provider->GetReadyToLogData())
    {
        return 0;
    }

    auto pLogData = reinterpret_cast<PHTTP_LOG_FIELDS_DATA>(provider->GetLogData());

    auto name = luaL_checkstring(L, 1);
    auto value = luaL_checkstring(L, 2);

    auto size = strlen(value);
    auto newValue = reinterpret_cast<PCHAR>(ctx->AllocateRequestMemory(size + 1));

    strcpy_s(newValue, size + 1, value);

    pLogData->ClientIp = newValue;
    pLogData->ClientIpLength = size;

    provider->SetLogData(reinterpret_cast<PHTTP_LOG_DATA>(pLogData));

    return 0;
}
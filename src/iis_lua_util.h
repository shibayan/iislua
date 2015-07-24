#pragma once

class CLuaHttpStoredContext;
class CModuleConfiguration;

lua_State *iis_lua_newstate();
void iis_lua_close(lua_State *L);

IHttpContext *iis_lua_get_http_ctx(lua_State *L);
void iis_lua_set_http_ctx(lua_State *L, IHttpContext *ctx);

REQUEST_NOTIFICATION_STATUS iis_lua_get_result(lua_State *L);
void iis_lua_set_result(lua_State *L, REQUEST_NOTIFICATION_STATUS result = RQ_NOTIFICATION_FINISH_REQUEST);

CLuaHttpStoredContext *iis_lua_get_stored_context(IHttpContext *pHttpContext);
const CModuleConfiguration *iis_lua_get_config(IHttpContext *pHttpContext);

PCSTR iis_lua_util_get_status_reason(USHORT status);

std::string iis_lua_wstr_to_str(PCWSTR wstr);
std::wstring iis_lua_str_to_wstr(PCSTR str);

bool iis_lua_load_function(lua_State *L, PCSTR name, PCSTR path);
lua_State *iis_lua_newthread(lua_State *L);
void iis_lua_create_cache_table(lua_State *L);
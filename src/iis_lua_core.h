#pragma once

class CLuaHttpStoredContext;
class CModuleConfiguration;

#define iis_lua_loadfile(L, scriptPath, enableCodeCache) iis_lua_load_function(L, scriptPath, __FUNCTION__, enableCodeCache)

lua_State *iis_lua_newstate();
void iis_lua_close(lua_State *L);

void iis_lua_create_cachetable(lua_State *L);
bool iis_lua_load_function(lua_State *L, PCSTR scriptPath);
bool iis_lua_load_function(lua_State *L, PCSTR scriptPath, PCSTR cacheKey, bool enableCodeCache);

lua_State *iis_lua_newthread(lua_State *L);
void iis_lua_set_sandbox(lua_State *root, lua_State *L);
void iis_lua_initialize(lua_State *L, IHttpContext *ctx);

REQUEST_NOTIFICATION_STATUS iis_lua_finish_request(lua_State *L);

IHttpContext *iis_lua_get_http_ctx(lua_State *L);
void iis_lua_set_http_ctx(lua_State *L, IHttpContext *ctx);

void iis_lua_set_result(lua_State *L, REQUEST_NOTIFICATION_STATUS result = RQ_NOTIFICATION_FINISH_REQUEST);

CLuaHttpStoredContext *iis_lua_get_stored_context(IHttpContext *pHttpContext);
const CModuleConfiguration *iis_lua_get_config(IHttpContext *pHttpContext);
#pragma once

class CLuaHttpStoredContext;
class CModuleConfiguration;

#define iislua_loadfile(L, scriptPath, enableCodeCache) iislua_load_function(L, scriptPath, __FUNCTION__, enableCodeCache)

lua_State *iislua_newstate();
void iislua_close(lua_State *L);

void iislua_create_cachetable(lua_State *L);
bool iislua_load_function(lua_State *L, PCSTR scriptPath);
bool iislua_load_function(lua_State *L, PCSTR scriptPath, PCSTR cacheKey, bool enableCodeCache);

lua_State *iislua_newthread(lua_State *L);
void iislua_set_sandbox(lua_State *root, lua_State *L);
void iislua_initialize(lua_State *L, IHttpContext *ctx);

REQUEST_NOTIFICATION_STATUS iislua_finish_request(lua_State *L);

IHttpContext *iislua_get_http_ctx(lua_State *L);
void iislua_set_http_ctx(lua_State *L, IHttpContext *ctx);

void iislua_set_result(lua_State *L, REQUEST_NOTIFICATION_STATUS result = RQ_NOTIFICATION_FINISH_REQUEST);
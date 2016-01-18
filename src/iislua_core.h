#pragma once

class CLuaHttpStoredContext;
class CModuleConfiguration;

lua_State *iislua_newstate();
void iislua_close(lua_State *L);

void iislua_load_file(lua_State *L, const char *name, const char *file);

void iislua_init(lua_State *L, IHttpContext *ctx);
REQUEST_NOTIFICATION_STATUS iislua_finish_request(lua_State *L);

IHttpContext *iislua_get_http_ctx(lua_State *L);
void iislua_set_http_ctx(lua_State *L, IHttpContext *ctx);

void iislua_set_result(lua_State *L, REQUEST_NOTIFICATION_STATUS result = RQ_NOTIFICATION_FINISH_REQUEST);
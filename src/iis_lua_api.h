#pragma once

struct lua_State;
class IHttpContext;
enum REQUEST_NOTIFICATION_STATUS;

#define IIS_LUA_API extern "C"

IIS_LUA_API inline lua_State *iis_lua_newstate();

IIS_LUA_API inline IHttpContext *iis_lua_get_http_ctx(lua_State *);
IIS_LUA_API inline void iis_lua_set_http_ctx(lua_State *, IHttpContext *);

IIS_LUA_API inline REQUEST_NOTIFICATION_STATUS iis_lua_get_result(lua_State *);
IIS_LUA_API inline void iis_lua_set_result(lua_State *, REQUEST_NOTIFICATION_STATUS);

IIS_LUA_API int iis_lua_debug(lua_State *);
IIS_LUA_API int iis_lua_exec(lua_State *);
IIS_LUA_API int iis_lua_exit(lua_State *);
IIS_LUA_API int iis_lua_flush(lua_State *);
IIS_LUA_API int iis_lua_headers_sent(lua_State *);
IIS_LUA_API int iis_lua_map_path(lua_State *);
IIS_LUA_API int iis_lua_print(lua_State *);
IIS_LUA_API int iis_lua_redirect(lua_State *);

IIS_LUA_API int iis_lua_req_get_headers(lua_State *);
IIS_LUA_API int iis_lua_req_get_method(lua_State *);
IIS_LUA_API int iis_lua_req_get_remote_addr(lua_State *);
IIS_LUA_API int iis_lua_req_get_url(lua_State *);
IIS_LUA_API int iis_lua_req_get_url_args(lua_State *);
IIS_LUA_API int iis_lua_req_http_version(lua_State *);
IIS_LUA_API int iis_lua_req_set_header(lua_State *);
IIS_LUA_API int iis_lua_req_set_method(lua_State *);
IIS_LUA_API int iis_lua_req_set_url(lua_State *);

IIS_LUA_API int iis_lua_resp_clear(lua_State *);
IIS_LUA_API int iis_lua_resp_clear_headers(lua_State *);
IIS_LUA_API int iis_lua_resp_get_headers(lua_State *);
IIS_LUA_API int iis_lua_resp_get_status(lua_State *);
IIS_LUA_API int iis_lua_resp_set_header(lua_State *);
IIS_LUA_API int iis_lua_resp_set_status(lua_State *);

IIS_LUA_API int iis_lua_srv_get_variable(lua_State *);
IIS_LUA_API int iis_lua_srv_set_variable(lua_State *);
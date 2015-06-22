
#pragma once

struct lua_State;
class IHttpContext;

#define IIS_LUA_API extern "C"

#define iis_http_ctx_key "__iis_http_ctx"

IIS_LUA_API inline IHttpContext *iis_get_http_ctx(lua_State *);
IIS_LUA_API inline void iis_set_http_ctx(lua_State *, IHttpContext *);

IIS_LUA_API int iis_http_exit(lua_State *);
IIS_LUA_API int iis_http_headers_sent(lua_State *);
IIS_LUA_API int iis_http_map_path(lua_State *);
IIS_LUA_API int iis_http_print(lua_State *);
IIS_LUA_API int iis_http_redirect(lua_State *);

IIS_LUA_API int iis_http_req_get_headers(lua_State *);
IIS_LUA_API int iis_http_req_get_method(lua_State *);
IIS_LUA_API int iis_http_req_http_version(lua_State *);
IIS_LUA_API int iis_http_req_set_method(lua_State *);
IIS_LUA_API int iis_http_req_set_url(lua_State *);

IIS_LUA_API int iis_http_resp_get_headers(lua_State *);
IIS_LUA_API int iis_http_resp_set_status(lua_State *);
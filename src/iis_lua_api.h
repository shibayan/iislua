#pragma once

struct lua_State;
class IHttpContext;

#define IIS_LUA_API extern "C"

IIS_LUA_API inline IHttpContext *iis_lua_get_http_ctx(lua_State *);
IIS_LUA_API inline void iis_lua_set_http_ctx(lua_State *, IHttpContext *);

IIS_LUA_API inline BOOL iis_lua_get_handled(lua_State *);
IIS_LUA_API inline void iis_lua_set_handled(lua_State *, BOOL);

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
IIS_LUA_API int iis_lua_req_http_version(lua_State *);
IIS_LUA_API int iis_lua_req_set_header(lua_State *);
IIS_LUA_API int iis_lua_req_set_method(lua_State *);
IIS_LUA_API int iis_lua_req_set_url(lua_State *);

IIS_LUA_API int iis_lua_resp_get_headers(lua_State *);
IIS_LUA_API int iis_lua_resp_get_status(lua_State *);
IIS_LUA_API int iis_lua_resp_set_header(lua_State *);
IIS_LUA_API int iis_lua_resp_set_status(lua_State *);

IIS_LUA_API int iis_lua_server_get_variables(lua_State *);

const struct luaL_Reg iis [] =
{
    { "debug", iis_lua_debug },
    { "exec", iis_lua_exec },
    { "exit", iis_lua_exit },
    { "flush", iis_lua_flush },
    { "headers_sent", iis_lua_headers_sent },
    { "map_path", iis_lua_map_path },
    { "print", iis_lua_print },
    { "redirect", iis_lua_redirect },
    { NULL, NULL }
};

const struct luaL_Reg iis_req [] =
{
    { "get_headers", iis_lua_req_get_headers },
    { "get_method", iis_lua_req_get_method },
    { "http_version", iis_lua_req_http_version },
    { "set_header", iis_lua_req_set_header },
    { "set_method", iis_lua_req_set_method },
    { "set_url", iis_lua_req_set_url },
    { NULL, NULL }
};

const struct luaL_Reg iis_resp [] =
{
    { "get_headers", iis_lua_resp_get_headers },
    { "get_status", iis_lua_resp_get_status },
    { "set_header", iis_lua_resp_set_header },
    { "set_status", iis_lua_resp_set_status },
    { NULL, NULL }
};
#pragma once

#define IISLUA_API extern "C"

// iis module
IISLUA_API int iis_lua_debug(lua_State *L);
IISLUA_API int iis_lua_exec(lua_State *L);
IISLUA_API int iis_lua_exit(lua_State *L);
IISLUA_API int iis_lua_flush(lua_State *L);
IISLUA_API int iis_lua_headers_sent(lua_State *L);
IISLUA_API int iis_lua_map_path(lua_State *L);
IISLUA_API int iis_lua_print(lua_State *L);
IISLUA_API int iis_lua_redirect(lua_State *L);
IISLUA_API int iis_lua_say(lua_State *L);

// iis.req module
IISLUA_API int iis_lua_req_get_headers(lua_State *L);
IISLUA_API int iis_lua_req_get_method(lua_State *L);
IISLUA_API int iis_lua_req_get_remote_addr(lua_State *L);
IISLUA_API int iis_lua_req_get_url(lua_State *L);
IISLUA_API int iis_lua_req_get_url_args(lua_State *L);
IISLUA_API int iis_lua_req_http_version(lua_State *L);
IISLUA_API int iis_lua_req_set_header(lua_State *L);
IISLUA_API int iis_lua_req_set_method(lua_State *L);
IISLUA_API int iis_lua_req_set_url(lua_State *L);

// iis.resp module
IISLUA_API int iis_lua_resp_clear(lua_State *L);
IISLUA_API int iis_lua_resp_clear_headers(lua_State *L);
IISLUA_API int iis_lua_resp_get_headers(lua_State *L);
IISLUA_API int iis_lua_resp_get_status(lua_State *L);
IISLUA_API int iis_lua_resp_set_header(lua_State *L);
IISLUA_API int iis_lua_resp_set_status(lua_State *L);

// iis.srv module
IISLUA_API int iis_lua_srv_get_variable(lua_State *L);
IISLUA_API int iis_lua_srv_set_variable(lua_State *L);

// iis.user module
IISLUA_API int iis_lua_user_get_name(lua_State *L);
IISLUA_API int iis_lua_user_get_type(lua_State *L);
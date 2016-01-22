#pragma once

IISLUA_API int iislua_req_get_headers(lua_State *L);
IISLUA_API int iislua_req_get_method(lua_State *L);
IISLUA_API int iislua_req_get_post_args(lua_State *L);
IISLUA_API int iislua_req_get_remote_addr(lua_State *L);
IISLUA_API int iislua_req_get_url(lua_State *L);
IISLUA_API int iislua_req_get_url_args(lua_State *L);
IISLUA_API int iislua_req_http_version(lua_State *L);
IISLUA_API int iislua_req_set_header(lua_State *L);
IISLUA_API int iislua_req_set_method(lua_State *L);
IISLUA_API int iislua_req_set_url(lua_State *L);
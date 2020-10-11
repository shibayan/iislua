#pragma once

IISLUA_API int iislua_resp_clear(lua_State* L);
IISLUA_API int iislua_resp_clear_headers(lua_State* L);
IISLUA_API int iislua_resp_get_headers(lua_State* L);
IISLUA_API int iislua_resp_get_status(lua_State* L);
IISLUA_API int iislua_resp_set_header(lua_State* L);
IISLUA_API int iislua_resp_set_status(lua_State* L);
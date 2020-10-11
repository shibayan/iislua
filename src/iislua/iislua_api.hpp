#pragma once

IISLUA_API int iislua_debug(lua_State* L);
IISLUA_API int iislua_exec(lua_State* L);
IISLUA_API int iislua_exit(lua_State* L);
IISLUA_API int iislua_flush(lua_State* L);
IISLUA_API int iislua_headers_sent(lua_State* L);
IISLUA_API int iislua_map_path(lua_State* L);
IISLUA_API int iislua_print(lua_State* L);
IISLUA_API int iislua_redirect(lua_State* L);
IISLUA_API int iislua_say(lua_State* L);
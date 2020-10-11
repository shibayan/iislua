#pragma once

IISLUA_API int iislua_socket_tcp(lua_State* L);
IISLUA_API int iislua_socket_tcp_connect(lua_State* L);
IISLUA_API int iislua_socket_tcp_send(lua_State* L);
IISLUA_API int iislua_socket_tcp_receive(lua_State* L);
IISLUA_API int iislua_socket_tcp_close(lua_State* L);
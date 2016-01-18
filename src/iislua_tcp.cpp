
#include "stdafx.h"

struct socket_data
{
    SOCKET sock;
    bool connected;
    bool closed;
};

char iislua_socket_tcp_metatable;

int iislua_socket_tcp_release(lua_State *L)
{
    auto data = reinterpret_cast<socket_data *>(lua_touserdata(L, -1));

    if (!data->closed)
    {
        closesocket(data->sock);
    }

    return 0;
}

int iislua_socket_tcp(lua_State *L)
{
    // create new object
    lua_createtable(L, 0, 0);
    lua_pushlightuserdata(L, &iislua_socket_tcp_metatable);
    lua_rawget(L, LUA_REGISTRYINDEX);
    lua_setmetatable(L, -2);

    return 1;
}

int iislua_socket_tcp_connect(lua_State *L)
{
    if (lua_gettop(L) != 3)
    {
        return luaL_error(L, "argument error");
    }

    auto ctx = iislua_get_http_ctx(L);

    if (ctx == NULL)
    {
        return luaL_error(L, "context is null");
    }

    auto config = CModuleConfiguration::GetConfig(ctx);

    luaL_checktype(L, 1, LUA_TTABLE);

    auto host = luaL_checkstring(L, 2);
    auto port = luaL_checkinteger(L, 3);

    auto sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // create socket_data
    auto data = reinterpret_cast<socket_data *>(lua_newuserdata(L, sizeof(socket_data)));

    data->sock = sock;
    data->connected = false;
    data->closed = false;

    lua_createtable(L, 0, 1);

    lua_pushcfunction(L, iislua_socket_tcp_release);
    lua_setfield(L, -2, "__gc");

    lua_setmetatable(L, -2);

    lua_rawseti(L, 1, 1);

    // winsock2 connect
    auto h = WSACreateEvent();

    WSAEventSelect(sock, h, FD_CONNECT);

    ADDRINFO hints;
    LPADDRINFO list;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_family = AF_INET;

    char service[6];
    sprintf_s(service, "%d", (int)port);

    if (getaddrinfo(host, service, &hints, &list) != 0)
    {
        return 0;
    }

    connect(sock, list->ai_addr, (int)list->ai_addrlen);

    if (WSAWaitForMultipleEvents(1, &h, FALSE, config->GetConnectTimeout(), FALSE) == WSA_WAIT_EVENT_0)
    {
        WSANETWORKEVENTS events;

        WSAEnumNetworkEvents(sock, h, &events);

        if ((events.lNetworkEvents & FD_CONNECT) && events.iErrorCode[FD_CONNECT_BIT] == 0)
        {
            data->connected = true;
        }
    }

    WSAEventSelect(sock, NULL, 0);
    WSACloseEvent(h);

    return 0;
}

int iislua_socket_tcp_send(lua_State *L)
{
    if (lua_gettop(L) != 2)
    {
        return luaL_error(L, "argument error");
    }

    auto ctx = iislua_get_http_ctx(L);

    if (ctx == NULL)
    {
        return luaL_error(L, "context is null");
    }

    auto config = CModuleConfiguration::GetConfig(ctx);

    luaL_checktype(L, 1, LUA_TTABLE);

    auto sendData = luaL_checkstring(L, 2);

    lua_rawgeti(L, 1, 1);
    auto data = reinterpret_cast<socket_data *>(lua_touserdata(L, -1));
    lua_pop(L, 1);

    auto h = WSACreateEvent();

    WSAEventSelect(data->sock, h, FD_WRITE | FD_CLOSE);

    if (WSAWaitForMultipleEvents(1, &h, FALSE, config->GetSendTimeout(), FALSE) != WSA_WAIT_FAILED)
    {
        WSANETWORKEVENTS events;

        WSAEnumNetworkEvents(data->sock, h, &events);

        if (events.lNetworkEvents & FD_CLOSE)
        {
            closesocket(data->sock);
            data->closed = true;
        }
        else if (events.lNetworkEvents & FD_WRITE)
        {
            send(data->sock, sendData, (int)strlen(sendData), 0);
        }
    }

    WSAEventSelect(data->sock, NULL, 0);
    WSACloseEvent(h);

    return 0;
}

int iislua_socket_tcp_receive(lua_State *L)
{
    if (lua_gettop(L) != 2)
    {
        return luaL_error(L, "argument error");
    }

    auto ctx = iislua_get_http_ctx(L);

    if (ctx == NULL)
    {
        return luaL_error(L, "context is null");
    }

    auto config = CModuleConfiguration::GetConfig(ctx);

    luaL_checktype(L, 1, LUA_TTABLE);

    auto size = luaL_checkinteger(L, 2);

    lua_rawgeti(L, 1, 1);
    auto data = reinterpret_cast<socket_data *>(lua_touserdata(L, -1));
    lua_pop(L, 1);

    auto h = WSACreateEvent();

    WSAEventSelect(data->sock, h, FD_READ | FD_CLOSE);

    if (WSAWaitForMultipleEvents(1, &h, FALSE, config->GetReadTimeout(), FALSE) != WSA_WAIT_FAILED)
    {
        WSANETWORKEVENTS events;

        WSAEnumNetworkEvents(data->sock, h, &events);

        if (events.lNetworkEvents & FD_CLOSE)
        {
            closesocket(data->sock);
            data->closed = true;

            lua_pushnil(L);
        }
        else if (events.lNetworkEvents & FD_READ)
        {
            std::vector<char> buffer(size);

            int len = recv(data->sock, &buffer[0], (int)size, 0);

            lua_pushlstring(L, &buffer[0], len);
        }
    }
    else
    {
        lua_pushnil(L);
    }

    WSAEventSelect(data->sock, NULL, 0);
    WSACloseEvent(h);

    return 1;
}

int iislua_socket_tcp_close(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);

    lua_rawgeti(L, 1, 1);

    auto data = reinterpret_cast<socket_data *>(lua_touserdata(L, -1));

    lua_pop(L, 1);

    if (!data->closed)
    {
        closesocket(data->sock);
    }

    return 0;
}
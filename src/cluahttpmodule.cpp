
#include "stdafx.h"

CLuaHttpModule::CLuaHttpModule()
{
    L = luaL_newstate();

    luaL_openlibs(L);

    luaL_newlib(L, iis);

    // create iis.req
    luaL_newlib(L, iis_req);
    lua_setfield(L, -2, "req");

    // create iis.resp
    luaL_newlib(L, iis_resp);
    lua_setfield(L, -2, "resp");

    // register iis
    lua_setglobal(L, "iis");
}

CLuaHttpModule::~CLuaHttpModule()
{
    lua_close(L);
}

REQUEST_NOTIFICATION_STATUS CLuaHttpModule::OnBeginRequest(IN IHttpContext *pHttpContext, IN OUT IHttpEventProvider *pProvider)
{
    UNREFERENCED_PARAMETER(pProvider);

    auto config = iis_lua_get_config(pHttpContext);

    iis_lua_set_http_ctx(L, pHttpContext);
    iis_lua_set_handled(L, FALSE);

    if (luaL_dofile(L, config->GetBeginRequest()))
    {
        auto text = lua_tostring(L, -1);

        OutputDebugString(text);
    }

    if (iis_lua_get_handled(L))
    {
        return RQ_NOTIFICATION_FINISH_REQUEST;
    }

    return RQ_NOTIFICATION_CONTINUE;
}
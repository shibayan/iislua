// iislua.cpp : DLL アプリケーション用にエクスポートされる関数を定義します。
//

#include "stdafx.h"


static const struct luaL_Reg iis [] =
{
    { "debug", iis_lua_debug },
    { "exit", iis_lua_exit },
    { "headers_sent", iis_lua_headers_sent },
    { "map_path", iis_lua_map_path },
    { "print", iis_lua_print },
    { "redirect", iis_lua_redirect },
    { NULL, NULL }
};

static const struct luaL_Reg iis_req [] =
{
    { "get_headers", iis_lua_req_get_headers },
    { "get_method", iis_lua_req_get_method },
    { "http_version", iis_lua_req_http_version },
    { "set_method", iis_lua_req_set_method },
    { "set_url", iis_lua_req_set_url },
    { NULL, NULL }
};

static const struct luaL_Reg iis_resp [] =
{
    { "get_headers", iis_lua_resp_get_headers },
    { "set_status", iis_lua_resp_set_status },
    { NULL, NULL }
};

class CLuaHttpModule : public CHttpModule
{
private:
    lua_State *L;

public:
    CLuaHttpModule()
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

        // TODO: load scriptpath from configuration file (ex. web.config)
        if (luaL_dofile(L, "C:\\inetpub\\script.lua"))
        {
            auto text = lua_tostring(L, -1);

            OutputDebugStringA(text);
        }
    }

    ~CLuaHttpModule()
    {
        lua_close(L);
    }

    REQUEST_NOTIFICATION_STATUS OnBeginRequest(IN IHttpContext *pHttpContext, IN OUT IHttpEventProvider *pProvider)
    {
        iis_lua_set_http_ctx(L, pHttpContext);
        iis_lua_set_handled(L, FALSE);

        lua_getglobal(L, "onBeginRequest");

        if (lua_pcall(L, 0, 0, 0) != 0)
        {
            auto text = lua_tostring(L, -1);

            OutputDebugStringA(text);
        }

        if (iis_lua_get_handled(L))
        {
            return RQ_NOTIFICATION_FINISH_REQUEST;
        }

        return RQ_NOTIFICATION_CONTINUE;
    }
};

class CLuaHttpModuleFactory : public IHttpModuleFactory
{
public:
    virtual HRESULT GetHttpModule(OUT CHttpModule **ppModule, IN IModuleAllocator *pAllocator)
    {
        *ppModule = new CLuaHttpModule();

        return S_OK;
    }

    virtual void Terminate()
    {
        delete this;
    }
};

HRESULT WINAPI RegisterModule(DWORD dwServerVersion, IHttpModuleRegistrationInfo *pModuleInfo, IHttpServer *pHttpServer)
{
    return pModuleInfo->SetRequestNotifications(new CLuaHttpModuleFactory(), RQ_BEGIN_REQUEST, 0);
}
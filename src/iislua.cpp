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
    { "set_header", iis_lua_resp_set_header },
    { "set_status", iis_lua_resp_set_status },
    { NULL, NULL }
};

static HTTP_MODULE_ID g_pModuleContext;
static IHttpServer *g_pHttpServer;

class CLuaHttpModuleConfiguration : public IHttpStoredContext
{
private:
    char beginRequest[MAX_PATH];
public:
    HRESULT Initialize(IN IHttpContext *pHttpContext)
    {
        // Get IAppHostElement
        IAppHostElement *section;

        auto path = SysAllocString(pHttpContext->GetMetadata()->GetMetaPath());
        auto elementName = SysAllocString(L"system.webServer/iislua");

        g_pHttpServer->GetAdminManager()->GetAdminSection(elementName, path, &section);

        SysFreeString(elementName);
        SysFreeString(path);

        // Get IAppHostProperty
        IAppHostProperty *prop;
        BSTR propertyValue;

        auto propertyName = SysAllocString(L"beginRequest");

        section->GetPropertyByName(propertyName, &prop);

        prop->get_StringValue(&propertyValue);

        // wchar_t to char
        size_t i;
        wcstombs_s(&i, beginRequest, propertyValue, MAX_PATH);

        SysFreeString(propertyValue);
        SysFreeString(propertyName);

        // Release
        prop->Release();

        section->Release();

        return S_OK;
    }

    VOID CleanupStoredContext(VOID)
    {
        delete this;
    }

    const char *GetBeginRequest() const
    {
        return beginRequest;
    }
};

static CLuaHttpModuleConfiguration *iis_lua_get_config(IHttpContext *pHttpContext)
{
    auto pModuleContextContainer = pHttpContext->GetMetadata()->GetModuleContextContainer();
    auto pModuleConfig = reinterpret_cast<CLuaHttpModuleConfiguration *>(pModuleContextContainer->GetModuleContext(g_pModuleContext));

    if (pModuleConfig != NULL)
    {
        return pModuleConfig;
    }

    pModuleConfig = new CLuaHttpModuleConfiguration();

    if (FAILED(pModuleConfig->Initialize(pHttpContext)))
    {
        pModuleConfig->CleanupStoredContext();

        return NULL;
    }

    if (FAILED(pModuleContextContainer->SetModuleContext(pModuleConfig, g_pModuleContext)))
    {
        pModuleConfig->CleanupStoredContext();

        return NULL;
    }

    return pModuleConfig;
}

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
    }

    ~CLuaHttpModule()
    {
        lua_close(L);
    }

    REQUEST_NOTIFICATION_STATUS OnBeginRequest(IN IHttpContext *pHttpContext, IN OUT IHttpEventProvider *pProvider)
    {
        auto config = iis_lua_get_config(pHttpContext);

        iis_lua_set_http_ctx(L, pHttpContext);
        iis_lua_set_handled(L, FALSE);

        if (luaL_dofile(L, config->GetBeginRequest()))
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
    g_pModuleContext = pModuleInfo->GetId();
    g_pHttpServer = pHttpServer;

    return pModuleInfo->SetRequestNotifications(new CLuaHttpModuleFactory(), RQ_BEGIN_REQUEST, 0);
}
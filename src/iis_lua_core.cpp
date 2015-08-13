
#include "stdafx.h"

extern HTTP_MODULE_ID g_pModuleContext;
extern IHttpServer *g_pHttpServer;

static PCSTR iis_lua_ctx_key = "__iis_lua_ctx";
static PCSTR iis_lua_result_key = "__iis_lua_result";

static char iis_lua_cache_table_key;

static const luaL_Reg iis[] =
{
	{ "debug", iis_lua_debug },
	{ "exec", iis_lua_exec },
	{ "exit", iis_lua_exit },
	{ "flush", iis_lua_flush },
	{ "headers_sent", iis_lua_headers_sent },
	{ "map_path", iis_lua_map_path },
	{ "print", iis_lua_print },
	{ "redirect", iis_lua_redirect },
	{ "say", iis_lua_say },
	{ NULL, NULL }
};

static const luaL_Reg iis_req[] =
{
	{ "get_headers", iis_lua_req_get_headers },
	{ "get_method", iis_lua_req_get_method },
	{ "get_remote_addr", iis_lua_req_get_remote_addr },
	{ "get_url", iis_lua_req_get_url },
	{ "get_url_args", iis_lua_req_get_url_args },
	{ "http_version", iis_lua_req_http_version },
	{ "set_header", iis_lua_req_set_header },
	{ "set_method", iis_lua_req_set_method },
	{ "set_url", iis_lua_req_set_url },
	{ NULL, NULL }
};

static const luaL_Reg iis_resp[] =
{
	{ "clear", iis_lua_resp_clear },
	{ "clear_headers", iis_lua_resp_clear_headers },
	{ "get_headers", iis_lua_resp_get_headers },
	{ "get_status", iis_lua_resp_get_status },
	{ "set_header", iis_lua_resp_set_header },
	{ "set_status", iis_lua_resp_set_status },
	{ NULL, NULL }
};

static const luaL_Reg iis_srv[] =
{
	{ "get_variable", iis_lua_srv_get_variable },
	{ "set_variable", iis_lua_srv_set_variable },
	{ NULL, NULL }
};

static const luaL_Reg iis_user[] =
{
	{ "get_name", iis_lua_user_get_name },
	{ NULL, NULL }
};

lua_State *iis_lua_newstate()
{
	auto L = luaL_newstate();

	luaL_openlibs(L);

	luaL_newlib(L, iis);

	// create iis.req
	luaL_newlib(L, iis_req);
	lua_setfield(L, -2, "req");

	// create iis.resp
	luaL_newlib(L, iis_resp);
	lua_setfield(L, -2, "resp");

	// create iis.srv
	luaL_newlib(L, iis_srv);
	lua_setfield(L, -2, "srv");

	// create iis.user
	luaL_newlib(L, iis_user);
	lua_setfield(L, -2, "user");

	// register iis
	lua_setglobal(L, "iis");

	// create cache table
	iis_lua_create_cache_table(L);

	return L;
}

void iis_lua_close(lua_State *L)
{
	lua_close(L);
}

void iis_lua_create_cache_table(lua_State *L)
{
	lua_pushlightuserdata(L, &iis_lua_cache_table_key);
	lua_createtable(L, 0, 0);
	lua_rawset(L, LUA_REGISTRYINDEX);
}

bool iis_lua_load_function(lua_State *L, PCSTR scriptPath, PCSTR cacheKey, bool enableCodeCache)
{
	// load cache table
	lua_pushlightuserdata(L, &iis_lua_cache_table_key);
	lua_rawget(L, LUA_REGISTRYINDEX);

	if (!lua_istable(L, -1))
	{
		return false;
	}

	// load function from cache
	lua_getfield(L, -1, cacheKey);

	if (!lua_isfunction(L, -1))
	{
		// remove nil from stack top
		lua_pop(L, 1);

		// load from file
		luaL_loadfile(L, scriptPath);

		// check enable lua code cache
		if (enableCodeCache)
		{
			// chunk into cache table
			lua_setfield(L, -2, cacheKey);

			// retry load function from cache
			lua_getfield(L, -1, cacheKey);
		}
	}

	// remove cache table from stack
	lua_remove(L, -2);

	return true;
}

lua_State *iis_lua_newthread(lua_State *L)
{
	// create thread
	auto co = lua_newthread(L);

	// global table
	lua_createtable(co, 0, 1);

	lua_pushvalue(co, -1);
	lua_setfield(co, -2, "_G");

	// create metatable
	lua_createtable(co, 0, 1);

	lua_pushvalue(co, LUA_GLOBALSINDEX);
	lua_setfield(co, -2, "__index");

	// set metatable
	lua_setmetatable(co, -2);

	// replace global table
	lua_replace(co, LUA_GLOBALSINDEX);

	// restore stack
	lua_pop(L, 1);

	return co;
}

IHttpContext *iis_lua_get_http_ctx(lua_State *L)
{
	lua_getglobal(L, iis_lua_ctx_key);

	auto ctx = reinterpret_cast<IHttpContext *>(lua_touserdata(L, -1));

	lua_pop(L, 1);

	return ctx;
}

void iis_lua_set_http_ctx(lua_State *L, IHttpContext *ctx)
{
	lua_pushlightuserdata(L, ctx);
	lua_setglobal(L, iis_lua_ctx_key);
}

REQUEST_NOTIFICATION_STATUS iis_lua_get_result(lua_State *L)
{
	lua_getglobal(L, iis_lua_result_key);

	auto result = static_cast<REQUEST_NOTIFICATION_STATUS>(lua_tointeger(L, -1));

	lua_pop(L, 1);

	return result;
}

void iis_lua_set_result(lua_State *L, REQUEST_NOTIFICATION_STATUS result)
{
	lua_pushinteger(L, result);
	lua_setglobal(L, iis_lua_result_key);
}

CLuaHttpStoredContext *iis_lua_get_stored_context(IHttpContext *pHttpContext)
{
	auto pModuleContextContainer = pHttpContext->GetModuleContextContainer();
	auto pHttpStoredContext = reinterpret_cast<CLuaHttpStoredContext *>(pModuleContextContainer->GetModuleContext(g_pModuleContext));

	if (pHttpStoredContext != NULL)
	{
		return pHttpStoredContext;
	}

	pHttpStoredContext = new CLuaHttpStoredContext();

	if (FAILED(pModuleContextContainer->SetModuleContext(pHttpStoredContext, g_pModuleContext)))
	{
		pHttpStoredContext->CleanupStoredContext();

		return NULL;
	}

	return pHttpStoredContext;
}

const CModuleConfiguration *iis_lua_get_config(IHttpContext *pHttpContext)
{
	auto pModuleContextContainer = pHttpContext->GetMetadata()->GetModuleContextContainer();
	auto pModuleConfig = reinterpret_cast<CModuleConfiguration *>(pModuleContextContainer->GetModuleContext(g_pModuleContext));

	if (pModuleConfig != NULL)
	{
		return pModuleConfig;
	}

	pModuleConfig = new CModuleConfiguration();

	if (FAILED(pModuleConfig->Initialize(pHttpContext, g_pHttpServer)))
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
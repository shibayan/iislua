
#include "stdafx.h"

static PCSTR iislua_ctx_key = "__iislua_ctx";
static PCSTR iislua_result_key = "__iislua_result";

static char iislua_cache_table_key;

static concurrency::critical_section critical_section;

static const luaL_Reg iis[] =
{
    { "debug", iislua_debug },
    { "exec", iislua_exec },
    { "exit", iislua_exit },
    { "flush", iislua_flush },
    { "headers_sent", iislua_headers_sent },
    { "map_path", iislua_map_path },
    { "print", iislua_print },
    { "redirect", iislua_redirect },
    { "say", iislua_say },
    { NULL, NULL }
};

static const luaL_Reg iis_req[] =
{
    { "get_headers", iislua_req_get_headers },
    { "get_method", iislua_req_get_method },
    { "get_remote_addr", iislua_req_get_remote_addr },
    { "get_url", iislua_req_get_url },
    { "get_url_args", iislua_req_get_url_args },
    { "http_version", iislua_req_http_version },
    { "set_header", iislua_req_set_header },
    { "set_method", iislua_req_set_method },
    { "set_url", iislua_req_set_url },
    { NULL, NULL }
};

static const luaL_Reg iis_resp[] =
{
    { "clear", iislua_resp_clear },
    { "clear_headers", iislua_resp_clear_headers },
    { "get_headers", iislua_resp_get_headers },
    { "get_status", iislua_resp_get_status },
    { "set_header", iislua_resp_set_header },
    { "set_status", iislua_resp_set_status },
    { NULL, NULL }
};

static const luaL_Reg iis_srv[] =
{
    { "get_variable", iislua_srv_get_variable },
    { "set_variable", iislua_srv_set_variable },
    { NULL, NULL }
};

static const luaL_Reg iis_user[] =
{
    { "get_name", iislua_user_get_name },
    { NULL, NULL }
};

lua_State *iislua_newstate()
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
    iislua_create_cachetable(L);

    return L;
}

void iislua_close(lua_State *L)
{
    lua_close(L);
}

void iislua_create_cachetable(lua_State *L)
{
    lua_pushlightuserdata(L, &iislua_cache_table_key);
    lua_createtable(L, 0, 0);
    lua_rawset(L, LUA_REGISTRYINDEX);
}

bool iislua_load_cachetable(lua_State *L)
{
    lua_pushlightuserdata(L, &iislua_cache_table_key);
    lua_rawget(L, LUA_REGISTRYINDEX);

    return lua_istable(L, -1);
}

lua_State *iislua_load_function(CModuleConfiguration *config, PCSTR scriptPath, PCSTR cacheKey)
{
    critical_section.lock();

    lua_State *L = config->GetLuaState();

    // load cache table
    if (!iislua_load_cachetable(L))
    {
        critical_section.unlock();

        return nullptr;
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
        if (config->IsEnableCodeCache())
        {
            // chunk into cache table
            lua_setfield(L, -2, cacheKey);

            // retry load function from cache
            lua_getfield(L, -1, cacheKey);
        }
    }

    // remove cache table from stack
    lua_remove(L, -2);

    // set sandboxing
    auto co = iislua_newthread(L);
    
    lua_xmove(L, co, 1);

    lua_pushvalue(co, LUA_GLOBALSINDEX);
    lua_setfenv(co, -2);

    critical_section.unlock();

    return co;
}

lua_State *iislua_newthread(lua_State *L)
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

void iislua_initialize(lua_State *L, IHttpContext *ctx)
{
    iislua_set_http_ctx(L, ctx);
    iislua_set_result(L, RQ_NOTIFICATION_CONTINUE);
}

IHttpContext *iislua_get_http_ctx(lua_State *L)
{
    lua_getglobal(L, iislua_ctx_key);

    auto ctx = reinterpret_cast<IHttpContext *>(lua_touserdata(L, -1));

    lua_pop(L, 1);

    return ctx;
}

void iislua_set_http_ctx(lua_State *L, IHttpContext *ctx)
{
    lua_pushlightuserdata(L, ctx);
    lua_setglobal(L, iislua_ctx_key);
}

REQUEST_NOTIFICATION_STATUS iislua_finish_request(lua_State *L)
{
    lua_getglobal(L, iislua_result_key);

    auto result = static_cast<REQUEST_NOTIFICATION_STATUS>(lua_tointeger(L, -1));

    lua_pop(L, 1);

    return result;
}

void iislua_set_result(lua_State *L, REQUEST_NOTIFICATION_STATUS result)
{
    lua_pushinteger(L, result);
    lua_setglobal(L, iislua_result_key);
}
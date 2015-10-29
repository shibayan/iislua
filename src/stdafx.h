#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <comdef.h>
#include <concrt.h>
#include <winsock2.h>

#include <string>
#include <vector>

#include <httpserv.h>
#include <lua.hpp>

#pragma comment(lib, "ws2_32.lib")

#if LUA_VERSION_NUM < 502
# define luaL_newlib(L, l) (lua_newtable(L), luaL_register(L, NULL, l))
#endif

#define IISLUA_API extern "C"
#define CHECK_ARGUMENT(L, top) if (lua_gettop(L) != top) { return luaL_error(L, "argument error"); }

// iislua
#include "iislua_core.h"
#include "iislua_util.h"

#include "iislua_api.h"
#include "iislua_tcp.h"

// iis http module interface
#include "cmoduleconfiguration.h"
#include "cluahttpmodule.h"
#include "cluahttpmodulefactory.h"
#include "cluahttpstoredcontext.h"
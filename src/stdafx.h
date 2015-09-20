#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <comdef.h>
#include <concrt.h>

#include <string>
#include <vector>

#include <httpserv.h>
#include <lua.hpp>

#if LUA_VERSION_NUM < 502
# define luaL_newlib(L, l) (lua_newtable(L), luaL_register(L, NULL, l))
#endif

// iislua
#include "iislua_core.h"
#include "iislua_util.h"

#include "iislua_api.h"

// iis http module interface
#include "cmoduleconfiguration.h"
#include "cluahttpmodule.h"
#include "cluahttpmodulefactory.h"
#include "cluahttpstoredcontext.h"
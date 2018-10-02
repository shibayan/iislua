#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <comdef.h>
#include <winsock2.h>

#include <string>
#include <vector>

#include <httpserv.h>
#include <lua.hpp>

#define IISLUA_API extern "C"
#define CHECK_ARGUMENT(L, top) if (lua_gettop(L) != top) { return luaL_error(L, "argument error"); }

// iislua
#include "iislua_core.hpp"
#include "iislua_util.hpp"

// iislua libs
#include "iislua_api.hpp"
#include "iislua_log.hpp"
#include "iislua_req.hpp"
#include "iislua_resp.hpp"
#include "iislua_srv.hpp"
#include "iislua_user.hpp"

#include "iislua_socket_tcp.hpp"

// lua pooling
#include "cluastatepool.hpp"

// iis http module interface
#include "cmoduleconfiguration.hpp"
#include "cluahttpmodule.hpp"
#include "cluahttpmodulefactory.hpp"
#include "cluahttpstoredcontext.hpp"
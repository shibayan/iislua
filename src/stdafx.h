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
#include "iislua_core.h"
#include "iislua_util.h"

// iislua libs
#include "iislua_api.h"
#include "iislua_req.h"
#include "iislua_resp.h"
#include "iislua_srv.h"
#include "iislua_user.h"

#include "iislua_socket_tcp.h"

// lua pooling
#include "cluastatepool.h"

// iis http module interface
#include "cmoduleconfiguration.h"
#include "cluahttpmodule.h"
#include "cluahttpmodulefactory.h"
#include "cluahttpstoredcontext.h"
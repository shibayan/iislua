#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <httpserv.h>
#include <lua.hpp>

// iislua
#define IISLUA_API extern "C"
#define IISLUA_INLINE_API extern "C" inline

#include "iis_lua_const.h"
#include "iis_lua_util.h"

#include "iis_lua_api.h"

// iis http module interface
#include "cmoduleconfiguration.h"
#include "cluahttpmodule.h"
#include "cluahttpmodulefactory.h"
#include "cluahttpstoredcontext.h"
#pragma once

class CLuaHttpStoredContext;
class CModuleConfiguration;

CLuaHttpStoredContext *iis_lua_get_stored_context(IHttpContext *);
const CModuleConfiguration *iis_lua_get_config(IHttpContext *);

PCSTR iis_lua_util_get_status_reason(USHORT);

PSTR iis_lua_wstr_to_str(PCWSTR);
PWSTR iis_lua_str_to_wstr(PCSTR);
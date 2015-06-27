#pragma once

class CLuaHttpStoredContext;
class CModuleConfiguration;

IISLUA_API lua_State *iis_lua_newstate(IHttpContext *ctx);
IISLUA_API REQUEST_NOTIFICATION_STATUS iis_lua_close(lua_State *L);

IISLUA_INLINE_API IHttpContext *iis_lua_get_http_ctx(lua_State *L);
IISLUA_INLINE_API void iis_lua_set_http_ctx(lua_State *L, IHttpContext *ctx);

IISLUA_INLINE_API REQUEST_NOTIFICATION_STATUS iis_lua_get_result(lua_State *L);
IISLUA_INLINE_API void iis_lua_set_result(lua_State *L, REQUEST_NOTIFICATION_STATUS result = RQ_NOTIFICATION_CONTINUE);

IISLUA_API CLuaHttpStoredContext *iis_lua_get_stored_context(IHttpContext *pHttpContext);
IISLUA_API const CModuleConfiguration *iis_lua_get_config(IHttpContext *pHttpContext);

IISLUA_API PCSTR iis_lua_util_get_status_reason(USHORT status);

IISLUA_API PSTR iis_lua_wstr_to_str(PCWSTR wstr);
IISLUA_API PWSTR iis_lua_str_to_wstr(PCSTR str);
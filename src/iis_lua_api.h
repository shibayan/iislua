
#pragma once

struct lua_State;
class IHttpContext;

#define IIS_LUA_API extern "C"

IIS_LUA_API inline IHttpContext *iis_lua_get_http_ctx(lua_State *);
IIS_LUA_API inline void iis_lua_set_http_ctx(lua_State *, IHttpContext *);

IIS_LUA_API inline BOOL iis_lua_get_handled(lua_State *);
IIS_LUA_API inline void iis_lua_set_handled(lua_State *, BOOL);

IIS_LUA_API int iis_lua_debug(lua_State *);
IIS_LUA_API int iis_lua_exit(lua_State *);
IIS_LUA_API int iis_lua_headers_sent(lua_State *);
IIS_LUA_API int iis_lua_map_path(lua_State *);
IIS_LUA_API int iis_lua_print(lua_State *);
IIS_LUA_API int iis_lua_redirect(lua_State *);

IIS_LUA_API int iis_lua_req_get_headers(lua_State *);
IIS_LUA_API int iis_lua_req_get_method(lua_State *);
IIS_LUA_API int iis_lua_req_http_version(lua_State *);
IIS_LUA_API int iis_lua_req_set_method(lua_State *);
IIS_LUA_API int iis_lua_req_set_url(lua_State *);

IIS_LUA_API int iis_lua_resp_get_headers(lua_State *);
IIS_LUA_API int iis_lua_resp_set_header(lua_State *);
IIS_LUA_API int iis_lua_resp_set_status(lua_State *);

IIS_LUA_API int iis_lua_server_get_variables(lua_State *);

static const char *iis_lua_util_get_status_reason(int status);

static const char* http_header_id_to_name [] =
{
    "Cache-Control",
    "Connection",
    "Date",
    "Keep-Alive",
    "Pragma",
    "Trailer",
    "Transfer-Encoding",
    "Upgrade",
    "Via",
    "Warning",
    "Allow",
    "Content-Length",
    "Content-Type",
    "Content-Encoding",
    "Content-Language",
    "Content-Location",
    "Content-MD5",
    "Content-Range",
    "Expires",
    "Last-Modified",
    "Accept",
    "Accept-Charset",
    "Accept-Encoding",
    "Accept-Language",
    "Authorization",
    "Cookie",
    "Expect",
    "From",
    "Host",
    "If-Match",
    "If-Modified-Since",
    "If-None-Match",
    "If-Range",
    "If-Unmodified-Since",
    "Max-Forwards",
    "Proxy-Authorization",
    "Referer",
    "Range",
    "Te",
    "Translate",
    "User-Agent"
};
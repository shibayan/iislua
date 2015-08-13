#pragma once

PCSTR iis_lua_util_get_http_req_header(USHORT id);
PCSTR iis_lua_util_get_http_resp_header(USHORT id);

PCSTR iis_lua_util_get_status_reason(USHORT status);

std::string iis_lua_to_str(PCWSTR wstr);
std::wstring iis_lua_to_wstr(PCSTR str);
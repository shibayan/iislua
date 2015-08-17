#pragma once

PCSTR iislua_util_get_http_req_header(USHORT id);
PCSTR iislua_util_get_http_resp_header(USHORT id);

PCSTR iislua_util_get_status_reason(USHORT status);

std::string iislua_to_str(PCWSTR wstr);
std::wstring iislua_to_wstr(PCSTR str);
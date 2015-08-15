
#include "stdafx.h"

static PCSTR iis_lua_http_header_id_to_req_name[] =
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

static PCSTR iis_lua_http_header_id_to_resp_name[] =
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
    "Accept-Ranges",
    "Age",
    "ETag",
    "Location",
    "Proxy-Authenticate",
    "Retry-After",
    "Server",
    "Set-Cookie",
    "Vary",
    "WWW-Authenticate"
};

PCSTR iis_lua_util_get_http_req_header(USHORT id)
{
    return iis_lua_http_header_id_to_req_name[id];
}

PCSTR iis_lua_util_get_http_resp_header(USHORT id)
{
    return iis_lua_http_header_id_to_resp_name[id];
}

PCSTR iis_lua_util_get_status_reason(USHORT status)
{
    switch (status)
    {
        case 100:
            return "Continue";
        case 101:
            return "Switching Protocols";
        case 200:
            return "OK";
        case 201:
            return "Created";
        case 202:
            return "Accepted";
        case 204:
            return "No Content";
        case 205:
            return "Reset Content";
        case 206:
            return "Partial Content";
        case 301:
            return "Moved Permanently";
        case 302:
            return "Found";
        case 303:
            return "See Other";
        case 304:
            return "Not Modified";
        case 307:
            return "Temporary Redirect";
        case 400:
            return "Bad Request";
        case 401:
            return "Unauthorized";
        case 403:
            return "Forbidden";
        case 404:
            return "Not Found";
        case 405:
            return "Method Not Allowed";
        case 406:
            return "Not Acceptable";
        case 407:
            return "Proxy Authentication Required";
        case 408:
            return "Request Timeout";
        case 409:
            return "Conflict";
        case 410:
            return "Gone";
        case 415:
            return "Unsupported Media Type";
        case 500:
            return "Internal Server Error";
        case 501:
            return "Not Implemented";
        case 502:
            return "Bad Gateway";
        case 503:
            return "Service Unavailable";
        case 504:
            return "Gateway Timeout";
        default:
            return "Unknown Reason";
    }
}

std::string iis_lua_to_str(PCWSTR wstr)
{
    auto len = wcslen(wstr);

    if (len == 0)
    {
        return std::string();
    }

    size_t i;
    std::vector<char> buffer(len * 2 + 1);

    wcstombs_s(&i, &buffer[0], len * 2 + 1, wstr, len);

    return std::string(buffer.begin(), buffer.begin() + i);
}

std::wstring iis_lua_to_wstr(PCSTR str)
{
    auto len = strlen(str);

    if (len == 0)
    {
        return std::wstring();
    }

    size_t i;
    std::vector<wchar_t> buffer(len + 1);

    mbstowcs_s(&i, &buffer[0], len + 1, str, len);

    return std::wstring(buffer.begin(), buffer.begin() + i);
}
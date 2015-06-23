#pragma once

class CModuleConfiguration;

const CModuleConfiguration *iis_lua_get_config(IHttpContext *pHttpContext);
const PCSTR iis_lua_util_get_status_reason(USHORT status);
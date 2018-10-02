
#include "stdafx.h"

HTTP_MODULE_ID g_pModuleContext;
IHttpServer *g_pHttpServer;

HRESULT APIENTRY RegisterModule(DWORD dwServerVersion, IHttpModuleRegistrationInfo *pModuleInfo, IHttpServer *pHttpServer)
{
    UNREFERENCED_PARAMETER(dwServerVersion);

    g_pModuleContext = pModuleInfo->GetId();
    g_pHttpServer = pHttpServer;

    auto hr = pModuleInfo->SetRequestNotifications(new CLuaHttpModuleFactory(), RQ_BEGIN_REQUEST | RQ_AUTHENTICATE_REQUEST | RQ_AUTHORIZE_REQUEST | RQ_EXECUTE_REQUEST_HANDLER | RQ_END_REQUEST | RQ_SEND_RESPONSE | RQ_MAP_PATH, 0);

    if (FAILED(hr))
    {
        return hr;
    }

    return pModuleInfo->SetPriorityForRequestNotification(RQ_SEND_RESPONSE, PRIORITY_ALIAS_HIGH);
}
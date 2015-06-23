
#include "stdafx.h"

HTTP_MODULE_ID g_pModuleContext;
IHttpServer *g_pHttpServer;

HRESULT APIENTRY RegisterModule(DWORD dwServerVersion, IHttpModuleRegistrationInfo *pModuleInfo, IHttpServer *pHttpServer)
{
    UNREFERENCED_PARAMETER(dwServerVersion);

    g_pModuleContext = pModuleInfo->GetId();
    g_pHttpServer = pHttpServer;

    return pModuleInfo->SetRequestNotifications(new CLuaModuleFactory(), RQ_BEGIN_REQUEST, 0);
}
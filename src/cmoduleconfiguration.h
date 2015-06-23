#pragma once

class IHttpStoredContext;
class IHttpContext;

class CModuleConfiguration : public IHttpStoredContext
{
private:
    char beginRequest[MAX_PATH];
public:
    HRESULT Initialize(IN IHttpContext *pHttpContext, IN IHttpServer *pHttpServer);

    VOID CleanupStoredContext(VOID);

    const char *GetBeginRequest() const;
};
#pragma once

#include "cef_app.h"
#include "internal/cef_types.h"

class CWUWebApp :
	public CefApp,
	public CefBrowserProcessHandler
{
public:
	~CWUWebApp(void);
	static CWUWebApp* getInstance();
	int Run(HINSTANCE hContext);

	virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() OVERRIDE;
	//实现CefBrowserProcessHandler接口
	virtual void OnContextInitialized() OVERRIDE;
	virtual void OnBeforeChildProcessLaunch( CefRefPtr<CefCommandLine> command_line ) OVERRIDE;
	virtual void OnRenderProcessThreadCreated( CefRefPtr<CefListValue> extra_info );

private:
	std::wstring getLogPath();
	std::wstring getCachePath();
	CWUWebApp(void);
	IMPLEMENT_REFCOUNTING(CWUWebApp);

	static CefRefPtr<CWUWebApp> m_pInstance;
};

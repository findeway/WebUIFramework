#ifndef WUWebCoreApp_h__
#define WUWebCoreApp_h__

#include "cef_app.h"
#include "internal/cef_types.h"
#include "BrowserV8Handler.h"

class CWUWebCoreApp :
	public CefApp,
	public CefRenderProcessHandler
{
public:
	~CWUWebCoreApp(void);
	static CefRefPtr<CWUWebCoreApp> getInstance();
	int Run(HINSTANCE hContext);

	virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() OVERRIDE;
	//实现CefRenderProcesshandler接口
	virtual void OnWebKitInitialized() OVERRIDE;
	virtual void OnContextCreated( CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context ) OVERRIDE;
	virtual void OnContextReleased( CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context ) OVERRIDE;
	virtual void OnBrowserCreated( CefRefPtr<CefBrowser> browser ) OVERRIDE;
	virtual void OnBrowserDestroyed( CefRefPtr<CefBrowser> browser ) OVERRIDE;
	virtual bool OnBeforeNavigation( CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request, NavigationType navigation_type, bool is_redirect ) OVERRIDE;
	virtual bool OnProcessMessageReceived( CefRefPtr<CefBrowser> browser, CefProcessId source_process, CefRefPtr<CefProcessMessage> message ) OVERRIDE;
	virtual void OnRenderThreadCreated( CefRefPtr<CefListValue> extra_info );
	virtual void OnFocusedNodeChanged( CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefDOMNode> node );

private:
	CWUWebCoreApp(void);
	IMPLEMENT_REFCOUNTING(CWUWebCoreApp);


	CefRefPtr<CBrowserV8Handler>			m_v8Handler;
	static CefRefPtr<CWUWebCoreApp>			m_pInstance;
};

#endif // WUWebCoreApp_h__


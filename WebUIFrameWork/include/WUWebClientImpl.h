#ifndef WUWebClient_h__
#define WUWebClient_h__
#include "cef_client.h"
#include "include/base/cef_lock.h"

class IWebBrowserEventHandler
{
public:
	virtual void OnDocumentComplete(){};
};

class CWUWebClientImpl :
	public CefClient,
	public CefDisplayHandler,
	public CefLifeSpanHandler,
	public CefLoadHandler,
	public CefContextMenuHandler
{
public:
	CWUWebClientImpl(void);
	~CWUWebClientImpl(void);
	
	//CefClient接口
	virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler() OVERRIDE;
	virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE;
	virtual CefRefPtr<CefLoadHandler> GetLoadHandler() OVERRIDE;
	virtual CefRefPtr<CefContextMenuHandler> GetContextMenuHandler() OVERRIDE;
	virtual bool OnProcessMessageReceived( CefRefPtr<CefBrowser> browser, CefProcessId source_process, CefRefPtr<CefProcessMessage> message ) OVERRIDE;

	//CefDisplayHandler接口
	virtual void OnLoadingStateChange( CefRefPtr<CefBrowser> browser, bool isLoading, bool canGoBack, bool canGoForward ) OVERRIDE;
	virtual void OnAddressChange( CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString& url ) OVERRIDE;
	virtual void OnTitleChange( CefRefPtr<CefBrowser> browser, const CefString& title ) OVERRIDE;

	//CefLifeSpanHandler接口
	virtual void OnAfterCreated( CefRefPtr<CefBrowser> browser ) OVERRIDE;
	virtual void OnBeforeClose( CefRefPtr<CefBrowser> browser ) OVERRIDE;

	//CefLoadHandler接口
	virtual void OnLoadEnd( CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int httpStatusCode ) OVERRIDE;
	virtual void OnLoadError( CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, ErrorCode errorCode, const CefString& errorText, const CefString& failedUrl ) OVERRIDE;

	//CefContextMenuHandler接口
	virtual void OnBeforeContextMenu(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefRefPtr<CefContextMenuParams> params,
		CefRefPtr<CefMenuModel> model);
	virtual bool OnContextMenuCommand(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefRefPtr<CefContextMenuParams> params,
		int command_id,
		EventFlags event_flags);
	//自定义接口
	CefRefPtr<CefBrowser> GetBrowser();
	void SetEventHandler(IWebBrowserEventHandler *handler);
private:
	bool IsDevToolsEnabled();
	bool ShowDevTools(CefRefPtr<CefBrowser> browser, const CefPoint& cefPoint);

	IMPLEMENT_REFCOUNTING(CWUWebClientImpl);
protected:
	CefRefPtr<CefBrowser>			m_ptrBrowser;
	IWebBrowserEventHandler			*m_eventHandler;
	bool							m_bBrowserReady;
private:
	base::Lock						m_lock;
};

#endif // WUWebClient_h__

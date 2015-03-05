#include "StdAfx.h"
#include "WUWebClientImpl.h"

//自定义鼠标菜单项
enum client_menu_ids {
	CLIENT_ID_SHOW_DEVTOOLS = MENU_ID_USER_FIRST,
	CLIENT_ID_INSPECT_ELEMENT
};

CWUWebClientImpl::CWUWebClientImpl(void)
{
	m_bBrowserReady = false;
}

CWUWebClientImpl::~CWUWebClientImpl(void)
{
}

CefRefPtr<CefDisplayHandler> CWUWebClientImpl::GetDisplayHandler()
{
	return this;
}

CefRefPtr<CefLifeSpanHandler> CWUWebClientImpl::GetLifeSpanHandler()
{
	return this;
}

CefRefPtr<CefLoadHandler> CWUWebClientImpl::GetLoadHandler()
{
	return this;
}

CefRefPtr<CefContextMenuHandler> CWUWebClientImpl::GetContextMenuHandler()
{
	return this;
}

bool CWUWebClientImpl::OnProcessMessageReceived( CefRefPtr<CefBrowser> browser, CefProcessId source_process, CefRefPtr<CefProcessMessage> message )
{
	bool bHandled = false;
	if (source_process == PID_RENDERER)
	{
		CefRefPtr<CefListValue> argumentList = message->GetArgumentList();
		CefString strMessageName = message->GetName();
		CefRefPtr<CefProcessMessage> resultMessage = CefProcessMessage::Create(strMessageName);
		//支持的JS回调列表,所有可供JS调用的接口都在此处
		if(strMessageName == _T("WUAPI_Test"))
		{
			if(argumentList->GetSize() == 1)
			{
				CefString strArgument = argumentList->GetString(0);
				resultMessage->GetArgumentList()->SetString(0,strArgument);
				bHandled = true;
			}
		}
		//通知browser进程
		if(bHandled)
		{
			browser->SendProcessMessage(PID_BROWSER, resultMessage);
		}
	}
	return bHandled;
}

void CWUWebClientImpl::OnLoadingStateChange( CefRefPtr<CefBrowser> browser, bool isLoading, bool canGoBack, bool canGoForward )
{
	__super::OnLoadingStateChange(browser,isLoading,canGoBack,canGoForward);
}

void CWUWebClientImpl::OnAddressChange( CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString& url )
{
	__super::OnAddressChange(browser,frame,url);
}

void CWUWebClientImpl::OnTitleChange( CefRefPtr<CefBrowser> browser, const CefString& title )
{
	__super::OnTitleChange(browser,title);
}

void CWUWebClientImpl::OnAfterCreated( CefRefPtr<CefBrowser> browser )
{
	base::AutoLock lock_scope(m_lock);
	m_ptrBrowser = browser;
	m_bBrowserReady = true;
	__super::OnAfterCreated(browser);
}

void CWUWebClientImpl::OnBeforeClose( CefRefPtr<CefBrowser> browser )
{
	base::AutoLock lock_scope(m_lock);
	//这里必须释放m_ptrBrowser否则cefshutdown在debug模式下会抛异常
	m_ptrBrowser = NULL;
	m_bBrowserReady = false;
	__super::OnBeforeClose(browser);
}

void CWUWebClientImpl::OnLoadEnd( CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int httpStatusCode )
{
	if (frame->IsMain())
	{
		if (m_eventHandler)
		{
			m_eventHandler->OnDocumentComplete();
		}
	}
	__super::OnLoadEnd(browser,frame,httpStatusCode);
}

void CWUWebClientImpl::OnLoadError( CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, ErrorCode errorCode, const CefString& errorText, const CefString& failedUrl )
{
	__super::OnLoadError(browser,frame,errorCode,errorText,failedUrl);
}
void CWUWebClientImpl::OnBeforeContextMenu(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	CefRefPtr<CefContextMenuParams> params,
	CefRefPtr<CefMenuModel> model)
{
	if (IsDevToolsEnabled())
	{
		model->AddItem(CLIENT_ID_SHOW_DEVTOOLS, L"开发者工具");
		model->AddItem(CLIENT_ID_INSPECT_ELEMENT, L"查看元素");
	}
}

bool CWUWebClientImpl::OnContextMenuCommand(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	CefRefPtr<CefContextMenuParams> params,
	int command_id,
	EventFlags event_flags)
{
	if (command_id == CLIENT_ID_SHOW_DEVTOOLS) {
		ShowDevTools(browser,CefPoint());
		return true;
	}else if(command_id == CLIENT_ID_INSPECT_ELEMENT){
		ShowDevTools(browser, CefPoint(params->GetXCoord(), params->GetYCoord()));
		return true;
	}
	return false;
}

bool CWUWebClientImpl::IsDevToolsEnabled()
{
#ifdef _DEBUG
	return true;
#else
	return false;
#endif
}

bool CWUWebClientImpl::ShowDevTools(CefRefPtr<CefBrowser> browser, const CefPoint& cefPoint)
{
	CefWindowInfo windowInfo;
	CefBrowserSettings settings;
#if defined(OS_WIN)
	windowInfo.SetAsPopup(browser->GetHost()->GetWindowHandle(), "DevTools");
#endif
	browser->GetHost()->ShowDevTools(windowInfo, browser->GetHost()->GetClient(), settings, cefPoint);
	return true;
}

CefRefPtr<CefBrowser> CWUWebClientImpl::GetBrowser()
{
	return m_ptrBrowser;
}

void CWUWebClientImpl::SetEventHandler( IWebBrowserEventHandler *handler )
{
	m_eventHandler = handler;
}

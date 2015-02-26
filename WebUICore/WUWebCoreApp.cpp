#include "StdAfx.h"
#include "WUWebCoreApp.h"
#include "internal/cef_types_wrappers.h"
#include "cef_browser_process_handler.h"
#include "CefUtil.h"

CWUWebCoreApp::CWUWebCoreApp(void)
{
}

CWUWebCoreApp::~CWUWebCoreApp(void)
{
}

CefRefPtr<CWUWebCoreApp> CWUWebCoreApp::getInstance()
{
	if(!m_pInstance.get())
	{
		m_pInstance = new CWUWebCoreApp();
	}
	return m_pInstance;
}

int CWUWebCoreApp::Run( HINSTANCE hContext )
{
	CefMainArgs mainArgs(hContext);
	return CefExecuteProcess(mainArgs, CWUWebCoreApp::getInstance().get(),NULL);
}

CefRefPtr<CefRenderProcessHandler> CWUWebCoreApp::GetRenderProcessHandler()
{
	return this;
}

void CWUWebCoreApp::OnWebKitInitialized()
{
	__super::OnWebKitInitialized();
	// 注册扩展
	std::wstring appCode =
		L"var WebUIApp;"
		L"if (!WebUIApp)"
		L"  WebUIApp = {};"
		L"(function() {"
		L"  WebUIApp.DoWebUICall = function(name, arguments) {"
		L"    native function DoWebUICall();"
		L"    return DoWebUICall(name, arguments);"
		L"  };"
		L"  WebUIApp.setWebUICallback = function(name, callback) {"
		L"    native function setWebUICallback();"
		L"    return setWebUICallback(name, callback);"
		L"  };"
		L"  WebUIApp.removeWebUICallback = function(name) {"
		L"    native function removeWebUICallback();"
		L"    return removeWebUICallback(name);"
		L"  };"
		L"})();";
	if(!m_v8Handler.get())
	{
		m_v8Handler = new CBrowserV8Handler();
	}
	CefRegisterExtension(L"v8/WebUIApp", appCode, m_v8Handler.get());
}

void CWUWebCoreApp::OnContextCreated( CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context )
{
	__super::OnContextCreated(browser,frame,context);
}

void CWUWebCoreApp::OnContextReleased( CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context )
{
	__super::OnContextReleased(browser,frame,context);
	//context销毁时自动销毁消息回调
	if(m_v8Handler.get())
	{
		m_v8Handler->RemoveMessageCallback(context);
	}
}

void CWUWebCoreApp::OnBrowserCreated( CefRefPtr<CefBrowser> browser )
{
	__super::OnBrowserCreated(browser);
}

void CWUWebCoreApp::OnBrowserDestroyed( CefRefPtr<CefBrowser> browser )
{
	__super::OnBrowserDestroyed(browser);
}

bool CWUWebCoreApp::OnBeforeNavigation( CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request, NavigationType navigation_type, bool is_redirect )
{
	return __super::OnBeforeNavigation(browser,frame,request,navigation_type,is_redirect);
}

bool CWUWebCoreApp::OnProcessMessageReceived( CefRefPtr<CefBrowser> browser, CefProcessId source_process, CefRefPtr<CefProcessMessage> message )
{
	__super::OnProcessMessageReceived(browser,source_process,message);
	//收到render进程js调用完成的通知，执行回调逻辑
	bool bHandled = false;
	if(source_process == PID_BROWSER)
	{
		if(m_v8Handler.get())
		{
			std::pair<CefRefPtr<CefV8Context>, CefRefPtr<CefV8Value> > targetCallback;
			if(m_v8Handler->FindMessageCallback(message->GetName(),browser->GetIdentifier(),targetCallback))
			{
				CefRefPtr<CefV8Value> callback = targetCallback.second;
				CefRefPtr<CefV8Context> context = targetCallback.first;
				if(callback.get())
				{
					context->Enter();
					CefV8ValueList arguments;
					CefRefPtr<CefListValue> msgArgs = message->GetArgumentList();
					arguments.push_back(CefV8Value::CreateString(message->GetName()));
					CefRefPtr<CefV8Value> argArray = CefV8Value::CreateArray(msgArgs->GetSize());
					ConvertListToArray(msgArgs,argArray);
					arguments.push_back(argArray);
					CefRefPtr<CefV8Value> retval = callback->ExecuteFunction(NULL,arguments);
					if(retval.get() && retval->IsBool())
					{
						bHandled = retval->GetBoolValue();
					}
					context->Exit();
				}
			}
		}
	}
	return bHandled;
}

void CWUWebCoreApp::OnRenderThreadCreated( CefRefPtr<CefListValue> extra_info )
{
	
}

void CWUWebCoreApp::OnFocusedNodeChanged( CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefDOMNode> node )
{
	
}

CefRefPtr<CWUWebCoreApp> CWUWebCoreApp::m_pInstance;

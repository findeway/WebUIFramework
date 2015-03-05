#include "StdAfx.h"
#include "WUWebFrmClient.h"
#include "cef/include/cef_runnable.h"
#include "cef/include/cef_task.h"

CWUWebFrmClient::CWUWebFrmClient(void):m_hOwner(NULL)
{
}

CWUWebFrmClient::~CWUWebFrmClient(void)
{
}

bool CWUWebFrmClient::Init( HWND hOwner, RECT rect )
{
	m_hOwner = hOwner;
	m_strUrl = L"file:///E:/workspace/html/template/index.html";
	return __super::Init(rect);
}

HWND CWUWebFrmClient::GetHandle()
{
	return m_hOwner;
}

bool CWUWebFrmClient::OnProcessMessageReceived( CefRefPtr<CefBrowser> browser, CefProcessId source_process, CefRefPtr<CefProcessMessage> message )
{
	bool bHandled = false;
	if (source_process == PID_RENDERER)
	{
		CefRefPtr<CefListValue> argumentList = message->GetArgumentList();
		CefString strMessageName = message->GetName();
		CefRefPtr<CefProcessMessage> resultMessage = CefProcessMessage::Create(strMessageName);
		//支持的JS回调列表,所有可供JS调用的接口都在此处
		if(strMessageName == _T("WUAPI_CloseWindow"))
		{
			bHandled = true;
			//multi_threaded_message_loop为true时，TID_UI并不是主线程,直接PostTask会失败
			//直接CloseWindow由于不是主线程会失败，所以需要通过windows消息来通信
			PostMessage(GetHandle(),WM_CLOSE,0,0);
			return bHandled;
		}
		browser->SendProcessMessage(PID_RENDERER, resultMessage);
	}
	return bHandled;
}

void CWUWebFrmClient::OnDocumentComplete()
{

}

void CWUWebFrmClient::SetInitUrl( const std::wstring& strUrl )
{
	m_strUrl = strUrl;
}


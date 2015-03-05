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
		//֧�ֵ�JS�ص��б�,���пɹ�JS���õĽӿڶ��ڴ˴�
		if(strMessageName == _T("WUAPI_CloseWindow"))
		{
			bHandled = true;
			//multi_threaded_message_loopΪtrueʱ��TID_UI���������߳�,ֱ��PostTask��ʧ��
			//ֱ��CloseWindow���ڲ������̻߳�ʧ�ܣ�������Ҫͨ��windows��Ϣ��ͨ��
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


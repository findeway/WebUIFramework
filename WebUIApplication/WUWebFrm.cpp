#include "StdAfx.h"
#include "WUWebFrm.h"

CWUWebFrm::CWUWebFrm(void)
{
}

CWUWebFrm::~CWUWebFrm(void)
{
}

HWND CWUWebFrm::GetHandle()
{
	return m_hWnd;
}

LRESULT CWUWebFrm::OnCreate( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddIdleHandler(this);
	pLoop->AddMessageFilter(this);
	RECT rect = {0};
	GetClientRect(&rect);
	Init(rect);
	CenterWindow(GetHandle());
	return 0;
}

LRESULT CWUWebFrm::OnClose( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);
	bHandled = FALSE;
	UnInit();
	return 1L;
}

LRESULT CWUWebFrm::OnDestroy( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	::PostQuitMessage(0);
	return 1L;
}

LRESULT CWUWebFrm::OnWindowPosChanged( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	CefRefPtr<CefBrowser> ptrBrowser = GetBrowser();
	if(ptrBrowser.get())
	{
		HWND hWnd = ptrBrowser->GetHost()->GetWindowHandle();
		if(hWnd && ::IsWindow(hWnd))
		{
			RECT rect = { 0 };
			::GetClientRect(m_hWnd, &rect);
			::MoveWindow(hWnd, 0, 0, rect.right - rect.left , rect.bottom - rect.top, FALSE);
		}
	}
	return 1;
}

BOOL CWUWebFrm::OnIdle()
{
	return FALSE;
}

BOOL CWUWebFrm::PreTranslateMessage( MSG* pMsg )
{
	return FALSE;
}

bool CWUWebFrm::Init( RECT rect )
{
	m_strUrl = L"http://www.baidu.com/";
	bool result = __super::Init(rect);
	return result;
}

bool CWUWebFrm::UnInit()
{
	__super::UnInit();
	return true;
}

void CWUWebFrm::OnDocumentComplete()
{
}



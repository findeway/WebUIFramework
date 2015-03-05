#include "StdAfx.h"
#include "WUWebFrm.h"

CWUWebFrm::CWUWebFrm(void)
{
	m_pWebFrmClient = NULL;
}

CWUWebFrm::~CWUWebFrm(void)
{
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
	CenterWindow(m_hWnd);
	return 0;
}

LRESULT CWUWebFrm::OnClose( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);
	bHandled = FALSE;
	return 0L;
}

LRESULT CWUWebFrm::OnDestroy( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	UnInit();
	::PostQuitMessage(0);
	return 1L;
}

LRESULT CWUWebFrm::OnWindowPosChanged( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	CefRefPtr<CefBrowser> ptrBrowser = m_pWebFrmClient->GetBrowser();
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
	if(!m_pWebFrmClient)
	{
		m_pWebFrmClient = new CWUWebFrmClient();
	}
	if(m_pWebFrmClient)
	{
		return m_pWebFrmClient->Init(m_hWnd,rect);
	}
	return false;
}

bool CWUWebFrm::UnInit()
{
	if(m_pWebFrmClient)
	{
		bool bResult = m_pWebFrmClient->UnInit();
		return bResult;
	}
	return false;
}
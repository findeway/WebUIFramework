#pragma once
#include <atlbase.h>
#include <atlwin.h>
#include <atlapp.h>
#include "WUWebClientBase.h"

class CWUWebFrm : 
	public CWindowImpl<CWUWebFrm>,
	public CWUWebClientBase,
	public CMessageFilter, 
	public CIdleHandler
{
public:
	CWUWebFrm(void);
	~CWUWebFrm(void);
	
	virtual HWND GetHandle();
	virtual bool Init(RECT rect);
	virtual bool UnInit();

	BEGIN_MSG_MAP(CWUWebFrm)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_WINDOWPOSCHANGED,OnWindowPosChanged)
	END_MSG_MAP()
protected:
	virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnWindowPosChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle();
	virtual void OnDocumentComplete();
private:
	IMPLEMENT_REFCOUNTING(CWUWebFrm);
};
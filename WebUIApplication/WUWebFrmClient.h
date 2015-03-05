#pragma once
#include "WUWebClientBase.h"

class CWUWebFrmClient:
	public CWUWebClientBase
{
public:
	CWUWebFrmClient(void);
	virtual ~CWUWebFrmClient(void);
	bool Init(HWND hOwner,RECT rect);
	void SetInitUrl(const std::wstring& strUrl);
	virtual HWND GetHandle();
	//ÖØÐ´CefClient½Ó¿Ú
	bool OnProcessMessageReceived( CefRefPtr<CefBrowser> browser, CefProcessId source_process, CefRefPtr<CefProcessMessage> message );
protected:
	virtual void OnDocumentComplete();
private:
	HWND					m_hOwner;
private:
	IMPLEMENT_REFCOUNTING(CWUWebFrmClient);
};

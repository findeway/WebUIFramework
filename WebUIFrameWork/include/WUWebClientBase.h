#ifndef WUWebBase_h__
#define WUWebBase_h__
#include "WUWebClientImpl.h"
#include "cef_v8.h"

class CWUWebClientBase :
	public CWUWebClientImpl,
	public IWebBrowserEventHandler
{
public:
	CWUWebClientBase(void);
	~CWUWebClientBase(void);
	virtual HWND GetHandle() = 0;
	void Refresh(bool bIgnoreCache = false);
	//执行简单js，不需要和Renderer进程交互，无返回值
	void ExecuteJavascript(const std::wstring& strFunc, const std::vector<std::wstring>& paramArray );
	virtual void Navigate(const wchar_t* szUrl);
	virtual bool Init(RECT rect);
	virtual bool UnInit();
protected:
	virtual void CreateControl(RECT rect);
	virtual void DestroyControl();
	virtual void OnDocumentComplete();

#pragma warning(push)
#pragma warning(disable:4251)
	std::wstring				m_strUrl;
#pragma warning(pop)
private:
	IMPLEMENT_REFCOUNTING(CWUWebClientBase)
};

#endif // WUWebBase_h__

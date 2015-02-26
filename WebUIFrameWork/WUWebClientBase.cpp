#include "StdAfx.h"
#include "WUWebClientBase.h"

CWUWebClientBase::CWUWebClientBase( void )
{
	m_strUrl = L"about:blank";
}

CWUWebClientBase::~CWUWebClientBase( void )
{
}

bool CWUWebClientBase::Init( RECT rect )
{
	CreateControl(rect);
	return true;
}

bool CWUWebClientBase::UnInit()
{
	DestroyControl();
	return true;
}

void CWUWebClientBase::Refresh( bool bIgnoreCache /*= false*/ )
{
	CefRefPtr<CefBrowser> ptrWebBrowser = GetBrowser();
	if(ptrWebBrowser.get())
	{
		if(!bIgnoreCache)
		{
			ptrWebBrowser->Reload();
		}
		else
		{
			ptrWebBrowser->ReloadIgnoreCache();
		}
	}
}

void CWUWebClientBase::ExecuteJavascript( const std::wstring& strFunc, const std::vector<std::wstring>& paramArray )
{
	CefRefPtr<CefBrowser> ptrBrowser = GetBrowser();
	if(ptrBrowser.get())
	{
		CefRefPtr<CefFrame> ptrFrame = ptrBrowser->GetMainFrame();
		if(ptrFrame.get())
		{
			std::wstring jsCode = strFunc;
			jsCode += L"(";
			for(size_t index = 0; index < paramArray.size(); index++)
			{
				jsCode += paramArray[index];
				if(index != paramArray.size() - 1)
				{
					jsCode += L",";
				}
			}
			jsCode += L")";
			ptrFrame->ExecuteJavaScript(jsCode,ptrFrame->GetURL(),0);
		}
	}
}

void CWUWebClientBase::Navigate( const wchar_t* szUrl )
{
	CefRefPtr<CefBrowser> ptrWebBrowser = GetBrowser();
	if(ptrWebBrowser.get())
	{
		ptrWebBrowser->GetMainFrame()->LoadURL(szUrl);
	}
}

void CWUWebClientBase::CreateControl( RECT rect )
{
	if(GetHandle() == NULL)
	{
		return;
	}
	CefWindowInfo				webWinInfo;
	CefBrowserSettings			browserSetting;
	//页面为宿主窗口的子窗口
	webWinInfo.SetAsChild(GetHandle(), rect);

	SetEventHandler(this);
	// 异步创建浏览器控件
	CefBrowserHost::CreateBrowser(webWinInfo, this, m_strUrl.c_str(), browserSetting,NULL);
}

void CWUWebClientBase::DestroyControl()
{
	//释放webbrowser资源
	CefRefPtr<CefBrowser> ptrBrowser = GetBrowser();
	if(ptrBrowser.get())
	{
		CefRefPtr<CefBrowserHost> ptrBrowserHost = ptrBrowser->GetHost();
		if(ptrBrowserHost.get())
		{
			ptrBrowserHost->CloseBrowser(false);
		}
	}
}

void CWUWebClientBase::OnDocumentComplete()
{

}
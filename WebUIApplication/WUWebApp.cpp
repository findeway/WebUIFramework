#include "StdAfx.h"
#include "WUWebApp.h"
#include "CommonTools.h"
#include "WUWebFrm.h"

CWUWebApp::CWUWebApp(void)
{
}

CWUWebApp::~CWUWebApp(void)
{
}

CWUWebApp* CWUWebApp::getInstance()
{
	if(!m_pInstance.get())
	{
		m_pInstance = new CWUWebApp();
	}
	return m_pInstance;
}

int CWUWebApp::Run( HINSTANCE hContext )
{
	CefMainArgs main_args(hContext);
	CefSettings settings;
	CefString(&settings.browser_subprocess_path).FromWString(L"WebUICore.exe");
	//在单独的子线程中执行cef循环
	settings.multi_threaded_message_loop = true;
	//设置日志和缓存目录
	CefString(&settings.log_file).FromWString(CWUWebApp::getInstance()->getLogPath());
	CefString(&settings.cache_path).FromWString(CWUWebApp::getInstance()->getCachePath());
	CMessageLoop theLoop;
	_Module.AddMessageLoop(&theLoop);
	CefInitialize(main_args, settings, CWUWebApp::getInstance(),NULL);

	CWUWebFrm *webFrm = new CWUWebFrm();
	RECT rect = {0,0,800,600};
	webFrm->Create(NULL,rect,NULL,WS_OVERLAPPEDWINDOW|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS);

	theLoop.Run();
	CefShutdown();
	_Module.RemoveMessageLoop();

	return 0;
}

std::wstring CWUWebApp::getLogPath()
{
	SYSTEMTIME curTime;
	GetLocalTime(&curTime);
	wchar_t szPath[MAX_PATH] = {0};
	swprintf(szPath,MAX_PATH,_T("%s\\log\\SohuWebCore\\%04d-%02d-%02d.log"),GetPersonalDir().c_str(),curTime.wYear,curTime.wMonth,curTime.wDay);
	std::wstring strLogPath = szPath;
	return strLogPath;
}

std::wstring CWUWebApp::getCachePath()
{
	TCHAR szCachePath[MAX_PATH] = _T("");
	GetTempPath(MAX_PATH,szCachePath);
	if(szCachePath[_tcslen(szCachePath)-1] == TCHAR('\\'))
		szCachePath[_tcslen(szCachePath)-1] = TCHAR('\0');
	if(_taccess(szCachePath,0) != 0)
		CreateDirectory(szCachePath,NULL);
	_tcscat_s(szCachePath,MAX_PATH,_T("\\WebUICore_Cache"));
	if(_taccess(szCachePath,0) != 0)
		CreateDirectory(szCachePath,NULL);
	std::wstring strCachePath = szCachePath;
	return strCachePath;
}

CefRefPtr<CefBrowserProcessHandler> CWUWebApp::GetBrowserProcessHandler()
{
	return this;
}

void CWUWebApp::OnContextInitialized()
{
}

void CWUWebApp::OnBeforeChildProcessLaunch( CefRefPtr<CefCommandLine> command_line )
{

}

void CWUWebApp::OnRenderProcessThreadCreated( CefRefPtr<CefListValue> extra_info )
{
	
}

CefRefPtr<CWUWebApp> CWUWebApp::m_pInstance;

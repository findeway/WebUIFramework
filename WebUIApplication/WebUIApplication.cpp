// WebUIApplication.cpp : main source file for WebUIApplication.exe
//

#include "stdafx.h"
#include "WebUIFramework.h"
#include "CommonTools.h"
#include "client/windows/handler/exception_handler.h"
#include "WUWebApp.h"

CAppModule _Module;

//#ifndef _DEBUG
//google_breakpad::ExceptionHandler* g_pExceptionHandler = NULL;
//
////
//static bool DumpCallback(const wchar_t *dump_path, 
//						 const wchar_t *id, 
//						 void *context, 
//						 EXCEPTION_POINTERS *exinfo,
//						 MDRawAssertionInfo *assertion,  
//						 bool succeeded) 
//{   
//	return true;
//}   
//
//int UnhandledExceptionFilter(unsigned int code, struct _EXCEPTION_POINTERS *ep)
//{	
//	if(g_pExceptionHandler)
//	{
//		g_pExceptionHandler->WriteMinidumpForException(ep);
//	}
//	return EXCEPTION_EXECUTE_HANDLER;
//}
//#endif
//
//void EnableDump()
//{
//#ifndef _DEBUG
//	//create dump path
//	TCHAR szPath[MAX_PATH] = _T("");
//	_tcscpy_s(szPath, MAX_PATH, GetPersonalDir().c_str());
//	//
//	_tcscat(szPath,_T("\\dump"));
//	if(_taccess(szPath,0) != 0)
//		CreateDirectory(szPath,NULL);
//	//
//	if(_taccess(szPath,0) == 0)
//	{
//		static google_breakpad::ExceptionHandler eh(szPath, NULL, DumpCallback, NULL,google_breakpad::ExceptionHandler::HANDLER_NONE);
//		g_pExceptionHandler = &eh;
//	}
//#endif
//
//}

int Run(HINSTANCE hInstance,
		HINSTANCE hPrevInstance,
		LPTSTR    lpCmdLine,
		int       nCmdShow)
{
	HRESULT hRes = ::CoInitialize(NULL);
	// If you are running on NT 4.0 or higher you can use the following call instead to 
	// make the EXE free threaded. This means that calls come in on a random RPC thread.
	//	HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	ATLASSERT(SUCCEEDED(hRes));
	// this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
	::DefWindowProc(NULL, 0, 0, 0L);
	AtlInitCommonControls(ICC_COOL_CLASSES | ICC_BAR_CLASSES);	// add flags to support other controls
	hRes = _Module.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hRes));

	int nResult = CWUWebApp::getInstance()->Run(hInstance);

	_Module.Term();
	::CoUninitialize();
	return nResult;
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPTSTR    lpCmdLine,
					   int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
//#ifndef _DEBUG
//	__try
//	{
//		EnableDump();
//#endif
		return Run(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
//#ifndef _DEBUG
//	}
//	__except(UnhandledExceptionFilter(GetExceptionCode(),GetExceptionInformation()))
//	{
//		return GetExceptionCode();
//	}
//#endif
//	return 0;
}
#include "Util.h"
#include <Windows.h>
#include <ShlObj.h>

std::wstring GetPersonalDir()
{
	std::wstring strPersonalDir;

	TCHAR szPersonalPath[MAX_PATH] = _T("");
	SHGetSpecialFolderPath(NULL,szPersonalPath,CSIDL_PERSONAL,TRUE);
	return szPersonalPath;

    TCHAR szName[MAX_PATH] = _T("");
    GetModuleFileName(NULL, szName, MAX_PATH);
    LPTSTR lpszName =  _tcsrchr(szName, TCHAR('\\'));
    *_tcsrchr(lpszName, TCHAR('.')) = TCHAR('\0');
    strPersonalDir += lpszName;
    if (_taccess(strPersonalDir.c_str(), 0) != 0)
        CreateDirectory(strPersonalDir.c_str(), NULL);
    return strPersonalDir;
}


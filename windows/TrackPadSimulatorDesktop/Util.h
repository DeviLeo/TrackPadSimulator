#pragma once

int ConvertCharToInt32(const char *data);
int ConvertCharToInt16(const char *data);

char* ConvertInt32ToChar(const int i);
char* ConvertInt16ToChar(const int i);

void ReplaceFileInPath(TCHAR *szPath, DWORD dwPathLength, TCHAR *szFileName, DWORD dwFileNameSize);
void ShowLastErrorMessage(DWORD errCode, LPTSTR errTitle);

BOOL IsUserAdmin();
BOOL RunAsAdministrator();
BOOL LaunchAsService(TCHAR *szFileName, DWORD dwFileNameSize);

//>> unused
BOOL SetPrivilege(LPCTSTR lpszPrivilege, BOOL bEnablePrivilege);
BOOL SetPrivilege(HANDLE hToken, LPCTSTR lpszPrivilege, BOOL bEnablePrivilege);

BOOL GetInputDesktopName(TCHAR **pszDesktopName, DWORD *pdwLength);
BOOL SetWinSta0Desktop(TCHAR *szDesktopName);
//<<
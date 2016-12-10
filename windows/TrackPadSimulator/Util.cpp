
#include "stdafx.h"

#define SERVICE_NAME	TEXT("TrackPadSimulatorService")

int ConvertCharToInt32(const char *data)
{
	int i = *data & 0xFF;
	i += (*(data + 1) & 0xFF) << 8;
	i += (*(data + 2) & 0xFF) << 16;
	i += (*(data + 3) & 0xFF) << 24;
	return i;
}

int ConvertCharToInt16(const char *data)
{
	int i = *data & 0xFF;
	i += (*(data + 1) & 0xFF) << 8;
	return i;
}

char* ConvertInt32ToChar(const int i)
{
	char *data = (char*)malloc(4*sizeof(char));
	data[0] = (char)(i & 0xFF);
	data[1] = (char)((i >> 8) & 0xFF);
	data[2] = (char)((i >> 16) & 0xFF);
	data[3] = (char)((i >> 24) & 0xFF);
	return data;
}
char* ConvertInt16ToChar(const int i)
{
	char *data = (char*)malloc(2*sizeof(char));
	data[0] = (char)(i & 0xFF);
	data[1] = (char)((i >> 8) & 0xFF);
	return data;
}

void ReplaceFileInPath(TCHAR *szPath, DWORD dwPathLength, TCHAR *szFileName, DWORD dwFileNameSize)
{
	while(dwPathLength > 0)
	{
		--dwPathLength;
		if (szPath[dwPathLength] == '\\')
		{
			CopyMemory(szPath+dwPathLength+1, szFileName, dwFileNameSize);
			return;
		}
	}
}

void ShowLastErrorMessage(DWORD errCode, LPTSTR errTitle)
{
	LPTSTR errorText = NULL;

	FormatMessage(
	   FORMAT_MESSAGE_FROM_SYSTEM |
	   FORMAT_MESSAGE_ALLOCATE_BUFFER |
	   FORMAT_MESSAGE_IGNORE_INSERTS,  
	   NULL,
	   errCode,
	   MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
	   (LPTSTR)&errorText,
	   0,
	   NULL);

	if ( NULL != errorText )
	{
		WCHAR msg[512] = {0};
		wsprintf(msg, L"%s:\nError Code: %u\n%s\n", errTitle, errCode, errorText);

		LocalFree(errorText);
		errorText = NULL;

		wprintf(msg);
		OutputDebugString(msg);
	}
}

BOOL IsUserAdmin()
{
	BOOL bIsUserAdmin = FALSE;
	SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
	PSID AdministratorsGroup; 
	bIsUserAdmin = AllocateAndInitializeSid(
		&NtAuthority,
		2,
		SECURITY_BUILTIN_DOMAIN_RID,
		DOMAIN_ALIAS_RID_ADMINS,
		0, 0, 0, 0, 0, 0,
		&AdministratorsGroup); 
	if(bIsUserAdmin) 
	{
		if (!CheckTokenMembership(NULL, AdministratorsGroup, &bIsUserAdmin))
		{
			ShowLastErrorMessage(GetLastError(), TEXT("CheckTokenMembership"));
			bIsUserAdmin = FALSE;
		}
		FreeSid(AdministratorsGroup); 
	}

	return bIsUserAdmin;
}
BOOL RunAsAdministrator()
{
	WCHAR szExe[MAX_PATH] = {0};
	BOOL bSuccess = GetModuleFileName(NULL, szExe, MAX_PATH);
	if (!bSuccess) { ShowLastErrorMessage(GetLastError(), TEXT("GetModuleFileName")); }
	else
	{
		// Evaluate privileges
		HINSTANCE hInstance = ShellExecute(NULL, TEXT("runas"), szExe, NULL, NULL, SW_SHOWNORMAL);
		int err = (int)hInstance;
		bSuccess = (err > 32);
		if (!bSuccess) { ShowLastErrorMessage(err, TEXT("ShellExecute")); }
	}
	return bSuccess;
}
BOOL LaunchAsService(TCHAR *szFileName, DWORD dwFileNameSize)
{
	SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);

	if (NULL == hSCM)
	{
		DWORD dwErrCode = GetLastError();
		ShowLastErrorMessage(dwErrCode, TEXT("OpenSCManager"));
		return FALSE;
	}

	DWORD dwExeLength = 1024;
	DWORD dwExeSize = dwExeLength*sizeof(TCHAR);
	TCHAR *szExe = (TCHAR *)malloc(dwExeSize);
	ZeroMemory(szExe, dwExeSize);
	dwExeLength = GetModuleFileName(NULL, szExe, dwExeLength);
	if (dwExeLength <= 0) { ShowLastErrorMessage(GetLastError(), TEXT("GetModuleFileName")); }

	ReplaceFileInPath(szExe, dwExeLength, szFileName, dwFileNameSize);
	SC_HANDLE hService = CreateService(hSCM, SERVICE_NAME, SERVICE_NAME, 
		GENERIC_ALL, SERVICE_WIN32_OWN_PROCESS, SERVICE_DEMAND_START, SERVICE_ERROR_IGNORE,
		szExe, NULL, NULL, NULL, NULL, NULL);
	
	free(szExe);

	if (NULL == hService)
	{
		ShowLastErrorMessage(GetLastError(), TEXT("CreateService"));
		hService = OpenService(hSCM, SERVICE_NAME, GENERIC_ALL);
		if (NULL == hService)
		{
			ShowLastErrorMessage(GetLastError(), L"OpenService");
		}
	}

	BOOL bSuccess = StartService(hService, NULL, NULL);
	if (!bSuccess) { ShowLastErrorMessage(GetLastError(), TEXT("StartService")); }

	bSuccess = DeleteService(hService);
	if (!bSuccess) { ShowLastErrorMessage(GetLastError(), TEXT("DeleteService")); }

	if (NULL != hService) { CloseServiceHandle(hService); }
	if (NULL != hSCM) { CloseServiceHandle(hSCM); }

	return bSuccess;
}


//>> unused

BOOL SetPrivilege(LPCTSTR lpszPrivilege, BOOL bEnablePrivilege)
{
	HANDLE hToken;
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
	{
		ShowLastErrorMessage(GetLastError(), L"OpenProcessToken");
		return FALSE;
	}
	
	return SetPrivilege(hToken, lpszPrivilege, bEnablePrivilege);
}

BOOL SetPrivilege(HANDLE hToken, LPCTSTR lpszPrivilege, BOOL bEnablePrivilege)
{
	TOKEN_PRIVILEGES tp;
    LUID luid;

    if ( !LookupPrivilegeValue( 
            NULL,           // lookup privilege on local system
            lpszPrivilege,  // privilege to lookup 
            &luid))			// receives LUID of privilege
    {
		ShowLastErrorMessage(GetLastError(), L"LookupPrivilegeValue");
        return FALSE; 
    }

    tp.PrivilegeCount = 1;
    tp.Privileges[0].Luid = luid;
	tp.Privileges[0].Attributes = bEnablePrivilege ? SE_PRIVILEGE_ENABLED : 0;

    // Enable the privilege or disable all privileges.

    if ( !AdjustTokenPrivileges(
           hToken, 
           FALSE, 
           &tp, 
           sizeof(TOKEN_PRIVILEGES), 
           (PTOKEN_PRIVILEGES)NULL, 
           (PDWORD)NULL))
    {
		ShowLastErrorMessage(GetLastError(), L"AdjustTokenPrivileges");
        return FALSE; 
    }

    if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)
    {
		ShowLastErrorMessage(GetLastError(), L"The token does not have the specified privilege.");
        return FALSE; 
    } 

    return TRUE;
}

BOOL GetInputDesktopName(TCHAR *szDesktopName, DWORD *pdwLength)
{
	if (NULL == pdwLength) { return FALSE; }

	BOOL bSuccess = FALSE;

	HDESK hCurDesk = OpenInputDesktop(DF_ALLOWOTHERACCOUNTHOOK, FALSE, MAXIMUM_ALLOWED);
	if (NULL == hCurDesk) { ShowLastErrorMessage(GetLastError(), TEXT("OpenInputDesktop")); }
	
	DWORD dwLength = 0;
	bSuccess = GetUserObjectInformation(hCurDesk, UOI_NAME, NULL, NULL, &dwLength);
	if (!bSuccess) { ShowLastErrorMessage(GetLastError(), TEXT("GetUserObjectInformation-GetLength")); }
	else
	{
		if (NULL == szDesktopName) { *pdwLength = dwLength; }
		else
		{
			bSuccess = GetUserObjectInformation(hCurDesk, UOI_NAME, szDesktopName, *pdwLength, &dwLength);
			if (!bSuccess) { ShowLastErrorMessage(GetLastError(), TEXT("GetUserObjectInformation-GetDesktopName")); }
		}
	}

	if (hCurDesk != NULL) { CloseDesktop(hCurDesk); }

	return bSuccess;
}

BOOL SetWinSta0Desktop(TCHAR *szDesktopName)
{
	BOOL bSuccess = FALSE;

	HWINSTA hWinSta0 = OpenWindowStation(TEXT("WinSta0"), FALSE, MAXIMUM_ALLOWED);
	if (NULL == hWinSta0) { ShowLastErrorMessage(GetLastError(), TEXT("OpenWindowStation")); }

	bSuccess = SetProcessWindowStation(hWinSta0);
	if (!bSuccess) { ShowLastErrorMessage(GetLastError(), TEXT("SetProcessWindowStation")); }

	HDESK hDesk = OpenDesktop(szDesktopName, 0, FALSE, MAXIMUM_ALLOWED);
	if (NULL == hDesk) { ShowLastErrorMessage(GetLastError(), TEXT("OpenDesktop")); }

	bSuccess = SetThreadDesktop(hDesk);
	if (!bSuccess) { ShowLastErrorMessage(GetLastError(), TEXT("SetThreadDesktop")); }

	if (hDesk != NULL) { CloseDesktop(hDesk); }
	if (hWinSta0 != NULL) { CloseWindowStation(hWinSta0); }

	return bSuccess;
}
//<<
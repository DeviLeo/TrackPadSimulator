// TrackPadSimulatorService.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#define SERVICE_NAME	TEXT("TrackPadSimulatorService")

SERVICE_STATUS			g_ServiceStatus			= {0};
SERVICE_STATUS_HANDLE	g_ServiceStatusHandle	= NULL;
HANDLE					g_ServiceStopEvent		= INVALID_HANDLE_VALUE;

VOID WINAPI ServiceMain(DWORD argc, LPTSTR *argv);
VOID WINAPI ServiceCtrlHandler(DWORD dwCtrlCode);
DWORD WINAPI ServiceWorkerThread(LPVOID lpParam);

BOOL CreateWinlogonProcess(TCHAR *szFileName, DWORD dwFileNameSize);
void ReplaceFileInPath(TCHAR *szPath, DWORD dwPathLength, TCHAR *szFileName, DWORD dwFileNameSize);
void ShowLastErrorMessage(DWORD errCode, LPTSTR errTitle);


int _tmain(int argc, _TCHAR* argv[])
{
	SERVICE_TABLE_ENTRY ServiceTableEntry[] =
	{
		{SERVICE_NAME, (LPSERVICE_MAIN_FUNCTION)ServiceMain},
		{NULL, NULL}
	};

	if (!StartServiceCtrlDispatcher(ServiceTableEntry))
	{
		return GetLastError();
	}

	return 0;
}

VOID WINAPI ServiceMain(DWORD argc, LPTSTR *argv)
{
	DWORD dwStatus = E_FAIL;

	// Register our service control handler with the SCM
	g_ServiceStatusHandle = RegisterServiceCtrlHandler(SERVICE_NAME, ServiceCtrlHandler);
	if (NULL == g_ServiceStatusHandle) { return; }

	// Tell the service controller we are starting
	ZeroMemory(&g_ServiceStatus, sizeof(SERVICE_STATUS));
	g_ServiceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	g_ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
	g_ServiceStatus.dwControlsAccepted = 0;
	g_ServiceStatus.dwWin32ExitCode = 0;
	g_ServiceStatus.dwServiceSpecificExitCode = 0;
	g_ServiceStatus.dwCheckPoint = 0;

	BOOL bSuccess = SetServiceStatus(g_ServiceStatusHandle, &g_ServiceStatus);
	if (!bSuccess) { ShowLastErrorMessage(GetLastError(), TEXT("SetServiceStatus-START_PENDING")); }

	/*
	 * Perform tasks necessary to start the service here
	 */

	// Create a service stop event to wait on later
	g_ServiceStopEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (NULL == g_ServiceStopEvent)
	{
		// Error creating event
		// Tell service controller we are stopped and exit
		g_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
		g_ServiceStatus.dwControlsAccepted = 0;
		g_ServiceStatus.dwWin32ExitCode = GetLastError();
		g_ServiceStatus.dwCheckPoint = 1;

		bSuccess = SetServiceStatus(g_ServiceStatusHandle, &g_ServiceStatus);
		if (!bSuccess) { ShowLastErrorMessage(GetLastError(), TEXT("SetServiceStatus-STOPPED")); }
		return;
	}

	// Tell the service controller we are started
	g_ServiceStatus.dwCurrentState = SERVICE_RUNNING;
	g_ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;
	g_ServiceStatus.dwWin32ExitCode = 0;
	g_ServiceStatus.dwCheckPoint = 0;

	bSuccess = SetServiceStatus(g_ServiceStatusHandle, &g_ServiceStatus);
	if (!bSuccess) { ShowLastErrorMessage(GetLastError(), TEXT("SetServiceStatus-RUNNING")); }

	// Start a thread that will perform the main task of the service
	HANDLE hThread = CreateThread(NULL, 0, ServiceWorkerThread, NULL, 0, NULL);

	// Wait until our worker thread exits signaling that the service needs to stop
	WaitForSingleObject(hThread, INFINITE);

	/*
	 * Perform any cleanup tasks
	 */
	CloseHandle(g_ServiceStopEvent);

	// Tell the service controller we are stopped
	g_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
	g_ServiceStatus.dwControlsAccepted = 0;
	g_ServiceStatus.dwWin32ExitCode = 0;
	g_ServiceStatus.dwCheckPoint = 3;

	bSuccess = SetServiceStatus(g_ServiceStatusHandle, &g_ServiceStatus);
	if (!bSuccess) { ShowLastErrorMessage(GetLastError(), TEXT("SetServiceStatus-STOPPED")); }
}

VOID WINAPI ServiceCtrlHandler(DWORD dwCtrlCode)
{
	switch(dwCtrlCode)
	{
	case SERVICE_CONTROL_STOP:
		if (SERVICE_RUNNING == g_ServiceStatus.dwCurrentState)
		{
			/*
			 * Perform tasks necessary to stop the service here
			 */
			g_ServiceStatus.dwCurrentState = SERVICE_STOP_PENDING;
			g_ServiceStatus.dwControlsAccepted = 0;
			g_ServiceStatus.dwWin32ExitCode = 0;
			g_ServiceStatus.dwCheckPoint = 4;

			BOOL bSuccess = SetServiceStatus(g_ServiceStatusHandle, &g_ServiceStatus);
			if (!bSuccess) { ShowLastErrorMessage(GetLastError(), TEXT("SetServiceStatus-STOP_PENDING")); }

			// This will signal the worker thread to start shutting down
			SetEvent(g_ServiceStopEvent);
		}
		break;
	}
}

DWORD WINAPI ServiceWorkerThread(LPVOID lpParam)
{
	// Periodically check if the service has been requested to stop
	while (WAIT_OBJECT_0 != WaitForSingleObject(g_ServiceStopEvent, 0))
	{
		/*
		 * Perform main service function here
		 */

		TCHAR szFileName[] = TEXT("TrackPadSimulatorDesktop.exe");
		CreateWinlogonProcess(szFileName, sizeof(szFileName));

		break;
	}
	return ERROR_SUCCESS;
}

BOOL CreateWinlogonProcess(TCHAR *szFileName, DWORD dwFileNameSize)
{
	HANDLE hProcess = NULL;

	PROCESSENTRY32 entry = {0};
	entry.dwSize = sizeof(PROCESSENTRY32);

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (Process32First(hSnapshot, &entry))
	{
		while (Process32Next(hSnapshot, &entry))
		{
			if (wcscmp(entry.szExeFile, TEXT("winlogon.exe")) == 0)
			{
				hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, entry.th32ProcessID);
				break;
			}
		}
	}

	CloseHandle(hSnapshot);

	if (hProcess == NULL) { return FALSE; }

	HANDLE hToken = NULL;
	if (OpenProcessToken(hProcess, TOKEN_QUERY | TOKEN_DUPLICATE | TOKEN_ASSIGN_PRIMARY, &hToken))
	{
		STARTUPINFO si = {0};
		si.cb = sizeof(STARTUPINFO);
		si.lpDesktop = L"WinSta0\\Winlogon";

		PROCESS_INFORMATION pi = {0};

		DWORD dwExeLength = 1024;
		DWORD dwExeSize = dwExeLength*sizeof(TCHAR);
		TCHAR *szExe = (TCHAR *)malloc(dwExeSize);
		ZeroMemory(szExe, dwExeSize);
		dwExeLength = GetModuleFileName(NULL, szExe, dwExeLength);
		if (dwExeLength > 0)
		{
			ReplaceFileInPath(szExe, dwExeLength, szFileName, dwFileNameSize);
			if (CreateProcessAsUser(hToken, szExe, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
			{
				CloseHandle(pi.hThread);
				CloseHandle(pi.hProcess);
			}
		}

		free(szExe);

		CloseHandle(hToken);
	}

	CloseHandle(hProcess);

	return TRUE;
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

		OutputDebugString(msg);
	}
}
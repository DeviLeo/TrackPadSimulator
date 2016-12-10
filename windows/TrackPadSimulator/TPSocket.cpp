#include "stdafx.h"

#define SOCKET_STATUS_UNKNOWN	0
#define SOCKET_STATUS_CREATING	1
#define SOCKET_STATUS_LISTENING	2

#define HOST_PORT			22222
#define HOST_PORT_DESKTOP	22223

//
BOOL g_bSendDataToDesktop = FALSE;

//>> Accpet
SOCKET	g_socketServer		= NULL;
SOCKET	g_socketClient		= NULL;
int		g_nSocketStatus		= SOCKET_STATUS_UNKNOWN;
BOOL	g_bContinueListening= FALSE;
//<<

//>> Desktop Socket
SOCKET	g_socketDesktop		= NULL;
//<<

//>> Accept
void SyncKeyboardState()
{
	int keys[11] = {VK_CAPITAL, VK_SCROLL, VK_NUMLOCK, VK_LCONTROL, VK_RCONTROL,
	VK_LSHIFT, VK_RSHIFT, VK_LMENU, VK_RMENU, VK_LWIN, VK_RWIN};

	for(int i = 0; i < 10; ++i)
	{
		int key = keys[i];
		short state = GetKeyState(key);
		BOOL isKeyDown = (state & 0x8000) ? TRUE : FALSE;
		BOOL isKeyToggled = (state & 0x0001) ? TRUE : FALSE;
		if (i > 2) { isKeyToggled = isKeyDown; }

		char data[7];
		data[0] = INPUT_TYPE_KEYBOARD;
		data[1] = ACTION_KK_TOGGLE;
		char *keyData = ConvertInt32ToChar(key);
		memcpy(data+2, keyData, 4);
		data[6] = isKeyToggled ? 1 : 0;
		SendData(data, 7);
	}

}

int ResolveAction(TP_ACTION action)
{
	int retVal = 0;
	switch(action.action)
	{
	case ACTION_DOWN:
		retVal = OnActionDown(action);
		break;
	case ACTION_UP:
		retVal = OnActionUp(action);
		break;
	case ACTION_MOVE:
		retVal = OnActionMove(action);
		break;
	case ACTION_SINGLE_TAP:
		retVal = OnActionSingleTap(action);
		break;
	case ACTION_SINGLE_TAP_UP:
		retVal = OnActionSingleTapUp(action);
		break;
	case ACTION_DOUBLE_TAP_DOWN:
		retVal = OnActionDoubleTapDown(action);
		break;
	case ACTION_DOUBLE_TAP_UP:
		retVal = OnActionDoubleTapUp(action);
		break;
	case ACTION_DOUBLE_TAP_MOVE:
		retVal = OnActionDoubleTapMove(action);
		break;
	case ACTION_FLING:
		retVal = OnActionFling(action);
		break;
	case ACTION_KK_DOWN:
		retVal = OnActionKeyboardDown(action);
		break;
	case ACTION_KK_UP:
		retVal = OnActionKeyboardUp(action);
		break;
	case ACTION_KK_PRESS:
		retVal = OnActionKeyboardPress(action);
		break;
	case ACTION_KK_TOGGLE:
		retVal = OnActionKeyboardToggle(action);
		break;
	}

	return retVal;
}

int ResolveAction(const char *data, const int length)
{
	TP_ACTION action = {0};
	action.input_type = data[0];
	action.action = data[1];
	if (INPUT_TYPE_MOUSE == action.input_type)
	{
		action.pointer_count = data[2];

		if (ACTION_MOVE == action.action) 
		{
			action.distance_x = ConvertCharToInt32(data+3);
			action.distance_y = ConvertCharToInt32(data+7);
		}
		else if (ACTION_UP == action.action)
		{
			action.distance_x = action.distance_y = 0;
		}
		else if (ACTION_DOWN == action.action)
		{
			action.distance_x = action.distance_y = 0;
		}
		ResolveAction(action);
	}
	else if (INPUT_TYPE_KEYBOARD == action.input_type)
	{
		action.keyboard_key = ConvertCharToInt32(data+2);
		if (ACTION_KK_TOGGLE == action.action)
		{
			action.state = data[6];
		}
		ResolveAction(action);
	}
	return 0;
}

int ResolvePacket(const char *data, const int length)
{
	if (length <= PACKET_HEAD_SIZE) return -1;
	int head_flag = ConvertCharToInt16(data);
	if (HEAD_FLAG != head_flag)		return -2;
	int end_flag = ConvertCharToInt16(data+length-PACKET_END_SIZE);
	if (END_FLAG != end_flag)		return -3;
	
	int type = ConvertCharToInt16(data+PACKET_HEAD_SIZE);
	int packet_data_len = ConvertCharToInt32(data+PACKET_HEAD_SIZE+PACKET_TYPE_SIZE);
	if (0 == type)
	{
		int checksum = ConvertCharToInt32(data+8);
		if(~(head_flag+type+packet_data_len+checksum) != 0)
		{
			// Packet is broken,
			// or this is not a packet, may be data content, 
			// even if the head flag, length and end flag look like correct.
			return -4;
		}
		ResolveAction(data + PACKET_HEAD_LENGTH + PACKET_CHECKSUM_SIZE,
			packet_data_len - PACKET_CHECKSUM_SIZE - PACKET_END_SIZE);
	}
	else if (1 == type)
	{
		// TODO
		//int checksum = ConvertCharToInt32(data+32);
	}
	else
	{
		// Unknown Type
	}
	return 0;
}

// return the last incomplete action's index
int ResolveData(const char *data, const int length)
{
	if(SOCKET_ERROR == length) return SOCKET_ERROR;
	if(length <= PACKET_HEAD_SIZE) return 0;

	int index = 0;

	// If the rest data length is not bigger than packet head length,
	// return, continue downloading.
	for (int i = 0; i + PACKET_HEAD_LENGTH < length; ++i)
	{
		// Search Head
		int head_flag = ConvertCharToInt16(data+i);
		if (HEAD_FLAG == head_flag)
		{
			int head_index = i;
			int packet_data_len = ConvertCharToInt32(data+i+PACKET_HEAD_SIZE+PACKET_TYPE_SIZE);
			int packet_len = PACKET_HEAD_LENGTH + packet_data_len;
			int total_len = i + packet_len;

			// If the packet's end index is not bigger than data length,
			// search end flag
			if (total_len <= length)
			{
				int end_flag = ConvertCharToInt16(data+total_len-PACKET_END_SIZE);
				// If found end flag, resolve the packet.
				if (END_FLAG == end_flag)
				{
					if (0 == ResolvePacket(data+head_index, packet_len))
					{
						index = total_len;
						i = index - 1;
					}
				}
			}
			// If the packet's end index is bigger than data length,
			// break and return, continue downloading.
		}
	}

	return index;
}

void SendData(const char *data, const int data_len)
{
	if (NULL == g_socketClient) return;
	int packet_data_len = data_len + PACKET_CHECKSUM_SIZE + PACKET_END_SIZE;
	char *head = ConvertInt16ToChar(HEAD_FLAG);
	char *type = ConvertInt16ToChar(0);
	char *length = ConvertInt32ToChar(packet_data_len);
	char *checksum = ConvertInt32ToChar(~(HEAD_FLAG+0+packet_data_len));
	char *end = ConvertInt16ToChar(END_FLAG);

	int packet_len = PACKET_HEAD_LENGTH + packet_data_len;
	char *packet = (char*)malloc(packet_len*sizeof(char));
	char *packet_offset = packet;
	memcpy(packet_offset, head, PACKET_HEAD_SIZE);
	packet_offset += PACKET_HEAD_SIZE;
	memcpy(packet_offset, type, PACKET_TYPE_SIZE);
	packet_offset += PACKET_TYPE_SIZE;
	memcpy(packet_offset, length, PACKET_LENGTH_SIZE);
	packet_offset += PACKET_LENGTH_SIZE;
	memcpy(packet_offset, checksum, PACKET_CHECKSUM_SIZE);
	packet_offset += PACKET_CHECKSUM_SIZE;
	memcpy(packet_offset, data, data_len);
	packet_offset += data_len;
	memcpy(packet_offset, end, PACKET_END_SIZE);

	free(head);
	free(type);
	free(length);
	free(checksum);
	free(end);

	int sent = send(g_socketClient, packet, packet_len, 0);
	if (SOCKET_ERROR == sent)
	{
		// Do nothing
		int errcode = WSAGetLastError();
		ShowLastErrorMessage(errcode, L"FAILED to send");
	}

	free(packet);
}

void *ListenThread(void *param)
{
	CreateSocket();

	return 0;
}

void *RecvThread(void *param)
{
	SOCKET socketClient = (SOCKET)param;

	static const int data_len = 2048;
	static const int buf_len = 1024;
	char data[data_len];	// save the rest data cannot be resolved as complete action
	char buf[buf_len];		// save the received data
	int nRecv = 0;
	int nRecvZeroCount = 0;
	int nIncompleteActionIndex = 0, length = 0;
	while (SOCKET_ERROR != nRecv)
	{
		nRecv = recv(socketClient, buf, buf_len, 0);
		if (nRecv == 0) ++nRecvZeroCount;
		if (nRecvZeroCount > 999) break;
		if (nRecv > 0)
		{
			nRecvZeroCount = 0;

			if (length + nRecv >= data_len) { length = 0; }
			memcpy(data+length, buf, nRecv);
			length += nRecv;

			if (g_bSendDataToDesktop)
			{
				if (SendDataToDesktop(data, length))
				{
					length = 0;
					continue;
				}
			}
			nIncompleteActionIndex = ResolveData(data, length);
			if (nIncompleteActionIndex <= length) {
				length -= nIncompleteActionIndex;
				if (length > 0)
				{
					memcpy(data, data+nIncompleteActionIndex, length);
				}
			}
		}
	}

	closesocket(socketClient);
	g_socketClient = NULL;

	
	if (NULL != g_socketDesktop)
	{
		closesocket(g_socketDesktop);
		g_socketDesktop = NULL;
	}

	return 0;
}

void CreateSocket()
{
	g_nSocketStatus = SOCKET_STATUS_CREATING;	// Creating
	// 
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD(1,1); // 0x0101
	err = WSAStartup(wVersionRequested, &wsaData);

	if (NO_ERROR != err)
	{
		WCHAR e[128];
		wsprintf(e, L"WSAStartup Error Code: %d", err);
		MessageBox(NULL, e, L"Error", MB_OK|MB_ICONERROR);
		g_nSocketStatus = 0;
		return;
	}

	if (LOBYTE(wsaData.wVersion) != 1)
	{
		WCHAR e[128];
		wsprintf(e, L"LOBYTE(wsaData.wVersion) != 1, is %d", LOBYTE(wsaData.wVersion));
		MessageBox(NULL, e, L"Error", MB_OK|MB_ICONERROR);
		WSACleanup();
		g_nSocketStatus = 0;
		return;
	}

	if (HIBYTE(wsaData.wVersion) != 1)
	{
		WCHAR e[128];
		wsprintf(e, L"HIBYTE(wsaData.wVersion) != 1, is %d", LOBYTE(wsaData.wVersion));
		MessageBox(NULL, e, L"Error", MB_OK|MB_ICONERROR);
		WSACleanup();
		g_nSocketStatus = 0;
		return;
	}

	// Create Socket
	SOCKET sockServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == sockServer)
	{
		WCHAR e[128];
		wsprintf(e, L"Socket function failed with error: %ld", WSAGetLastError());
		WSACleanup();
		g_nSocketStatus = 0;
		return;
	}

	// Create IP & Port
	SOCKADDR_IN addrServer;
	addrServer.sin_family = AF_INET;
	addrServer.sin_port = htons(HOST_PORT);
	addrServer.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	// Bind Listener Port
	err = bind(sockServer, (SOCKADDR *)&addrServer, sizeof(SOCKADDR));
	if (SOCKET_ERROR == err)
	{
		WCHAR e[128];
		wsprintf(e, L"Bind Error Code: %d", WSAGetLastError());
		MessageBox(NULL, e, L"Error", MB_OK|MB_ICONERROR);
		closesocket(sockServer);
		WSACleanup();
		g_nSocketStatus = 0;
		return;
	}

	err = listen(sockServer, 1);
	if (SOCKET_ERROR == err)
	{
		WCHAR e[128];
		wsprintf(e, L"Listen Error Code: %d", WSAGetLastError());
		MessageBox(NULL, e, L"Error", MB_OK|MB_ICONERROR);
		closesocket(sockServer);
		WSACleanup();
		g_nSocketStatus = 0;
		return;
	}

	g_nSocketStatus = SOCKET_STATUS_LISTENING; // Listening
	g_socketServer = sockServer;

	g_bContinueListening = TRUE;
	while(g_bContinueListening)
	{
		SOCKET sockClient;
		int len = sizeof(sockaddr);
		sockaddr_in addrClient;
		sockClient = accept(sockServer, (sockaddr*)&addrClient, &len);
		if (INVALID_SOCKET != sockClient)
		{
			if (NULL == g_socketDesktop)
			{
				TCHAR szFileName[] = TEXT("TrackPadSimulatorService.exe");
				if (LaunchAsService(szFileName, sizeof(szFileName))) { CreateSocketToDesktop(); }
				else { g_socketDesktop = INVALID_SOCKET; }
				// Failed because priviliges is not enough, so trying to launch again is unnecessary.
				// Setting g_socketDesktop as INVALID_SOCKET can avoid to try again.
			}
			if (NULL != g_socketClient) { closesocket(g_socketClient); }
			g_socketClient = sockClient;
			SyncKeyboardState();
			CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)RecvThread, (void*)sockClient, 0, NULL);
		}
	}

	DestroySocket();
}

void DestroySocket()
{
	g_bContinueListening = FALSE;
	if (NULL != g_socketDesktop) { closesocket(g_socketDesktop); g_socketDesktop = NULL; }
	if (NULL != g_socketClient) { closesocket(g_socketClient); g_socketClient = NULL; }
	if (NULL != g_socketServer) { closesocket(g_socketServer); g_socketServer = NULL; }
	g_nSocketStatus = 0;
	WSACleanup();
}
//<<

// Desktop Socket
void CreateSocketToDesktop()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD(1,1); // 0x0101
	err = WSAStartup(wVersionRequested, &wsaData);

	if (NO_ERROR != err)
	{
		WCHAR e[128];
		wsprintf(e, L"WSAStartup Error Code: %d", err);
		MessageBox(NULL, e, L"Error", MB_OK|MB_ICONERROR);
		return;
	}

	if (LOBYTE(wsaData.wVersion) != 1)
	{
		WCHAR e[128];
		wsprintf(e, L"LOBYTE(wsaData.wVersion) != 1, is %d", LOBYTE(wsaData.wVersion));
		MessageBox(NULL, e, L"Error", MB_OK|MB_ICONERROR);
		return;
	}

	if (HIBYTE(wsaData.wVersion) != 1)
	{
		WCHAR e[128];
		wsprintf(e, L"HIBYTE(wsaData.wVersion) != 1, is %d", LOBYTE(wsaData.wVersion));
		MessageBox(NULL, e, L"Error", MB_OK|MB_ICONERROR);
		return;
	}

	// Create Socket
	SOCKET sockHost = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == sockHost)
	{
		WCHAR e[128];
		wsprintf(e, L"Create Socket Error Code: %d", WSAGetLastError());
		MessageBox(NULL, e, L"Error", MB_OK|MB_ICONERROR);
		return;
	}

	// Create IP & Port
	SOCKADDR_IN addrHost;
	addrHost.sin_family = AF_INET;
	addrHost.sin_port = htons(HOST_PORT_DESKTOP);
	addrHost.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	// Connect Host
	err = connect(sockHost, (SOCKADDR *)&addrHost, sizeof(SOCKADDR_IN));
	if (SOCKET_ERROR == err)
	{
		WCHAR e[128];
		wsprintf(e, L"Create Socket Error Code: %d", WSAGetLastError());
		MessageBox(NULL, e, L"Error", MB_OK|MB_ICONERROR);
		return;
	}
	else
	{
		g_socketDesktop = sockHost;
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)RecvFromDesktopThread, (void*)sockHost, 0, NULL);
	}
}
BOOL SendDataToDesktop(const char *data, const int data_len)
{
	if (NULL == g_socketDesktop) return FALSE;
	int sent = send(g_socketDesktop, data, data_len, 0);
	if (SOCKET_ERROR == sent)
	{
		// Do nothing
		int errcode = WSAGetLastError();
		ShowLastErrorMessage(errcode, L"FAILED to send");
		return FALSE;
	}
	return TRUE;
}
void *RecvFromDesktopThread(void *param)
{
	SOCKET socketDesktop = (SOCKET)param;

	static const int data_len = 2048;
	static const int buf_len = 1024;
	char data[data_len];	// save the rest data cannot be resolved as complete action
	char buf[buf_len];		// save the received data
	int nRecv = 0;
	int nRecvZeroCount = 0;
	int length = 0;
	while (SOCKET_ERROR != nRecv)
	{
		nRecv = recv(socketDesktop, buf, buf_len, 0);
		if (nRecv == 0) ++nRecvZeroCount;
		if (nRecvZeroCount > 999) break;
		if (nRecv > 0)
		{
			nRecvZeroCount = 0;

			if (length + nRecv >= data_len) { length = 0; }
			memcpy(data+length, buf, nRecv);
			length += nRecv;

			if (NULL == g_socketClient) break;
			int sent = send(g_socketClient, data, length, 0);
			if (SOCKET_ERROR == sent)
			{
				// Do nothing
				int errcode = WSAGetLastError();
				ShowLastErrorMessage(errcode, L"FAILED to send");
			}
			else
			{
				length = 0;
			}
		}
	}

	closesocket(socketDesktop);
	g_socketDesktop = NULL;

	return 0;
}
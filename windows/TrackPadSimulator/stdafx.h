// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // �� Windows ͷ���ų�����ʹ�õ�����
// Windows ͷ�ļ�:
#include <windows.h>

// C ����ʱͷ�ļ�
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
//>> system
#include <exception>
#include <stdio.h>
#include <shellapi.h>
#include <WinSock2.h>
#pragma comment(lib, "Ws2_32.lib")
#include <wtsapi32.h>
#pragma comment(lib, "wtsapi32.lib")
//<<

//>> user
#include "Util.h"
#include "TPDef.h"
#include "TPEvent.h"
#include "TPSocket.h"
//<<

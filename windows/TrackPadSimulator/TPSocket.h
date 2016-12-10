#pragma once

#include "stdafx.h"

//
extern BOOL g_bSendDataToDesktop;

// Accept
void CreateSocket();
void DestroySocket();
void SendData(const char *data, const int data_len);
void *ListenThread(void *param);
void *RecvThread(void *param);
int ResolveData(const char *data, const int length);
int ResolvePacket(const char *data, const int length);
int ResolveAction(const char *data, const int length);
int ResolveAction(TP_ACTION action);
void SyncKeyboardState();

// Desktop Socket
void CreateSocketToDesktop();
BOOL SendDataToDesktop(const char *data, const int data_len);
void *RecvFromDesktopThread(void *param);
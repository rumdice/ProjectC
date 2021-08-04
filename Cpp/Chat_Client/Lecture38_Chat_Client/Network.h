#pragma once

#include "stdafx.h"
#include "Packet.h"
#include "RingBuffer.h"

extern BOOL g_netSendPossibility;
extern BOOL g_netConnect;
extern CRingBuffer	g_RecvQ;
extern CRingBuffer	g_SendQ;
extern SOCKET		listenSocket;			

bool NetworkInit();													
bool NetworkPortOpen(HWND hWnd, wchar_t *ipAddress, wchar_t *port); 
bool NetworkProcess(HWND, UINT, WPARAM, LPARAM);					
void NetworkRelease();

BOOL RecvEvent();
BOOL SendEvent();
DWORD CompleteRecvPacket();
DWORD CompleteSendPacket();

bool SendPacket(CPacket *cPacket);

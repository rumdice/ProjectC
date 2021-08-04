#include "stdafx.h"
#include "Define.h"
#include "Network.h"
#include "RingBuffer.h"
#include "Packet.h"
#include "PacketDefine.h"
#include "PacketProc.h"

SOCKET		listenSocket;			
SOCKADDR_IN socketAddr;				
BOOL g_netConnect = FALSE;			
BOOL g_netSendPossibility = FALSE;	

CRingBuffer	g_RecvQ;							
CRingBuffer	g_SendQ;							
char tempRecvRingBuf[DEFAULT_RING_BUF_SIZE];	
char tempSendRingBuf[DEFAULT_RING_BUF_SIZE];	


bool NetworkInit()
{
	int result = -1;

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa))
		return false;

	listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listenSocket == INVALID_SOCKET)
		return false;

	char cFlag = 1;
	result = setsockopt(listenSocket, IPPROTO_TCP, TCP_NODELAY, &cFlag, sizeof(char));
	if (result == SOCKET_ERROR)
		return false;

	return true;
}

bool NetworkPortOpen(HWND hWnd, wchar_t *ipAddress, wchar_t *port)
{
	int result = -1;

	char address[16];
	wcstombs(address, ipAddress, 16);

	socketAddr.sin_family = AF_INET;
	socketAddr.sin_addr.s_addr = inet_addr(address);
	socketAddr.sin_port = htons(6000);

	WSAAsyncSelect(listenSocket, hWnd, WM_SOCKET, FD_CONNECT | FD_CLOSE | FD_READ | FD_WRITE);

	result = connect(listenSocket, (SOCKADDR *)&socketAddr, sizeof(socketAddr));
	if (result == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
			return false;
	}

	return true;
}

void NetworkRelease()
{
	if (listenSocket)
	{
		shutdown(listenSocket, SD_BOTH);
		closesocket(listenSocket);
		listenSocket = 0;
	}

	WSACleanup();
}

bool NetworkProcess(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	WORD event = WSAGETSELECTEVENT(lParam);
	WORD error = WSAGETSELECTERROR(lParam);

	if (error != 0)
	{
		MessageBox(0, L"Connect Error", L"", MB_OK);
		return FALSE;
	}

	switch (event)
	{
	case FD_CONNECT:
		g_netConnect = TRUE;
		SetWindowText(hWnd, L"Server connected");
		return TRUE;

	case FD_CLOSE:
		g_netConnect = FALSE;
		SetWindowText(hWnd, L"Server와 접속이 끊어졌습니다");
		closesocket(listenSocket);
		return FALSE;

	case FD_READ:
		if (!RecvEvent())
			return FALSE;

		return TRUE;

	case FD_WRITE:
		g_netSendPossibility = TRUE;

		if (!SendEvent())
			return FALSE;

		return TRUE;
	}
	return TRUE;
}

BOOL RecvEvent()
{
	DWORD	dwResult;
	DWORD	dwRecvSize;
	DWORD	dwFlag = 0;

	dwResult = recv(listenSocket, tempRecvRingBuf, DEFAULT_RING_BUF_SIZE, dwFlag);

	if (SOCKET_ERROR == dwResult)
	{
		return FALSE;
	}

	dwRecvSize = dwResult;

	if (0 < dwRecvSize)
	{
		g_RecvQ.Enqueue(tempRecvRingBuf, dwRecvSize);

		while (1)
		{
			dwResult = CompleteRecvPacket();

			if (1 == dwResult)
				break;
			if (0xff == dwResult)
				return FALSE;
		}
	}
	return TRUE;
}

DWORD CompleteRecvPacket()
{
	st_PACKET_HEADER stPacketHeader;

	int packetHeaderSize = sizeof(st_PACKET_HEADER);
	int iRecvQSize;

	iRecvQSize = g_RecvQ.GetUseSize();

	if (packetHeaderSize > iRecvQSize)
		return 1;

	g_RecvQ.Peek((char*)&stPacketHeader, packetHeaderSize);

	if (dfPACKET_CODE != stPacketHeader.byCode)
		return 0xff;

	g_RecvQ.RemoveData(packetHeaderSize);

	CPacket	cPacket;

	if (!g_RecvQ.Dequeue(cPacket.GetBufferPtr(), stPacketHeader.wPayloadSize))
		return 0xff;

	cPacket.MoveWritePos(stPacketHeader.wPayloadSize);

	if (!PacketProc(stPacketHeader.wMsgType, &cPacket))
		return 0xff;

	return 0;
}

BOOL SendEvent()
{
	DWORD dwResult;
	DWORD dwFlag = 0;

	if (!g_netSendPossibility)
		return TRUE;

	if (0 >= g_SendQ.GetUseSize())
		return TRUE;

	while (1)
	{
		dwResult = CompleteSendPacket();

		if (1 == dwResult)
			break;

		if (0xff == dwResult)
			return FALSE;
	}

	return TRUE;
}

DWORD CompleteSendPacket()
{
	int		iSendQSize;
	DWORD	dwResult;
	DWORD	dwSendSize;

	iSendQSize = g_SendQ.GetUseSize();

	if (iSendQSize == 0)
		return 1;

	g_SendQ.Peek(tempSendRingBuf, iSendQSize);

	dwResult = send(listenSocket, tempSendRingBuf, iSendQSize, 0);
	if (SOCKET_ERROR == dwResult)
	{
		if (WSAEWOULDBLOCK == WSAGetLastError())
		{
			g_netSendPossibility = FALSE;
			return 0;
		}
	}
	else
	{
		dwSendSize = dwResult;
		g_SendQ.RemoveData(dwSendSize);
	}

	return 0;
}

bool SendPacket(CPacket *cPacket)
{
	if (!g_netConnect)
		return FALSE;

	g_SendQ.Enqueue(cPacket->GetBufferPtr(), cPacket->GetDataSize());

	if (!SendEvent())
	{
		closesocket(listenSocket);
	}

	return TRUE;
}

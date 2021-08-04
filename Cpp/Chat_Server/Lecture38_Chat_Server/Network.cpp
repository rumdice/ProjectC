#include "Network.h"
#include "PacketDefine.h"
#include "Session.h"
#include "Define.h"
#include "PacketProc.h"
#include "Packet.h"
#include "Util.h"

SOCKET		listenSocket;			
SOCKADDR_IN socketAddr;				

FD_SET	recvSet;					
FD_SET	sendSet;					

map<DWORD, st_NETWORK_SESSION*> g_ClientMap;


extern st_NETWORK_SESSION	g_netSession[dfNETWORK_MAX_USER];

bool NetworkInit()
{
	int result = -1;

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa))
		return false;


	listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // 마지막인자가 어떤 책에선 0, 의미파악하기.
	if (listenSocket == INVALID_SOCKET)
		return false;


	u_long on = 1;
	result = ioctlsocket(listenSocket, FIONBIO, &on);
	if (result == SOCKET_ERROR)
		return false;
	
	char cFlag = 1;
	result = setsockopt(listenSocket, IPPROTO_TCP, TCP_NODELAY, &cFlag, sizeof(char));
	if (result == SOCKET_ERROR)
		return false;

	return true;
}

bool NetworkPortOpen()
{
	int result = -1;

	socketAddr.sin_family = AF_INET;
	socketAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	socketAddr.sin_port = htons(dfNETWORK_PORT);
	
	result = bind(listenSocket, (SOCKADDR*)&socketAddr, sizeof(SOCKADDR_IN));
	if (result == SOCKET_ERROR)
		return false;
	
	result = listen(listenSocket, SOMAXCONN);
	if (result == SOCKET_ERROR)
		return false;

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

bool NetworkProcess()
{
	int result = -1;

	TIMEVAL	timeout;

	FD_ZERO(&recvSet);
	FD_ZERO(&sendSet);
	FD_SET(listenSocket, &recvSet);

	for (int i = 0; i < dfNETWORK_MAX_USER; i++)
	{
		if (g_netSession[i].bConnect)
		{
			FD_SET(g_netSession[i].Socket, &recvSet);

			if (0 < g_netSession[i].SendBuffer.GetUseSize())
				FD_SET(g_netSession[i].Socket, &sendSet);
		}
	}

	timeout.tv_sec = 0;
	timeout.tv_usec = 1; 
	result = select(0, &recvSet, &sendSet, 0, &timeout);

	if (0 < result)
	{
		if (FD_ISSET(listenSocket, &recvSet))
		{
			SOCKET sock = NetworkAccept();

			if (sock != NULL)
			{
				int newSessionID = AddNewSession(sock);
				PacketProcConnect(newSessionID);
			}
		}

		for (int i = 0; i < dfNETWORK_MAX_USER; i++)
		{
			if (g_netSession[i].bConnect)
			{
				if (FD_ISSET(g_netSession[i].Socket, &recvSet))
				{
					// Recv 처리
					if (!NetworkRecv(i))
					{
						PacketProcDisconnect(g_netSession[i].dwSessionID);
						closesocket(g_netSession[i].Socket);
						RemoveSession(g_netSession[i].dwSessionID);
					}
				}
				else if (FD_ISSET(g_netSession[i].Socket, &sendSet))
				{
					// Send 처리
					if (!NetworkSend(i))
					{
						PacketProcDisconnect(g_netSession[i].dwSessionID);
						closesocket(g_netSession[i].Socket);
						RemoveSession(g_netSession[i].dwSessionID);
					}
				}
			}
		}
	}
	else if (result == SOCKET_ERROR)
	{
		return false;
	}

	return true;
}

void NetworkProcessNew()
{
	st_NETWORK_SESSION *pClient;
	DWORD userTable_NO[FD_SETSIZE];
	SOCKET userTable_SOCKET[FD_SETSIZE];
	int iSocketCount = 0;

	FD_SET ReadSet;
	FD_SET WriteSet;
	FD_ZERO(&ReadSet);
	FD_ZERO(&WriteSet);
	memset(userTable_NO, -1, sizeof(DWORD) * FD_SETSIZE);
	memset(userTable_SOCKET, INVALID_SOCKET, sizeof(SOCKET) * FD_SETSIZE);

	FD_SET(listenSocket, &ReadSet);
	userTable_NO[iSocketCount] = 0;
	userTable_SOCKET[iSocketCount] = listenSocket;
	iSocketCount++;

	map<DWORD, st_NETWORK_SESSION *>::iterator ClientIter;
	for (ClientIter = g_ClientMap.begin(); ClientIter != g_ClientMap.end();)
	{
		pClient = ClientIter->second;
		ClientIter++;

		userTable_NO[iSocketCount] = pClient->dwSessionID;
		userTable_SOCKET[iSocketCount] = pClient->Socket;

		FD_SET(pClient->Socket, &ReadSet);

		if (pClient->SendBuffer.GetUseSize() > 0)
			FD_SET(pClient->Socket, &WriteSet);

		iSocketCount++;

		if (FD_SETSIZE <= iSocketCount)
		{
			SelectSocket(userTable_NO, userTable_SOCKET, &ReadSet, &WriteSet);

			FD_ZERO(&ReadSet);
			FD_ZERO(&WriteSet);

			memset(userTable_NO, -1, sizeof(DWORD) * FD_SETSIZE);
			memset(userTable_SOCKET, INVALID_SOCKET, sizeof(SOCKET) * FD_SETSIZE);
			iSocketCount = 0;
		}
	}

	if (iSocketCount > 0)
	{
		SelectSocket(userTable_NO, userTable_SOCKET, &ReadSet, &WriteSet);
	}
}

void SelectSocket(DWORD* dwpTableNo, SOCKET* pTableSocket, FD_SET* pReadSet, FD_SET* pWriteSet)
{
	timeval Time;
	int iResult, iCnt;

	Time.tv_sec = 0;
	Time.tv_usec = 0; // 0 이상일 경우 64개 이후의 다음 소켓들의 접속이 늦어질 수 있다.

	iResult = select(0, pReadSet, pWriteSet, 0, &Time);

	if (0 < iResult)
	{
		for (iCnt = 0; iCnt < FD_SETSIZE; ++iCnt)
		{
			if (pTableSocket[iCnt] == INVALID_SOCKET)
				continue;

			if (FD_ISSET(pTableSocket[iCnt], pWriteSet))
			{
				NetworkSendNew(dwpTableNo[iCnt]);
			}

			if (FD_ISSET(pTableSocket[iCnt], pReadSet))
			{
				if (dwpTableNo[iCnt] == 0)
					NetworkAccept();
				else
					NetworkRecvNew(dwpTableNo[iCnt]);
			}
		}
	}
	else if (iResult == SOCKET_ERROR)
	{
		WriteConsoleLog("Select Socket Error %d", iResult);
	}
}

SOCKET NetworkAccept()
{
	SOCKADDR_IN	Addr;
	SOCKET		Socket;
	int			iSize = sizeof(Addr);

	Socket = accept(listenSocket, (SOCKADDR *)&Addr, &iSize);
	if (Socket == INVALID_SOCKET)
		return NULL;

	return Socket;
}

bool NetworkRecv(int sessionArrCount)
{
	int result;
	char				chBuff[DEFAULT_RING_BUF_SIZE];
	st_NETWORK_SESSION *stpSession = &g_netSession[sessionArrCount]; 
	DWORD				dwResult;

	result = recv(stpSession->Socket, chBuff, DEFAULT_RING_BUF_SIZE, 0);
	if (result == SOCKET_ERROR)
		return false;
	if (result == 0)
		return false;

	if (0 < result)
	{
		stpSession->RecvBuffer.Enqueue(chBuff, result);

		while (1)
		{
			dwResult = NetworkCompleteRecv(stpSession->dwSessionID);

			if (1 == dwResult)
				break;

			if (0xff == dwResult)
				return false;
		}
	}

	return true;
}

void NetworkRecvNew(DWORD sessionID)
{
	int iResult;
	char RecvBuff[DEFAULT_RING_BUF_SIZE];
	st_NETWORK_SESSION *pClient;

	pClient = FindClient(sessionID);
	if (NULL == pClient)
	{
		return;
	}

	iResult = recv(pClient->Socket, RecvBuff, DEFAULT_RING_BUF_SIZE, 0);

	if (iResult == SOCKET_ERROR)
	{
		closesocket(pClient->Socket);
		return;
	}

	if (iResult == 0)
	{
		closesocket(pClient->Socket);
		return;
	}

	if (0 < iResult)
	{
		pClient->RecvBuffer.Enqueue(RecvBuff, iResult);

		while (1)
		{
			iResult = NetworkCompleteRecvNew(pClient);

			if (1 == iResult)
				break;

			if (0xff == iResult)
				return;
		}
	}
}

bool NetworkSend(int sessionArrCount)
{
	int result;
	char				chBuff[DEFAULT_RING_BUF_SIZE];
	st_NETWORK_SESSION *stpSession = &g_netSession[sessionArrCount];
	DWORD				dwSendSize;

	dwSendSize = stpSession->SendBuffer.GetUseSize();
	dwSendSize = min(DEFAULT_RING_BUF_SIZE, dwSendSize);

	if (0 >= dwSendSize)
		return true;

	stpSession->SendBuffer.Peek(chBuff, dwSendSize);

	result = send(stpSession->Socket, chBuff, dwSendSize, 0);
	
	if (result == SOCKET_ERROR)
	{
		if (WSAEWOULDBLOCK == WSAGetLastError())
		{
			return true;
		}
		return false;
	}
	else
	{
		if ((int)dwSendSize < result)
		{
			return false;
		}
		else
		{
			stpSession->SendBuffer.RemoveData(dwSendSize);
		}
	}
	return true;
}

void NetworkSendNew(DWORD sessionID)
{
	int iResult;
	char SendBuff[DEFAULT_RING_BUF_SIZE];
	st_NETWORK_SESSION *pClient;
	int iSendSize;

	pClient = FindClient(sessionID);
	if (NULL == pClient)
	{
		return;
	}

	iSendSize = pClient->SendBuffer.GetUseSize();
	iSendSize = min(DEFAULT_RING_BUF_SIZE, iSendSize);

	if (0 >= iSendSize)
		return;

	pClient->SendBuffer.Peek(SendBuff, iSendSize);

	iResult = send(pClient->Socket, SendBuff, iSendSize, 0);

	if (iResult == SOCKET_ERROR)
	{
		if (WSAEWOULDBLOCK == WSAGetLastError())
		{
			return;
		}
		closesocket(pClient->Socket);
		return;
	}
	else
	{
		{
			// 송신작업 완료
			// 링버퍼의 큐의 데이터를 보낸 크기만큼 비워주기.
			pClient->SendBuffer.RemoveData(iSendSize);
		}
	}
	return;
}

DWORD NetworkCompleteRecv(DWORD dwSessionID)
{
	st_NETWORK_SESSION			*stpSession = GetSession(dwSessionID);
	st_PACKET_HEADER			stPacketHeader;
	int packetHeaderSize = sizeof(st_PACKET_HEADER);
	int RecvBufferSize;

	RecvBufferSize = stpSession->RecvBuffer.GetUseSize();

	if (packetHeaderSize > RecvBufferSize)
		return 1;

	stpSession->RecvBuffer.Peek((char*)&stPacketHeader, packetHeaderSize);

	if (dfPACKET_CODE != stPacketHeader.byCode)
		return 0xff;

	stpSession->RecvBuffer.RemoveData(packetHeaderSize);

	CPacket cPacket;

	if (stPacketHeader.wPayloadSize > 0)
	{
		if (!stpSession->RecvBuffer.Dequeue(cPacket.GetBufferPtr(), stPacketHeader.wPayloadSize))
			return 0xff;

		cPacket.MoveWritePos(stPacketHeader.wPayloadSize);
	}

	if (!PacketProc(stpSession->dwSessionID, stPacketHeader.wMsgType, &cPacket))
		return 0xff;

	return 0;
}

DWORD NetworkCompleteRecvNew(st_NETWORK_SESSION* pClient)
{
	st_PACKET_HEADER			stPacketHeader;
	int packetHeaderSize = sizeof(st_PACKET_HEADER);

	int RecvBufferSize;
	RecvBufferSize = pClient->RecvBuffer.GetUseSize();

	if (packetHeaderSize > RecvBufferSize)
		return 1;

	pClient->RecvBuffer.Peek((char*)&stPacketHeader, packetHeaderSize);

	if (dfPACKET_CODE != stPacketHeader.byCode)
		return 0xff;

	if (stPacketHeader.wPayloadSize + packetHeaderSize > RecvBufferSize)
	{
		return 1;
	}

	pClient->RecvBuffer.RemoveData(packetHeaderSize);

	CPacket cPacket;

	if (stPacketHeader.wPayloadSize > 0)
	{
		if (stPacketHeader.wPayloadSize != pClient->RecvBuffer.Dequeue(cPacket.GetBufferPtr(), stPacketHeader.wPayloadSize))
			return 0xff;

		cPacket.MoveWritePos(stPacketHeader.wPayloadSize);
	}

	BYTE byCheckSum = stPacketHeader.byCheckSum;
	if (byCheckSum != stPacketHeader.byCheckSum)
	{
		WriteConsoleLog("Packet CheckSum Error : %d", byCheckSum);
		return 0xff;
	}

	if (!PacketProc(pClient->dwSessionID, stPacketHeader.wMsgType, &cPacket))
		return 0xff;

	return 0;
}


bool SendPacket(DWORD dwSessionID, CPacket *packet)
{
	st_NETWORK_SESSION *stpSession = GetSession(dwSessionID);

	if (NULL == stpSession)
		return FALSE;

	stpSession->SendBuffer.Enqueue(packet->GetBufferPtr(), packet->GetDataSize());

	return true;
}


bool SendPacketAll(CPacket *packet)
{
	for (int i = 0; i < dfNETWORK_MAX_USER; i++)
	{
		if (g_netSession[i].bConnect)
		{
			g_netSession[i].SendBuffer.Enqueue(packet->GetBufferPtr(), packet->GetDataSize());
		}
	}
	return true;
}


st_NETWORK_SESSION* FindClient(DWORD sessionID)
{
	st_NETWORK_SESSION *pClient = NULL;
	map<DWORD, st_NETWORK_SESSION *>::iterator ClientIter;
	for (ClientIter = g_ClientMap.begin(); ClientIter != g_ClientMap.end();)
	{
		if (ClientIter->first == sessionID)
		{
			pClient = ClientIter->second;
			break;
		}
	}

	return pClient;
}
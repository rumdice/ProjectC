#include "stdafx.h"
#include "Packet.h"
#include "PacketProc.h"
#include "PacketDefine.h"
#include "resource.h"
#include "Network.h"
#include "Contents.h"

bool PacketProc(WORD byPacketType, CPacket *packet)
{
	switch (byPacketType)
	{
	case df_RES_LOGIN:
		PacketProc_Login(packet);
		break;
	case df_RES_ROOM_LIST:
		PacketProc_RoomList(packet);
		break;
	case df_RES_ROOM_CREATE:
		PacketProc_CreateRoom(packet);
		break;
	}

	return TRUE;
}

BOOL PacketProc_Login(CPacket *sPacket)
{
	BYTE loginResult;
	DWORD dwSessionID;

	*sPacket >> loginResult;
	*sPacket >> dwSessionID;

	g_userID = dwSessionID;
	SetDlgItemInt(g_hWndLobby, IDC_USER_ID, g_userID, false);

	CPacket cPacket;
	PacketCreate_RoomList(&cPacket);

	SendPacket(&cPacket);

	return TRUE;
}

bool PacketProc_RoomList(CPacket *sPacket)
{
	short roomCount;
	*sPacket >> roomCount;

	HWND hEditBox;
	hEditBox = GetDlgItem(g_hWndLobby, IDC_ROOM_LIST);
	SendMessage(hEditBox, LB_RESETCONTENT, 0, 0);

	for (int i = 0; i < roomCount; i++)
	{
		DWORD roomid;
		*sPacket >> roomid;

		short roomnamesize;
		*sPacket >> roomnamesize;

		wchar_t *roomName = (wchar_t*)malloc(sizeof(wchar_t) * roomnamesize * 2); 
		int roomNameSizeBytes = sPacket->GetData(roomName, (roomnamesize * 2)); 

		BYTE joinCount;
		*sPacket >> joinCount;

		int index = SendMessage(hEditBox, LB_ADDSTRING, 0, (LPARAM)roomName);
		SendMessage(hEditBox, LB_SETITEMDATA, (WPARAM)index, (LPARAM)joinCount);
	}

	return true;
}

bool PacketProc_CreateRoom(CPacket *cPacket)
{
	BYTE result; 
	DWORD roomID;
	short roomNameSize;

	*cPacket >> result;
	*cPacket >> roomID;
	*cPacket >> roomNameSize;

	
	wchar_t *roomName = (wchar_t*)malloc(sizeof(wchar_t) * (roomNameSize * 2) - 2); 
	int roomNameSizeBytes = cPacket->GetData(roomName, (roomNameSize * 2) - 2); 

	CreateRoom(roomName);

	if (roomName != NULL)
	{
		free(roomName);
	}

	CPacket nPacket;
	PacketCreate_RoomList(&nPacket);

	SendPacket(&nPacket);

	return true;
}

void PacketCreate_RoomList(CPacket *pPacket)
{
	st_PACKET_HEADER stPacketHeader;
	stPacketHeader.byCode = dfPACKET_CODE;
	stPacketHeader.byCheckSum = 0; 
	stPacketHeader.wMsgType = df_REQ_ROOM_LIST;
	stPacketHeader.wPayloadSize = 0; 

	pPacket->PutData((char *)&stPacketHeader, sizeof(stPacketHeader));
}

void PacketCreate_CreateRoom(CPacket *pPacket, wchar_t* roomName)
{
	short roomNameSize = wcslen(roomName);

	st_PACKET_HEADER stPacketHeader;
	stPacketHeader.byCode = dfPACKET_CODE;
	stPacketHeader.byCheckSum = 0; 
	stPacketHeader.wMsgType = df_REQ_ROOM_CREATE;
	stPacketHeader.wPayloadSize = 2 + (roomNameSize * 2);

	pPacket->PutData((char *)&stPacketHeader, sizeof(stPacketHeader));

	*pPacket << roomNameSize;				
	pPacket->PutData(roomName, roomNameSize * 2);
}
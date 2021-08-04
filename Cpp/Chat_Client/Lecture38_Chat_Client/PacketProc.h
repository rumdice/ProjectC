#pragma once
#include "stdafx.h"
#include "Packet.h"

extern DWORD g_userID;
extern HWND			g_hWndLobby;

bool PacketProc(WORD byPacketType, CPacket *packet);

BOOL PacketProc_Login(CPacket *cPacket);		
bool PacketProc_RoomList(CPacket *cPacket);		
bool PacketProc_CreateRoom(CPacket *cPacket);	

void PacketCreate_RoomList(CPacket *pPacket);	
void PacketCreate_CreateRoom(CPacket *pPacket, wchar_t* roomName);



#pragma once

#include "stdafx.h"
#include "Packet.h"


void PacketProcConnect(DWORD sessionID);
void PacketProcDisconnect(DWORD sessionID);

bool PacketProcLogin(DWORD sessionID, CPacket *packet);
bool PacketProcRoomList(DWORD dwSessionID, CPacket *packet);
bool PacketProcCreateRoom(DWORD dwSessionID, CPacket *packet);
bool PacketProcRoomEnter(DWORD dwSessionID, CPacket *packet);
bool PacketProcChat(DWORD dwSessionID, CPacket *packet);
bool PacketProcRoomLeave(DWORD dwSessionID, CPacket *packet);


bool PacketProc(DWORD dwSessionID, WORD byPacketType, CPacket *packet);

BYTE GetCheckSum(unsigned short msgType, char *pPayLoad, int payloadSize);

void MakePacketLogin();

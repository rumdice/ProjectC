#include "PacketProc.h"
#include "PacketDefine.h"
#include "Network.h"
#include "Util.h"
#include "Contents.h"

void PacketProcConnect(DWORD sessionID)
{
	WriteConsoleLog("# PACKET_CONNECT # / SessionID:%d", sessionID);

	CPacket packet;

	st_PACKET_HEADER stPacketHeader;
	stPacketHeader.byCode = dfPACKET_CODE;
	stPacketHeader.byCheckSum = GetCheckSum(df_RES_LOGIN, (char *)sessionID, sizeof(sessionID));
	stPacketHeader.wMsgType = df_RES_LOGIN;
	stPacketHeader.wPayloadSize = 5; // (프로토콜 명시) 사용자 ID 길이(4) + 결과코드(1) 이므로 5

	packet.PutData((char *)&stPacketHeader, sizeof(stPacketHeader));

	packet << (BYTE)df_RESULT_LOGIN_ETC;
	packet << sessionID;

	SendPacket(sessionID, &packet);
}

void PacketProcDisconnect(DWORD sessionID)
{

}

bool PacketProcLogin(DWORD sessionID, CPacket *clpPacket)
{
	WriteConsoleLog("PacketProcLogin # / SessionID:%d", sessionID);

	int id;
	*clpPacket >> id;

	CPacket packet;
	st_PACKET_HEADER stPacketHeader;
	stPacketHeader.byCode = dfPACKET_CODE;
	stPacketHeader.byCheckSum = GetCheckSum(df_RES_LOGIN, (char *)sessionID, sizeof(sessionID));
	stPacketHeader.wMsgType = df_RES_LOGIN;
	stPacketHeader.wPayloadSize = 4; // 4byte (프로토콜 명시) 사용자 ID를 돌려주므로 4다

	packet.PutData((char *)&stPacketHeader, sizeof(stPacketHeader));

	packet << df_RESULT_LOGIN_OK;
	packet << sessionID;

	SendPacket(sessionID, &packet);

	return true;
}

bool PacketProcRoomList(DWORD dwSessionID, CPacket *packet)
{
	WriteConsoleLog("# PACKET_ROOMLIST # / SessionID:%d", dwSessionID);
	short roomCount = RoomList.size();
	BYTE totalRoomByteSize = 0;

	for (auto room : RoomList)
	{
		int rommIdSize = sizeof(room.roomID);			// 4 byte
		int roNameSize = sizeof(room.roomNameSize);		// 2 byte
		int nameSize = room.roomNameSize * 2;			// 2~n byte * 2
		int joinSize = sizeof(room.joinCount);			// 1 byte

		int roomPacketSize = rommIdSize + roNameSize + nameSize + joinSize;

		totalRoomByteSize += roomPacketSize;
	}


	
	st_PACKET_HEADER stPacketHeader;
	stPacketHeader.byCode = dfPACKET_CODE;
	stPacketHeader.byCheckSum = GetCheckSum(df_RES_ROOM_LIST, (char *)dwSessionID, sizeof(dwSessionID)); 
	stPacketHeader.wMsgType = df_RES_ROOM_LIST;
	stPacketHeader.wPayloadSize = 2 + totalRoomByteSize;

	packet->PutData((char *)&stPacketHeader, sizeof(stPacketHeader));

	*packet << roomCount; // 방의 개수 (하드코딩2)

	// 방의 갯수만큼 방의 정보를 패킷에 담아 줘야 한다.
	for (auto room : RoomList)
	{
		DWORD rid = room.roomID;
		short rnSize = room.roomNameSize;
		BYTE joinC = room.joinCount;

		*packet << rid;
		*packet << rnSize;

		wchar_t *roomName = (wchar_t*)malloc(sizeof(wchar_t) * ((rnSize * 2)));
		roomName = room.roomName;
		int a = lstrlenW(roomName);

		packet->PutData(roomName, ((rnSize * 2)));
		*packet << room.joinCount;
	}

	SendPacket(dwSessionID, packet);

	return true;
}

bool PacketProcCreateRoom(DWORD dwSessionID, CPacket *clpPacket)
{
	WriteConsoleLog("# PACKET_CREATEROOM # / SessionID:%d", dwSessionID);

	short roomNameSize;
	*clpPacket >> roomNameSize;

	wchar_t *roomName = (wchar_t*)malloc(sizeof(wchar_t) * (roomNameSize * 2));
	int roomNameSizeBytes = clpPacket->GetData(roomName, (roomNameSize * 2));

	CPacket packet;

	st_PACKET_HEADER stPacketHeader;
	stPacketHeader.byCode = dfPACKET_CODE;
	stPacketHeader.byCheckSum = GetCheckSum(df_RES_ROOM_CREATE, (char *)dwSessionID, sizeof(dwSessionID));
	stPacketHeader.wMsgType = df_RES_ROOM_CREATE;
	stPacketHeader.wPayloadSize = 1 + 4 + 2 + roomNameSizeBytes;

	packet.PutData((char *)&stPacketHeader, sizeof(stPacketHeader));

	DWORD roomID = g_RoomID;
	packet << (BYTE)df_RESULT_ROOM_CREATE_OK; // 방생성 결과 코드
	packet << roomID;
	packet << roomNameSize;
	packet.PutData(roomName, (roomNameSize * 2));

	SendPacketAll(&packet);

	st_Room room;
	room.roomID = roomID;
	room.roomNameSize = roomNameSize;
	room.roomName = roomName;
	room.joinCount = 0;
	
	RoomList.push_back(room);

	return true;
}

bool PacketProcRoomEnter(DWORD dwSessionID, CPacket *packet)
{
	WriteConsoleLog("PacketProcRoomEnter # / SessionID:%d", dwSessionID);
	return true;
}

bool PacketProcChat(DWORD dwSessionID, CPacket *packet)
{
	WriteConsoleLog("PacketProcChat # / SessionID:%d", dwSessionID);
	return true;
}

bool PacketProcRoomLeave(DWORD dwSessionID, CPacket *packet)
{
	WriteConsoleLog("PacketProcRoomLeave # / SessionID:%d", dwSessionID);
	return true;
}

bool PacketProc(DWORD dwSessionID, WORD byPacketType, CPacket *packet)
{
	switch (byPacketType)
	{
	case df_REQ_LOGIN:
		return PacketProcLogin(dwSessionID, packet);
		break;

	case df_REQ_ROOM_LIST:
		return PacketProcRoomList(dwSessionID, packet);
		break;

	case df_REQ_ROOM_CREATE:
		return PacketProcCreateRoom(dwSessionID, packet);
		break;

	case df_REQ_ROOM_ENTER:
		return PacketProcRoomEnter(dwSessionID, packet);
		break;

	case df_REQ_CHAT:
		return PacketProcChat(dwSessionID, packet);
		break;

	case df_REQ_ROOM_LEAVE:
		return PacketProcRoomLeave(dwSessionID, packet);
		break;
	}

	return true;
}


BYTE GetCheckSum(unsigned short msgType, char *pPayLoad, int payloadSize)
{
	BYTE checkSum = 0;
	checkSum = (int(msgType) + (int)pPayLoad) % 256;

	return checkSum;
}
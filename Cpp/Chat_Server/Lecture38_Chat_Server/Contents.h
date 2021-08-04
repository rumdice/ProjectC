#pragma once

#include "stdafx.h"

DWORD	g_RoomID = 1;

struct st_Room
{
	DWORD roomID;
	short roomNameSize;
	wchar_t* roomName;
	BYTE joinCount;			
};

list<st_Room>RoomList;

void MakeTempRoomList() // 하드코딩된 값으로 결과 확인
{
	st_Room stRoom;
	stRoom.roomID = 1;
	stRoom.roomNameSize = 6;
	stRoom.roomName = L"aabbcc";
	stRoom.joinCount = 2;

	RoomList.push_back(stRoom);

	st_Room stRoom1;
	stRoom1.roomID = 2;
	stRoom1.roomNameSize = 8;
	stRoom1.roomName = L"AABBCCDD";
	stRoom1.joinCount = 3;

	RoomList.push_back(stRoom1);
}
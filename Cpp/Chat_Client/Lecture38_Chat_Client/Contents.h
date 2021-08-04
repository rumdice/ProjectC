#pragma once

struct st_Room
{
	DWORD roomID;
	short roomNameSize;
	wchar_t* roomName;
	BYTE joinCount;				
	list<wchar_t*>UserNameList;
};

void MakeRoom(wchar_t* roomName);
void CreateRoom(wchar_t* roomName);
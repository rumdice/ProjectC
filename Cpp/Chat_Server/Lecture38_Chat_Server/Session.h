#pragma once

#include "stdafx.h"
#include "RingBuffer.h"

struct st_NETWORK_SESSION
{
	BOOL		bConnect;
	SOCKET		Socket;
	DWORD		dwSessionID;

	CRingBuffer	RecvBuffer;
	CRingBuffer SendBuffer;
};

void SessionInit();
void RemoveSession(DWORD sessionID);
int AddNewSession(SOCKET socket);
st_NETWORK_SESSION* GetSession(DWORD sessionID);
st_NETWORK_SESSION* GetEmptySession();

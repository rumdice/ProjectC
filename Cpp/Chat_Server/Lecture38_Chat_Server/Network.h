#ifndef NETWORK_H
#define NETWORK_H

#include "stdafx.h"
#include "Packet.h"
#include "Session.h"

// select ¸ðµ¨

bool NetworkInit();				
bool NetworkPortOpen();			
void NetworkRelease();			
bool NetworkProcess();			

SOCKET NetworkAccept();								
bool NetworkRecv(int sessionArrCount);				
bool NetworkSend(int sessionArrCount);				

DWORD NetworkCompleteRecv(DWORD dwSessionID);		
//DWORD NetworkCompleteSend(DWORD dwSessionID);		

bool SendPacket(DWORD dwSessionID, CPacket *packet);
bool SendPacketAll(CPacket *packet);


void NetworkProcessNew();
void SelectSocket(DWORD*, SOCKET*, FD_SET*, FD_SET*);
void NetworkRecvNew(DWORD sessionID);
void NetworkSendNew(DWORD sessionID);


DWORD NetworkCompleteRecvNew(st_NETWORK_SESSION* pClient);

st_NETWORK_SESSION* FindClient(DWORD sessionID);

#endif
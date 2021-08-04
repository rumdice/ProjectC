
#include "stdafx.h"


#define dfNETWORK_PORT		6000

#define dfNICK_MAX_LEN		15			
#define dfPACKET_CODE		0x89

#pragma pack(push, 1)   

struct st_PACKET_HEADER
{
	BYTE	byCode;
	BYTE	byCheckSum;

	WORD	wMsgType;
	WORD	wPayloadSize;
};

#pragma pack(pop)

#define df_REQ_LOGIN	1
#define df_RES_LOGIN	2

#define df_RESULT_LOGIN_OK		1
#define df_RESULT_LOGIN_DNICK	2
#define df_RESULT_LOGIN_MAX		3
#define df_RESULT_LOGIN_ETC		4


#define df_REQ_ROOM_LIST	3
#define df_RES_ROOM_LIST	4

#define df_REQ_ROOM_CREATE	5
#define df_RES_ROOM_CREATE	6

#define df_RESULT_ROOM_CREATE_OK		1
#define df_RESULT_ROOM_CREATE_DNICK		2
#define df_RESULT_ROOM_CREATE_MAX		3
#define df_RESULT_ROOM_CREATE_ETC		4

#define df_REQ_ROOM_ENTER		7
#define df_RES_ROOM_ENTER		8

#define df_RESULT_ROOM_ENTER_OK			1
#define df_RESULT_ROOM_ENTER_NOT		2
#define df_RESULT_ROOM_ENTER_MAX		3
#define df_RESULT_ROOM_ENTER_ETC		4

#define df_REQ_CHAT				9
#define df_RES_CHAT				10

#define df_REQ_ROOM_LEAVE		11
#define df_RES_ROOM_LEAVE		12

#define df_RES_ROOM_DELETE		13
#define df_RES_USER_ENTER		14

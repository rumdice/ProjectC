
#include "Session.h"
#include "define.h"
#include "Util.h"

st_NETWORK_SESSION	g_netSession[dfNETWORK_MAX_USER];	 
DWORD				g_netSessionID = 1;					 

void SessionInit()
{
	for (int i = 0; i < dfNETWORK_MAX_USER; i++)
	{
		g_netSession[i].bConnect = FALSE;
		g_netSession[i].Socket = INVALID_SOCKET;
		g_netSession[i].dwSessionID = 0;
		
		g_netSession[i].RecvBuffer.ClearBuffer();
		g_netSession[i].RecvBuffer.ClearBuffer();
	}
}

void RemoveSession(DWORD sessionID)
{
	st_NETWORK_SESSION	*stpSession = NULL;
	stpSession = GetSession(sessionID);

	if (stpSession != NULL)
	{
		stpSession->Socket = INVALID_SOCKET;
		stpSession->bConnect = FALSE;
		stpSession->dwSessionID = 0;
	}
}

int AddNewSession(SOCKET socket)
{
	int newSessionID = 0;
	st_NETWORK_SESSION	*stpSession = NULL;
	stpSession = GetEmptySession();

	if (stpSession == NULL)
	{
		closesocket(socket);
		return 0;
	}

	stpSession->bConnect = true;
	stpSession->Socket = socket;
	stpSession->dwSessionID = g_netSessionID;

	stpSession->RecvBuffer.ClearBuffer();
	stpSession->SendBuffer.ClearBuffer();

	newSessionID = g_netSessionID;

	g_netSessionID++;

	return newSessionID;
}

st_NETWORK_SESSION *GetSession(DWORD sessionID)
{
	st_NETWORK_SESSION	*stpSession = NULL;

	for (int i = 0; i < dfNETWORK_MAX_USER; i++)
	{
		if (sessionID == g_netSession[i].dwSessionID)
			return &g_netSession[i];
	}

	return NULL;
}

st_NETWORK_SESSION *GetEmptySession()
{
	st_NETWORK_SESSION	*stpSession = NULL;

	for (int i = 0; i < dfNETWORK_MAX_USER; i++)
	{
		if (!g_netSession[i].bConnect)
			return &g_netSession[i];
	}

	return NULL;
}

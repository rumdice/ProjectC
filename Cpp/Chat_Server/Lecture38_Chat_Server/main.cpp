#include "stdafx.h"
#include "Network.h"
#include "Session.h"

int main()
{
	NetworkInit();
	NetworkPortOpen();

	SessionInit();

	while (1)
	{
		NetworkProcess();
	}

	NetworkRelease();
}
#pragma once
namespace RumdaLib
{
	class Endpoint
	{
	public:
		Endpoint();
		Endpoint(const char* address, int port);
		~Endpoint();

		static Endpoint Any;
		sockaddr_in _endpoint; // ipv4
	};
}

//#pragma comment(lib, "Ws2_32.lib")
//#pragma comment(lib, "mswsock.lib")
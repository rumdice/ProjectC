#include "pch.h"

namespace RumdaLib
{
	Endpoint::Endpoint()
	{
		memset(&_endpoint, 0, sizeof(_endpoint));
		_endpoint.sin_family = AF_INET;
	}

	Endpoint::Endpoint(const char* address, int port)
	{
		memset(&_endpoint, 0, sizeof(_endpoint));
		_endpoint.sin_family = AF_INET;

		inet_pton(AF_INET, address, &_endpoint.sin_addr);
		_endpoint.sin_port = htons((uint16_t)port);
	}

	Endpoint::~Endpoint()
	{
	}

	Endpoint Endpoint::Any;
}
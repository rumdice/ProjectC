#pragma once

#include <iostream>
#include <chrono>
#include <list>
#include <string>

// lib���� �ִ� ����� �� �߰��ؾ߸� �ұ�?
#include <cstdint>
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <MSWSock.h>

#include <thread>
#include <unordered_map>
#include <memory>

// ����� ���� ��ũ ���� �߻�
//#include <boost/asio.hpp>
//#include <boost/bind.hpp>

#include "Libs/header/test.h"
#include "Libs/header/singleton.hpp"
#include "Libs/header/memorypool.hpp"
#include "Libs/header/streambuffer.h"
#include "Libs/header/messagequeue.hpp"
#include "Libs/header/endpoint.h"
#include "Libs/header/socket.h"
#include "Libs/header/iocp.h"

#pragma comment (lib, "RumdaLib.lib")

#include "testserver.h"
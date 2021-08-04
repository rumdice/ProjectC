#include "stdafx.h"

std::unordered_map<RemoteClient*, std::shared_ptr<RemoteClient>> remoteClients;


TestServer::TestServer()
{
	// 별도의 메서드를 마련하는게 더 좋을듯. start같은
	Iocp iocp2;
	iocp = iocp2; // ..?

	Socket socket(SocketType::Tcp);
	listenSocket = socket;

	remoteClientCandidate = std::make_shared<RemoteClient>(SocketType::Tcp);
}

TestServer::~TestServer()
{
}

void TestServer::Init()
{
	listenSocket.Bind(Endpoint("0.0.0.0", 5555));
	listenSocket.Listen();

	iocp.Regist(listenSocket, nullptr);

	listenSocket._isReadOverlapped = true;
}

// 자료 참조한 testserver prototype. 기능개선 및 추가할 사항이 있는지 체크해보기
bool TestServer::Run()
{
	while (true) // TODO : 일단은 무한루프 - 차후에 강종 플레그 추가하기
	{
		IocpEvent readEvents;
		iocp.Work(readEvents, 100);

		for (int i = 0; i < readEvents._eventCount; i++)
		{
			auto& readEvent = readEvents._events[i];
			if (readEvent.lpCompletionKey == 0) // 리슨 소켓
			{
				listenSocket._isReadOverlapped = false;
				// Win32 AcceptEx 
				if (remoteClientCandidate->tcpConnection.UpdateAcceptContext(listenSocket) != 0)
				{
					listenSocket.Close();
				}
				else
				{
					auto remoteClient = remoteClientCandidate;
					iocp.Regist(remoteClient->tcpConnection, remoteClient.get());

					if (remoteClient->tcpConnection.ReceiveOverlapped() != 0 && WSAGetLastError() != ERROR_IO_PENDING)
					{
						// error. 소켓을 정리하자. 그리고 그냥 버리자.
						remoteClient->tcpConnection.Close();
					}
					else
					{
						remoteClient->tcpConnection._isReadOverlapped = true;
						remoteClients.insert({ remoteClient.get(), remoteClient });
						std::cout << "Client joined. There are " << remoteClients.size() << " connections.\n";
					}

					// 계속해서 소켓 받기를 해야 하므로 리슨소켓도 overlapped I/O를 걸자.
					remoteClientCandidate = std::make_shared<RemoteClient>(SocketType::Tcp);
					if (!listenSocket.AcceptOverlapped(remoteClientCandidate->tcpConnection) && WSAGetLastError() != ERROR_IO_PENDING)
					{
						// error. 에러나면 리슨소켓 불능 상태로 남기자. 
						listenSocket.Close();
					}
					else
					{
						// 리슨소켓은 연결이 들어옴을 기다리는 상태가 되었다.
						listenSocket._isReadOverlapped = true;
					}
				}
			}
			else  // TCP 연결 소켓이면
			{
				// 받은 데이터를 그대로 회신한다. - 프로코톨 처리 필요
				auto remoteClient = remoteClients[(RemoteClient*)readEvent.lpCompletionKey];

				if (readEvent.dwNumberOfBytesTransferred <= 0)
				{
					int a = 0;
				}

				if (remoteClient)
				{
					// 이미 수신된 상태이다. 수신 완료된 것을 그냥 꺼내 쓰자.
					remoteClient->tcpConnection._isReadOverlapped = false;
					int ec = readEvent.dwNumberOfBytesTransferred;

					if (ec <= 0)
					{
						// error
						ProcessClientLeave(remoteClient);
					}
					else
					{
						// 이미 수신된 상태이다. 수신 완료된 것을 그냥 꺼내 쓰자.
						char* echoData = remoteClient->tcpConnection._receiveBuffer;
						int echoDataLength = ec;

						remoteClient->tcpConnection.Send(echoData, echoDataLength);

						if (remoteClient->tcpConnection.ReceiveOverlapped() != 0 && WSAGetLastError() != ERROR_IO_PENDING)
						{
							ProcessClientLeave(remoteClient);
							return false; // error
						}
						else
						{
							remoteClient->tcpConnection._isReadOverlapped = true;
						}
					}
				}
			}
		}
	}

	return true;
}

void TestServer::Exit()
{
	listenSocket.Close();
	for (auto i : remoteClients)
	{
		i.second->tcpConnection.Close();
	}

	// cout << "서버를 종료하고 있습니다...\n";
	while (remoteClients.size() > 0 || listenSocket._isReadOverlapped)
	{
		for (auto i = remoteClients.begin(); i != remoteClients.end();)
		{
			if (!i->second->tcpConnection._isReadOverlapped)
			{
				i = remoteClients.erase(i);
			}
			else
				i++; // 좀 더 기다려보자.
		}

		IocpEvent readEvents;
		iocp.Work(readEvents, 100);

		// 받은 이벤트 각각을 처리합니다.
		for (int i = 0; i < readEvents._eventCount; i++)
		{
			auto& readEvent = readEvents._events[i];
			if (readEvent.lpCompletionKey == 0) // 리슨소켓이면
			{
				listenSocket._isReadOverlapped = false;
			}
			else
			{
				auto remoteClient = remoteClients[(RemoteClient*)readEvent.lpCompletionKey];
				if (remoteClient)
				{
					remoteClient->tcpConnection._isReadOverlapped = false;
				}
			}
		}
	}

	// cout << "서버 종료\n";
}

void TestServer::ProcessClientLeave(std::shared_ptr<RemoteClient> remoteClient)
{
	// 에러 혹은 소켓 종료이다.
	// 해당 소켓은 제거해버리자. 
	remoteClient->tcpConnection.Close();
	remoteClients.erase(remoteClient.get());
	std::cout << "Client left. There are " << remoteClients.size() << " connections.\n";
}

#pragma once

using namespace RumdaLib;

class RemoteClient
{
public:
	std::shared_ptr<std::thread> thread; // 클라이언트 처리를 하는 스레드 1개
	Socket tcpConnection;		// accept한 TCP 연결

	RemoteClient() {}
	RemoteClient(SocketType socketType) :tcpConnection(socketType) {}
};


// CTestServer, CGameServer, CLobbyServer, CChatServer ....
class TestServer
{
public:
	TestServer();
	virtual ~TestServer();

	void Init();
	bool Run();
	void Exit();

private:
	//...
	Iocp iocp;
	Socket listenSocket;
	std::shared_ptr<RemoteClient> remoteClientCandidate;

	void ProcessClientLeave(std::shared_ptr<RemoteClient> remoteClient);
};
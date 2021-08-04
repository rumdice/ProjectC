#pragma once

using namespace RumdaLib;

class RemoteClient
{
public:
	std::shared_ptr<std::thread> thread; // Ŭ���̾�Ʈ ó���� �ϴ� ������ 1��
	Socket tcpConnection;		// accept�� TCP ����

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
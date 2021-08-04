#pragma once
namespace RumdaLib
{
	class Socket;
	class IocpEvent;

	class Iocp
	{
	public:
		Iocp();
		~Iocp();

		bool Regist(Socket& socket, void* completionKey);
		void Work(IocpEvent& output, int timeoutMs);

	public:
		static const int MaxEventCount = 100;

	private:
		HANDLE _iocp;
		int _threadCount;
	};

	class IocpEvent
	{
	public:
		
		// GetQueuedCompletionStatus으로 꺼내온 이벤트들
		OVERLAPPED_ENTRY _events[Iocp::MaxEventCount];
		int _eventCount;
	};
}
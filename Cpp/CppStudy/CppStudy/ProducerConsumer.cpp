#include "stdafx.h"

// 생산자 소비자 모델 공부 (memoryOrder 공부 포함) 
// 스레드 공유 전역 변수들
std::mutex mx;
std::condition_variable cv;	// 조건변수 - 특별한 조건이 만족되었을 경우 스레드가 동작 가능하게 해주는 유저 영역 동기화 객체. 결국 스레드 간 의 동기화를 가능하게 해준다.
std::queue<int> q;
// 상태 값 전역변수
bool finished = false;


struct stPayload
{
	int num;
	std::string str;
};
stPayload g_payload;
std::atomic<int> g_guard; // memory order 가드용 atomic 변수



// 생산자는 락을 걸고 입력 받은 수n 만큼 큐에 데이터를 넣는다.
void producer(int n)
{
	for (int i = 0; i < n; ++i)
	{
		std::lock_guard<std::mutex> lock(mx);	// 락을 건다
		q.push(i);
		std::cout << "pushing " << i << std::endl;
	}
	cv.notify_all();	// 락을 풀고 신호를 보낸다.

	std::lock_guard<std::mutex> lock(mx);


	//// MemoryOrder
	// 전역변수 값 쓰기
	g_payload.num = 10;
	g_payload.str = "Message";

	// 지금까지 쓴 내용들이 Acquire 이후에 보여진다.
	g_guard.store(1, std::memory_order_release);
	/////


	finished = true;
	cv.notify_all();
}

// 소비자는 신호를 기다리면서 큐가 빌때까지 데이터를 꺼낸다.
void consumer()
{
	stPayload result; // MemoryOrder 체크 변수

	while (true)
	{
		std::unique_lock<std::mutex> lock(mx); // 락을 건다.
		cv.wait(lock, [] {return finished || !q.empty(); }); // 신호를 기다면서 신호가 오면 락을 푼다.

		while (!q.empty())
		{
			std::cout << "consuming " << q.front() << std::endl;
			q.pop();
		}

		//// MemoryOrder
		int ready = g_guard.load(std::memory_order_acquire);
		// 이후부터는 Release 이전에 쓰여진 메모리 값들이 모두 제대로 보임
		if (ready != 0)
		{
			result.num = g_payload.num;
			result.str = g_payload.str;
		}
		//// MemoryOrder

		if (finished)
			break;
	}
}


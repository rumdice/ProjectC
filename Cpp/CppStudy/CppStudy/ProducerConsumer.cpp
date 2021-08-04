#include "stdafx.h"

// ������ �Һ��� �� ���� (memoryOrder ���� ����) 
// ������ ���� ���� ������
std::mutex mx;
std::condition_variable cv;	// ���Ǻ��� - Ư���� ������ �����Ǿ��� ��� �����尡 ���� �����ϰ� ���ִ� ���� ���� ����ȭ ��ü. �ᱹ ������ �� �� ����ȭ�� �����ϰ� ���ش�.
std::queue<int> q;
// ���� �� ��������
bool finished = false;


struct stPayload
{
	int num;
	std::string str;
};
stPayload g_payload;
std::atomic<int> g_guard; // memory order ����� atomic ����



// �����ڴ� ���� �ɰ� �Է� ���� ��n ��ŭ ť�� �����͸� �ִ´�.
void producer(int n)
{
	for (int i = 0; i < n; ++i)
	{
		std::lock_guard<std::mutex> lock(mx);	// ���� �Ǵ�
		q.push(i);
		std::cout << "pushing " << i << std::endl;
	}
	cv.notify_all();	// ���� Ǯ�� ��ȣ�� ������.

	std::lock_guard<std::mutex> lock(mx);


	//// MemoryOrder
	// �������� �� ����
	g_payload.num = 10;
	g_payload.str = "Message";

	// ���ݱ��� �� ������� Acquire ���Ŀ� ��������.
	g_guard.store(1, std::memory_order_release);
	/////


	finished = true;
	cv.notify_all();
}

// �Һ��ڴ� ��ȣ�� ��ٸ��鼭 ť�� �������� �����͸� ������.
void consumer()
{
	stPayload result; // MemoryOrder üũ ����

	while (true)
	{
		std::unique_lock<std::mutex> lock(mx); // ���� �Ǵ�.
		cv.wait(lock, [] {return finished || !q.empty(); }); // ��ȣ�� ��ٸ鼭 ��ȣ�� ���� ���� Ǭ��.

		while (!q.empty())
		{
			std::cout << "consuming " << q.front() << std::endl;
			q.pop();
		}

		//// MemoryOrder
		int ready = g_guard.load(std::memory_order_acquire);
		// ���ĺ��ʹ� Release ������ ������ �޸� ������ ��� ����� ����
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


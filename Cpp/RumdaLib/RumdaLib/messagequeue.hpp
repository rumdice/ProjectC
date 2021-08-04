// 페킷(mesage)를 담는 큐를 작성
#pragma once
#include <queue>

namespace RumdaLib
{
	template<typename T>
	class MessageQueue
	{
	public:
		MessageQueue();
		virtual ~MessageQueue();

		void Set(int size);
		bool Push(T* message);
		T* Pop();


	private:
		std::queue<T*> _queue; // 메시지를 담을 큐
	};


	template<typename T>
	inline MessageQueue<T>::MessageQueue()
	{
	}

	template<typename T>
	inline MessageQueue<T>::~MessageQueue()
	{
		while (!_queue.empty())
		{
			T* pQueue;
			pQueue = _queue.front();
			_queue.pop();
			delete pQueue;
		}
	}

	template<typename T>
	inline void MessageQueue<T>::Set(int size)
	{
		for (int i = 0; i < size; ++i)
		{
			_queue.push(new T);
		}
	}

	template<typename T>
	inline bool MessageQueue<T>::Push(T* message)
	{
		_queue.push(message);
		return true;
	}

	template<typename T>
	inline T* MessageQueue<T>::Pop()
	{
		T* pQueue;

		if (_queue.empty())
			return nullptr;

		pQueue = _queue.front();
		_queue.pop();

		return pQueue;
	}
}
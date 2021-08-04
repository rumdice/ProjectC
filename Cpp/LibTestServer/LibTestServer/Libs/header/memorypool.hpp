#pragma once
namespace RumdaLib
{
	template<typename T, int ALLOC_BLOCK_SIZE = 50>
	class MemoryPool
	{
	public:
		

	public:
		void Init()
		{

		}

		static void* operator new(size_t length)
		{
			if (!_Free)
			{
				alloc();
			}

			unsigned char* returnPointer = _Free;
			if (returnPointer != 0)
			{
				_Free = *reinterpret_cast<unsigned char**>(returnPointer);
			}

			return returnPointer;
		}

		static void operator delete(void* deletePointer)
		{
			*reinterpret_cast<unsigned char**>(deletePointer) = _Free;
			_Free = static_cast<unsigned char*>(deletePointer);
		}

	private:
		static void alloc()
		{
			_Free = new unsigned char[sizeof(T) * ALLOC_BLOCK_SIZE];

			unsigned char** current = reinterpret_cast<unsigned char**>(_Free);
			unsigned char* next = _Free;

			for (int i = 0; i < ALLOC_BLOCK_SIZE - 1; i++)
			{
				next += sizeof(T);
				*current = next;
				current = reinterpret_cast<unsigned char**>(next);
			}

			*current = 0;
		}

		static void deloc()
		{

		}

	private:
		static unsigned char* _Free;
	};

	template <typename T, int ALLOC_BLOCK_SIZE>
	unsigned char* MemoryPool<T, ALLOC_BLOCK_SIZE>::_Free;
}
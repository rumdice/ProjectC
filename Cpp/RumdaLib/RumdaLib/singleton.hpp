#pragma once
namespace RumdaLib
{
	template<typename T>
	class Singleton
	{
	protected:
		Singleton() {}
		virtual ~Singleton() {}

		static void Instance()
		{
			if (_instance == nullptr)
			{
				_instance = new T;
			}
		}

		static T* GetInstance()
		{
			return (_instance == nullptr) ? nullptr : _instance;
		}

	private:
		void operator delete(void* p)
		{
			delete _instance;
		}

	private:
		static T* _instance;

	};

	template<typename T> T* Singleton<T>::_instance = nullptr;
}
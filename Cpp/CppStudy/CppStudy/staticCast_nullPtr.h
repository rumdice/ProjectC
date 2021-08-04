#pragma once

static void staticCast_NullPtr()
{
	int a = 1;
	void* aptr = &a;
	char* cptr = static_cast<char*>(aptr); // void* Ÿ���� char* Ÿ������ ��ȯ�Ѵ�.
	if (cptr == nullptr)
	{
		std::cout << "nullptr"; // ����� ���� �ɸ� �� ����.
	}
	else
	{
		std::cout << "not nullptr"; // �翬�ϰԵ� ������ ���� �ɸ���.
	}


	void* bptr = nullptr;
	char* dptr = static_cast<char*>(bptr);
	if (dptr == nullptr)
	{
		std::cout << "nullptr";
	}
	else
	{
		std::cout << "not nullptr"; // ������ ���� �ɸ���.
	}

	//nullptr_t* eptr = static_cast<nullptr_t*>(aptr);
	void* fptr = static_cast<nullptr_t*>(aptr);
	if (fptr == nullptr)
	{
		std::cout << "nullptr";
	}
	else
	{
		std::cout << "not nullptr"; // ������ ���� �ɸ���.
	}

	// ��������� static_cast ���Ŀ� �ϴ� nullptr üũ�� �ƹ� �ǹ̰� ����.
	// ���� static_cast ������ nullptr���� üũ�ϴ°� �ǹ̰� ������ ���� �ʹ�.
}
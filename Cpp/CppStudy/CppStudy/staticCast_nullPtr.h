#pragma once

static void staticCast_NullPtr()
{
	int a = 1;
	void* aptr = &a;
	char* cptr = static_cast<char*>(aptr); // void* 타입을 char* 타입으로 변환한다.
	if (cptr == nullptr)
	{
		std::cout << "nullptr"; // 절대로 여기 걸릴 수 없다.
	}
	else
	{
		std::cout << "not nullptr"; // 당연하게도 무조건 여기 걸린다.
	}


	void* bptr = nullptr;
	char* dptr = static_cast<char*>(bptr);
	if (dptr == nullptr)
	{
		std::cout << "nullptr";
	}
	else
	{
		std::cout << "not nullptr"; // 무조건 여기 걸린다.
	}

	//nullptr_t* eptr = static_cast<nullptr_t*>(aptr);
	void* fptr = static_cast<nullptr_t*>(aptr);
	if (fptr == nullptr)
	{
		std::cout << "nullptr";
	}
	else
	{
		std::cout << "not nullptr"; // 무조건 여기 걸린다.
	}

	// 결론적으로 static_cast 이후에 하는 nullptr 체크는 아무 의미가 없다.
	// 차라리 static_cast 이전에 nullptr인지 체크하는게 의미가 있으면 있지 싶다.
}
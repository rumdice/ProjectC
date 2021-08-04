#pragma once

static void DanglingPointer()
{
	int* pi = (int*)malloc(sizeof(int));
	*pi = 5;

	printf("*pi : %d\n", *pi);
	free(pi);

	if (pi == nullptr) // 의미가 없는 코드
	{
		printf("aaa");
	}
	else
	{
		printf("bbb"); // 무조건 여기 걸린다
	}

	*pi = 10; // pi is danging pointer
	// free 함수에 의하여 pi가 가리키는 메모리는 해제되었다.
	// 하지만 ptr은 삭제되지 않아 그대로 메모리가 해제된 지역을 가리킨다.
	// 그로 인하여 pi는 댕글링 포인터가 된다.

	// 발생할 수 있는 이슈
	// 댕글링 포인터를 사용하게 되면 예측할 수 없는 문제가 터짐
}

#pragma once

static void DanglingPointer()
{
	int* pi = (int*)malloc(sizeof(int));
	*pi = 5;

	printf("*pi : %d\n", *pi);
	free(pi);

	if (pi == nullptr) // �ǹ̰� ���� �ڵ�
	{
		printf("aaa");
	}
	else
	{
		printf("bbb"); // ������ ���� �ɸ���
	}

	*pi = 10; // pi is danging pointer
	// free �Լ��� ���Ͽ� pi�� ����Ű�� �޸𸮴� �����Ǿ���.
	// ������ ptr�� �������� �ʾ� �״�� �޸𸮰� ������ ������ ����Ų��.
	// �׷� ���Ͽ� pi�� ��۸� �����Ͱ� �ȴ�.

	// �߻��� �� �ִ� �̽�
	// ��۸� �����͸� ����ϰ� �Ǹ� ������ �� ���� ������ ����
}

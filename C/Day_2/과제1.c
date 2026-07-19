#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

void type_1(int n, int t)
{
	int i, k;
	for (i = 0; i < n; i++)				//첫번째~중간줄 까지 출력
	{
		for (k = 0; k < i + 1; k++)
			printf("*");
		printf("\n");
	}
	if (t == 1)
		i = 1;
	else
		i = 0;
	for (; i < n; i++)				//중간줄 이후부터 마지막 줄까지 출력
	{
		for (k = 0; k < n - i; k++)
			printf("*");
		printf("\n");
	}


}
void type_2(int n, int t)
{
	int i, k;
	for (i = 0; i < n; i++)				//첫번째~중간줄 까지 출력
	{
		for (k = 0; k < n - i - 1; k++)
			printf(" ");
		for (k = 0; k < i + 1; k++)
			printf("*");
		printf("\n");
	}
	if (t == 1)
		i = 1;
	else
		i = 0;
	for (; i < n; i++)				//중간줄 이후부터 마지막 줄까지 출력
	{
		for (k = 0; k < i; k++)
			printf(" ");
		for (k = 0; k < n - i; k++)
			printf("*");
		printf("\n");
	}
}

void type_3(int n, int t)
{
	int i, k;
	for (i = 0; i < n; i++)				//첫번째~중간줄 까지 출력
	{
		for (k = 0; k < i; k++)
			printf(" ");
		for (k = 0; k < 2 * n - 2*i - 1; k++)
			printf("*");
		printf("\n");
	}
	if (t == 1)							//짝수면 중간줄을 한번 더 출력하기 위한 코드
		i = 1;
	else
		i = 0;
	for (i = 1; i < n; i++)				//중간줄 이후부터 마지막 줄까지 출력
	{
		for (k = 0; k < n - i - 1; k++)
			printf(" ");
		for (k = 0; k < 2*i+1; k++)
			printf("*");
		printf("\n");
	}
}

void type_4(int n, int t)
{
	int i, k;
	for (i = 0; i < n; i++)				//첫번째~중간줄 까지 출력
	{
		for (k = 0; k < i; k++)
			printf(" ");
		for (k = 0; k < n - i; k++)
			printf("*");
		printf("\n");
	}
	if (t == 1)
		i = 1;
	else
		i = 0;
	for (; i < n; i++)				//중간줄 이후부터 마지막 줄까지 출력
	{
		for (k = 0; k < n - 1; k++)
			printf(" ");
		for (k = 0; k < i + 1; k++)
			printf("*");
		printf("\n");
	}
}

int main()
{
	int size, type, t;
	printf("사이즈와 종류를 입력하시오.");
	scanf("%d %d", &size, &type);
	if (size % 2 == 0)				//짝수를 입력했을 때 중간 줄을 하나 더 출력하는 것으로 대칭을 유지하고 길이도 충족시키기 위한 코드
	{								//짝수 관련해서 질문을 드리기 전에 이미 이런 상황을 만들어 놓아서 어떻게 할지 여쭈어보니 우선은 만들었으면 그대로 제출하라고 하셔서 추가로 수정하지는 않았습니다.
		size = size / 2;
		t = 0;
	}
	else
	{
		size = (size + 1) / 2;
		t = 1;
	}
	switch (type)					//종류에 따라 각 함수로 이동
	{
	case 1:
		type_1(size, t);
		break;
	case 2:
		type_2(size, t);
		break;
	case 3:
		type_3(size, t);
		break;
	case 4:
		type_4(size, t);
		break;
	default:
		printf("종류는 1~4 사이의 정수를 입려하시오.");
		break;
	}
	return 0;
}
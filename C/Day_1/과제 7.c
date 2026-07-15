#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
int main()
{
	int n, r;
	float result;
	printf("---순열, 중복순열, 조합, 중복조합을 구하는 코드---\n\nn과 r을 입력하시오. ");
	scanf("%d %d", &n, &r);

	if (n < r || r<0)			//순열, 조합의 조건에 맞지 않을 시 아래 글 출력
	{
		printf("r은 음의 정수가 아니고, n보다 작거나 같아야 합니다.");
		return 0;
	}

	//순열
	result = 1;			//현재 상황에서는 값을 저장할 필요가 없기에 result 변수를 하나를 출력하고 초기화 하는 방식으로 코드를 제작함
	for (int i = 1; i <= n; i++)
		result *= i;
	for (int i = 1; i <= n - r; i++)
		result /= i;
	printf("순열	:	 %.2f\n", result);

	//중복순열
	result = 1;
	for (int i = 0; i < r; i++)
		result *= n;
	printf("중복순열:	 %.2f\n", result);

	//조합
	result = 1;
	for (int i = 1; i <= n; i++)
		result *= i;
	for (int i = 1; i <= n - r; i++)
		result /= i;
	for (int i = 1; i <= r; i++)
		result /= i;
	printf("조합	:	 %.2f\n", result);

	//중복조합
	result = 1;
	for (int i = 1; i <= n+r-1; i++)
		result *= i;
	for (int i = 1; i <= n - 1; i++)
		result /= i;
	for (int i = 1; i <= r; i++)
		result /= i;
	printf("중복조합:	 %.2f\n", result);
	return 0;
}
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
int main()
{
	int n;
	printf("값을 입력하세요.");
	scanf("%d", &n);
	if (n <= 0)
	{
		printf("양의 정수를 입력하세요.");
		return 0;
	}
	for (int i = n; i >0; i--)			//첫번째 줄부터 가장 *이 많이 들어가는 중간 줄까지 출력
	{
		for (int t = 0; t<n-i+1; t++)
			printf("*");
		for (int k = i-1; k > 0; k--)
			printf("  ");
		for (int t = 0; t<n-i+1; t++)
			printf("*");
		printf("\n");
	}
	for (int i = 1; i < n; i++)			//중간 줄 이후부터 마지막 줄까지 중간 줄을 제외하고 반대로 출력
	{
		for (int t = n - i; t > 0; t--)
			printf("*");
		for (int k =i; k > 0; k--)
			printf("  ");
		for (int t = n - i; t > 0; t--)
			printf("*");
		printf("\n");
	}
	return 0;
}
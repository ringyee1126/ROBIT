#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
int main()
{
	int n;
	int f = 0;
	int F = 1;
	int temp;
	printf("몇번째 피보나치 수를 출력할까요? ");
	scanf("%d", &n);
	if (n < 1)		//n을 음수를 입력하거나 0을 입력하면 제대로 된 값이 출력되지 않는 것을 방지
	{
		printf("ERROR : 양의 정수를 입력해 주시기 바랍니다.");
		return 0;
	}
	for(int i = 2; i<n; i++)		//1번째, 2번째는 else를 통해 출력하고 n이 그 이상이라면 아래를 통해 계산하여 F를 출력한다.
	{
		temp = F;
		F = F + f;
		f = temp;
	}
	if(n>=2)
		printf("%d번째 피보나치 수 : %d\n", n, F);
	else
		printf("%d번째 피보나치 수 : %d\n", n, n-1);
	return 0;
}
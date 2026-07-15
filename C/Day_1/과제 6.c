#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
int main()
{
	int n;
	printf("값을 입력하세요. ");
	scanf("%d", &n);
	if (n <= 0)
	{
		printf("양의 정수를 입력하세요.");
		return 0;
	}
	for (int i = n; i > 1; i--)
	{
		for (int k = i-1; k > 0; k--)		//첫*이 나오기 전 앞에 공백 몇번 필요한지 계산하여 추가
		{
			printf(" ");
		}
		printf("*");						//왼쪽 끝에 있는 *출력
		for (int k = 0; k < 2*n-2*i-1; k++) //왼쪽 끝과 오른쪽 끝 * 사이에 있는 공백을 계산하여 출력
		{
			printf(" ");
		}
		if (i < n)							//첫번째 줄에는 *이 하나만 있으므로 두번째 줄부터 오른쪽 끝 *을 출력
			printf("*");
		printf("\n");
	}
	for (int i = 0; i < 2*n-1; i++)			//맨 마지막 줄은 공백없이 *만 출력
	{
		printf("*");
	}
	return 0;
}
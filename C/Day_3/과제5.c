#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
int main()
{
	int n1, n2, n = 1;
	printf("행과 열의 크기를 입력하시오. : ");
	scanf("%d %d", &n1, &n2);
	if (n1 <= 0 || n2 <= 0)
	{
		printf("ERROR : 양의 정수를 입력해주시기 바랍니다.");
		return 0;
	}
	int** p = (int**)malloc(sizeof(int*) * n1);			//malloc으로 다차원 배열의 행과 열 크기를 입력받아서 동적할당 완료
	if (p == NULL)
		printf("malloc error");
	for(int i=0; i<n1; i++)
		p[i] = (int*)malloc(sizeof(int) * n2);

	for (int num = 0; num <= n1 + n2 - 2; num++)		//수학적인 방법으로 접근해서 행 + 열의 좌표를 더한 값이 일정하다는 것을 이용함 (예: (0,2)가 0+2, (1,1)이 1+1, (2,0)이 2+1, 이렇게 일정한 값을 유지함)
	{
		for (int i = 0; i < n1; i++)
		{
			int k = num - i;		//i+k가 num로 일정해야하니 k = num - i이다

			if (k >= 0 && k < n2)
			{
				p[i][k] = n;
				n++;
			}
		}
	}

	//출력
	for (int i = 0; i < n1; i++)
	{
		for (int k = 0; k < n2; k++)
		{
			printf("%4d", p[i][k]);
		}
		printf("\n");
	}

	for (int i = 0; i < n1; i++)
		free(p[i]);
	free(p);

	return 0;
}
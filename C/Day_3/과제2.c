#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
int main()
{
	int i, k, n = 1;
	int n1, n2;
	printf("행과 열의 크기를 입력하시오. : ");
	scanf("%d %d", &n1, &n2);
  if (n1 <= 0 || n2 <= 0)
	{
		printf("ERROR : 양의 정수를 입력해주시기 바랍니다.");
		return 0;
	}
	int** p1 = (int**)malloc(sizeof(int*) * n1);			//malloc으로 다차원 배열의 행과 열 크기를 입력받아서 동적할당 완료
	if (p1 == NULL)
		printf("malloc error");
	for (i = 0; i < n1; i++)
		p1[i] = (int*)malloc(sizeof(int) * n2);

	int** p2 = (int**)malloc(sizeof(int*) * n2);			//malloc으로 행과 열의 크기가 반대인 배열 동적할당 완료
	for (i = 0; i < n2; i++)
		p2[i] = (int*)malloc(sizeof(int) * n1);
	if (p2 == NULL)
		printf("malloc error");

	//숫자 채우기
	for (i = 0; i < n1; i++)
	{
		for (k = 0; k < n2; k++)
		{
			p1[i][k] = n;
			n++;
		}
	}

	//숫자 바꾸기
	for (i = 0; i < n1; i++)
	{
		for (k = 0; k < n2; k++)
			p2[k][i] = p1[i][k];
	}

	//출력
	for (i = 0; i < n2; i++)
	{
		for (k = 0; k < n1; k++)
			printf("%4d", p2[i][k]);
		printf("\n");
	}

	for (i = 0; i < n1; i++)
		free(p1[i]);
	free(p1);
	for (i = 0; i < n2; i++)
		free(p2[i]);
	free(p2);

	return 0;
}
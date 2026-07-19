#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main()
{
	int num[8];
	int t;
	int max;
	int temp;
	printf(" 입력\n");
	for (int i = 0; i < 8; i++)		//정수 8개를 입력받기
	{
		printf("\n%d : ", i);
		scanf("%d", &num[i]);
	}			

	printf("\n");
	for (int i = 0; i < 8; i++)
		printf("%4d", num[i]);
	printf("\n");

	while (1)						//sorting 이 되는 중간 과정 출력
	{
		for (int i = 7; i >= 0; i--)
		{
			max = i;
			for (int k = i-1; k >= 0 ; k--)
			{
				if (num[max] < num[k])		//배열 속에서 가장 큰 수를 기억
				{
					max = k;
				}
			}
			if (max != i)					//위 과정을 첫번째 숫자까지 반복해서 그 결과 나온 가장 큰 수와 바꾸는 코드
			{
				temp = num[max];
				num[max] = num[i];
				num[i] = temp;
			}
			else if (max == i)				//예시 사진 속 처럼 8번 반복하기 전에 결과가 나오면 즉시 반복문 탈출
				break;
			printf("\n");
			for (int i = 0; i < 8; i++)
				printf("%4d", num[i]);
			printf("\n");
			continue;
		}
		break;
	}
	printf("\n 출력\n");
	for (int i = 0; i < 8; i++)		//정렬된 정수 8개를 출력하기
		printf("\n  %d : %3d\n", i, num[i]);
	return 0;
}
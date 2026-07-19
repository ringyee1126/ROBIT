#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main()
{
	int num = 0;
	int n, b, result;
	printf("N : ");
	scanf("%d", &n);
	printf("B : ");
	scanf("%d", &b);
	printf("\n");
	int A[100];				//입력받는 숫자들은 넉넉하게 잡아둠
	for (int i = 0; i < n; i++)
		scanf("%d", &A[i]);

	while (1)				//입력받은 숫자들을 크기별로 배열해주는 코드
	{
		for (int i = n - 1; i >= 0; i--)
		{
			for (int k = i - 1; k >= 0; k--)
			{
				if (A[i] < A[k])
				{
					int temp = A[k];
					A[k] = A[i];
					A[i] = temp;
				}
			}
			continue;
		}
		break;
	}

	for (int i = 0; i < n; i++)		//중간값의 위치를 찾아주는 코드
	{
		if (b != A[i])
			num += 1;
		else
			break;
	}

	if (num <= (n + 1) / 2)			//중간값의 위치를 기준으로 부분수열의 계수를 찾아주는 코드
		result = num + 1;
	else
		result = n - num;

	printf("\n중앙값이 %d인 부분수열의 개수 : %d", b, result);
	return 0;
}
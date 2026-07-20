#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
int main()
{
	int num = 5;
	int n = 1;
	int i,k;
	int nums[5][5] = { 0, };		//지금은 5x5 행렬이지만 유동적으로 행렬의 크기를 변경할 수 있도록 코드를 작성함 (num과 배열의 범위만 조정하면 모든 홀수에서 같은 규칙을 성립함)
	for (i = 0; i < num; i++)
	{
		for (k = 0; k < num; k++)
		{
			if (i <= num / 2)		//중간보다 윗줄인지 아랫줄인지 확인하는 코드
			{
				if (k < i || k > num - 1 - i)
					continue;		//처음에 배열을 모두 0으로 채웠으므로, 조건에 맞지 않으면 그냥 continue를 하여 0을 유지함
			}
			else if (i > num / 2)
			{
				if (k > i || k < num - 1 - i)
					continue;
			}
			nums[i][k] = n;
			n++;
		}
	}
	for (i = 0; i < num; i++)
	{
		for (k = 0; k < num; k++)
		{
			printf("%3d", nums[i][k]);
		}
		printf("\n");
	}
  return 0;
}
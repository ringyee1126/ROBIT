#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

int main()
{
	int n, min, max, all = 0;
	printf("몇 개의 원소를 할당하겠습니까? : ");
	scanf("%d", &n);
	if (n <= 0)
	{
		printf("ERROR : 양의 정수를 입력해주시기 바랍니다.");
		return 0;
	}
	int* p = (int*)malloc(sizeof(int) * n);
	if (p == NULL)
		printf("malloc error");
	for (int i = 0; i < n; i++)
	{
		printf("정수형 데이터 입력:");
		scanf("%d", &p[i]);

		//최대값
		if (i == 0)			//처음 한번 max의 값을 지정해준다 (max=0을 처음에 했다가 모든 수가 음수가 나오면 최대값이 0이 되어버리는 문제를 막기 위해
			max = p[i];
		else if (i > 0)
		{
			if (max < p[i])
				max = p[i];
		}

		//최소값
		if (i == 0)			//처음 한번 min의 값을 지정해준다 (이하 생략)
			min = p[i];
		else if (i > 0)
		{
			if (min > p[i])
				min = p[i];
		}

		//전체합
		all += p[i];
	}
	printf("최대값: %d\n", max);
	printf("최소값: %d\n", min);
	printf("전체합: %d\n", all);
	printf("평 균: %f\n", (float)all / n);

	free(p);

	return 0;
}
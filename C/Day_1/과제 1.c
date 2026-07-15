#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
int main()
{
	float a;
	float mean = 0;
	float max;
	float min;

	printf("1 번째 실수를 입력하시오. ");
	scanf("%f", &a);
	mean += a;
	max = a;
	min = a;
	//max와 min의 값을 지정하지 않으면 max와 min을 a와 비교할 수 없지만 그렇다고 min을 0으로 놓았다가 입력받은 a가 모두 0보다 커버리면 최솟값이 0이 나오기 때문에 처음 1번은 a의 값을 그대로 입려받는 구조.

	for (int i = 1; i < 5; i++)
	{
		printf("%d 번째 실수를 입력하시오. ", i + 1);
		scanf("%f", &a);
		mean += a;
		if(max<a)
			max = a;
		if(min>a)
			min = a;
	}
	printf("---- 결과 ----\n");
	printf("평균은 %.2f입니다.\n", mean / 5);		//실수형의 소수점 2번째 자리까지만 출력
	printf("최댓값은 %.2f입니다.\n", max);
	printf("최솟값은 %.2f입니다.\n", min);
    return 0;
}
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
int main()
{
	int year;
	printf("년도를 입력하세요 : ");
	scanf("%d", &year);				//입력받는 년도는 음수나 0이 아닌 양의 정수가 아니면 아래 메세지 출력
	if (year <= 0)
	{
		printf("올바른 연도를 입력하시오.");
		return 0;
	}
	if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
		printf("윤년");
	else
		printf("윤년 X");
	return 0;
}
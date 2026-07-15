#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
int main()
{
	float a, b;
	char c;
	printf("Input Arithmetic Operation\n");			//pdf파일 속 과제4의 예시 사진 속 구조를 그대로 가져왔다. 
	printf("ex)      3.4 * 8.5\n         2.9 - 5.4\n         3.9 * 8.0\n         3.9 ^ 8\n");

	printf("input : ");
	scanf("%f %c %f", &a, &c, &b);

	if (c == '+')
		printf("%.2f + %.2f = %.2f", a, b, a + b);
	else if (c == '-')
		printf("%.2f - %.2f = %.2f", a, b, a - b);
	else if (c == '*')
		printf("%.2f * %.2f = %.2f", a, b, a * b);
	else if (c == '/')
		if (b == 0)
		{
			printf("0으로 나눌 수 없다.");			//0으로는 나눌 수 없으므로 따로 코드를 작성함
			return 0;
		}
		else
			printf("%.2f / %.2f = %.2f", a, b, a / b);
	else if (c == '^')
	{
		float result = 1;
		for (int i = 0; i < b; i++)
			result *= a;
		printf("%.2f ^ %.2f = %.2f", a, b, result);			// 지수는 0 이상의 정수라고 가정한다.
	}
	else          //계산 가능한 기호를 입력하지 않으면 아래 메세지를 출력한다.
		printf("이 프로그램은 사칙연산, 거듭제곱만 가능하다.\n해당하는 기호를 입력해주시오. (+,-,*,/,^)");
	return 0;
}
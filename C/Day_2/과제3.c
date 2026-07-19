#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int select(char num[9])
{
	char temp[9] = "cdefgabC";		//ascending의 배열을 만들고 이를 함수로 전달받은 배열과 비교해서 반환 값을 내놓는다.
	for (int i = 0; i < 8; i++)
	{
		if (num[i] != temp[i])
		{
			if (num[i] != temp[7 - i])
			{
				return 3;
			}
			return 2;
		}
		return 1;
	}
}

int main()
{
	char num[9];
	printf("음 입력\n");
	scanf("%c", num);
	switch (select(num))
	{
	case 1:
		printf("\n결과 : ascending");
		break;
	case 2:
		printf("\n결과 : descending");
		break;
	default:
		printf("\n결과 : mixed");
		break;
	}
	return 0;
}
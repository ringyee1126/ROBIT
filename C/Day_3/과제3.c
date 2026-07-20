#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

int main()
{
	int n1, n2, i, n = 1;

	printf("행과 열의 크기를 입력하시오 : ");
    scanf("%d %d", &n1, &n2);
	if (n1 <= 0 || n2 <= 0)
	{
		printf("ERROR : 양의 정수를 입력해주시기 바랍니다.");
		return 0;
	}
	int** p = (int**)malloc(sizeof(int*) * n1);
	if (p == NULL)
		printf("malloc error");
	for (i = 0; i < n1; i ++ )
		p[i] = (int*)malloc(sizeof(int) * n2);

	int top = 0, left = 0;
	int bottom = n1 - 1;
	int right = n2 - 1;

	while (top <= bottom && right >= left)
	{
		//위쪽
		for (i = left; i <= right; i++)
			p[top][i] = n++;
		top++;
		//오른쪾
		for (i = top; i <= bottom; i++)
			p[i][right] = n++;
		right--;
		//아래쪽
		if (top <= bottom)			//이 조건을 추가함으로써 행렬이 3 4처럼 정사각형이 아닌 경우 기존 숫자를 덮어써버리는 문제를 막는다
		{
			for (i = right; i >= left; i--)
				p[bottom][i] = n++;
			bottom--;
		}
		//왼쪽
		if (left <= right)			//이하 생략
		{
			for (i = bottom; i >= top; i--)
				p[i][left] = n++;
			left++;
		}
	}

    for (int i = 0; i < n1; i++)
    {
        for (int j = 0; j < n2; j++)
            printf("%4d", p[i][j]);
        printf("\n");
    }

	for (int i = 0; i < n1; i++)
        free(p[i]);
    free(p);

    return 0;

}
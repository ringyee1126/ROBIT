#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

//전체적인 코드를 3일차 과제3의 코드를 보고 참고하였다.(둘 다 달팽이 배열을 만들어서 출력하는 것이라 이것에 이번 시간에 배운 포인터로 매개변수를 주는 함수를 잘 녹여내렸다.
//포인터를 사용하였기에 함수에는 추가적인 반환값을 지정해 줄 필요가 없었다.

void print(int* row, int* col, int** pArr)		//완성된 달팽이 코드의 행과 열, 전체 배열을 포인터(이중포인터 포함)으로 받아와서 출력함
{
	for (int i = 0; i < *row; i++)
	{
		for (int j = 0; j < *col; j++)
			printf("%4d", pArr[i][j]);
		printf("\n");
	}
}

void arr_ij(int* sizeRow, int* sizeCol, int** pArr)
{
	int i;
	int n = 1;
	int top = 0, left = 0;
	int bottom = *sizeRow - 1;
	int right = *sizeCol - 1;

	while (top <= bottom && right >= left)
	{
		//위쪽
		for (i = left; i <= right; i++)
			pArr[top][i] = n++;
		top++;
		//오른쪾
		for (i = top; i <= bottom; i++)
			pArr[i][right] = n++;
		right--;
		//아래쪽
		if (top <= bottom)			//이 조건(if문)을 추가함으로써 행렬이 3 4처럼 정사각형이 아닌 경우 기존 숫자를 덮어써버리는 문제를 막고 중간에 몸통이 끊기지 않는(숫자가 생략되는) 달팽이를 출력할 수 있다.
		{
			for (i = right; i >= left; i--)
				pArr[bottom][i] = n++;
			bottom--;
		}
		//왼쪽
		if (left <= right)			//이하 생략
		{
			for (i = bottom; i >= top; i--)
				pArr[i][left] = n++;
			left++;
		}
	}
}

int main()
{
	int** arr = NULL;		//후에 출력하기 위한 2차원 배열(달팽이)을 이중 포인터로 선언
	int row, col, sizeRow, sizeCol, i;

	printf("열의 수를 입력하세요:");
	if (scanf("%d", &sizeCol) != 1 || sizeCol <= 0)		//양의 정수가 아닌 0이하의 정수 또는 다른 형태가 입력되면 오류 메세지를 출력하도록 과제3의 예시 사진에 이 코드를 추가함
	{
		printf("ERROR : 열은 양의 정수로 입력해주시기 바랍니다.");
		return 1;
	}
	printf("행의 수를 입력하세요:");
	if (scanf("%d", &sizeRow) != 1 ||sizeRow <= 0)
	{
		printf("ERROR : 행은 양의 정수로 입력해주시기 바랍니다.");
		return 1;
	}

	row = sizeRow;
	col = sizeCol;

	arr = (int**)malloc(sizeof(int*) * row);
	if (arr == NULL)
	{
		printf("malloc error");
		return 1;
	}
	for (i = 0; i < row; i++)
		arr[i] = (int*)malloc(sizeof(int) * col);			//2차월 동적 메모리 할당을 사용하여 낭비되는 메모리가 최대한 없도록 한다.
		if (arr == NULL)		//만약 arr[i]은 정상적으로 동적 할당이 되었는데 이를 한번 더 동적할당 했을 경우 만약에 오류가 발생한다면
		{
			printf("malloc error");
			for (int k = 0; k < i; k++)		//이미 할당된 메모리는 꼭 메모리를 해제하고 코드 종료
				free(arr[k]);
			free(arr);
			return 1;
		}

	arr_ij(&sizeRow, &sizeCol, arr);		//달팽이를 만드는 함수

	print(&row, &col, arr);					//위 함수에서 만들어진 달팽이 배열을 출력하는 함수

	for (int i = 0; i < row; i++)
		free(arr[i]);
	free(arr);

	return 0;

}
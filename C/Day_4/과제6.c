#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define ROWS 3
#define COLS 4

void print_shape(int *values, int count)    //3*4도형을 1줄(1차원)로 나열하여 숫자를 대입하기 위한 함수
{
    int row, col;
    int k = 0;      //3*4도형 전체를 1차원으로 봤을 때의 순서(0번째 칸부터)

    for (row = 0; row < ROWS; row++)
    {
        for (col = 0; col < COLS; col++)
        {
            printf("%d", *(values + (k % count)));      //포인터 연산으로 값을 접근했다
            k++;
        }
        printf("\n");
    }
    printf("\n");
}

int main(void)
{
    int* values = NULL;     //values의 값을 처음에 NULL로 정의하여 밑에 있는 realloc에서 사용 가능하도록 함 + 또다른 문제가 발생하지 않도록 NULL로 정의함
    int count = 0;
    int num;

    printf("숫자를 입력하시오. : ");
    while (scanf("%d", &num) == 1)      //정수가 아닌 소수가 있는 실수 또는 문자같은 다른 입력을 받으면 while문 작동 x -> 코드 종료
    {
        count++;
        int *temp = (int *)realloc(values, sizeof(int) * count);
        if (temp == NULL)
        {
            printf("메모리 할당에 실패했습니다.\n");
            return 1;
        }
        values = temp;
        *(values + count - 1) = num;        //새로 입력된 숫자를 배열 맨 끝에 저장(수업 자료 속 포인터 배열관련 내용 참고)

        print_shape(values, count);
        printf("다음 숫자를 입력하시오. : ");
    }

    free(values);
    return 0;
}

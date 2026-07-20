#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
int main()
{
    int n1, n2, i, n = 1;
    printf("행과 열의 크기를 입력하시오.(행과 열은 둘 다 홀수이다.) : ");
    scanf("%d %d", &n1, &n2);
    if (n1 <= 0 || n2 <= 0)
    {
        printf("ERROR : 양의 정수를 입력해주시기 바랍니다.");
        return 0;
    }
    if (n1 % 2 == 0 || n2 % 2 == 0)
    {
        printf("ERROR : 행과 열 모두 홀수로 입력해주시기 바랍니다.");
        return 0;
    }
    int** p = (int**)malloc(sizeof(int*) * n1);            //malloc으로 다차원 배열의 행과 열 크기를 입력받아서 동적할당 완료
    if (p == NULL)
        printf("malloc error");
    for (int i = 0; i < n1; i++)
        p[i] = (int*)malloc(sizeof(int) * n2);

    int start_x = n2 / 2; //열
    int start_y = n1 / 2; //행

    //func_start
    int x = start_x, y = start_y;
    p[y][x] = n++;    //중심 : 1

    int total = n1 * n2;

    for (int r = 1; n <= total; r++)    //r을 중심으로부터의 반지름으로 이용하여 반복문 작성 + n값이 n1*n2n를 넘는 것(모든 공간에 숫자가 채워짐)을 기준으로 반복문 탈출
    {
        // func_0 : 아래로 1칸
        y++;
        if (x >= 0 && x < n2 && y >= 0 && y < n1)
            p[y][x] = n++;

        // func_1   :   우하
        for (int i = 0; i < r - 1 && n <= total; i++)
        {
            x++; y++;
            if (x >= 0 && x < n2 && y >= 0 && y < n1)
                p[y][x] = n++;
        }
        // func_2   :   우상
        for (int i = 0; i < r && n <= total; i++)
        {
            x++; y--;
            if (x >= 0 && x < n2 && y >= 0 && y < n1)
                p[y][x] = n++;
        }
        // func_3   :   좌상
        for (int i = 0; i < r && n <= total; i++)
        {
            x--; y--;
            if (x >= 0 && x < n2 && y >= 0 && y < n1)
                p[y][x] = n++;
        }
        // func_4   :   좌하
        for (int i = 0; i < r && n <= total; i++)
        {
            x--; y++;
            if (x >= 0 && x < n2 && y >= 0 && y < n1)
                p[y][x] = n++;
        }
    }

    //출력
    for (i = 0; i < n1; i++)
    {
        for (int k = 0; k < n2; k++)
        {
            printf("%4d", p[i][k]);
        }
        printf("\n");
    }

    for (i = 0; i < n1; i++)
        free(p[i]);
    free(p);

    return 0;
}
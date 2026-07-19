#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int func(int n)
{
    for (int i = 2; i <= 100; i++)      //100의 제곱이 10000이라서 100까지만 잡음
    {
        int num = i * i;

        if (num > n)
            break;

        if (n % num == 0)
            return 0;       // num(i의 제곱)으로 나누어 떨어지면 제곱 ㄴㄴ수가 아님
    }

    return 1;               // 제곱ㄴㄴ수가 맞음
}

int main()
{
    int min, max;
    int count = 0;

    printf("min : ");
    scanf("%d", &min);
    printf("max : ");
    scanf("%d", &max);

    for (int i = min; i <= max; i++)    //수 출력보다 개수를 먼저 출력해야함
    {
        if (func(i))
            count++;
    }
    printf("제곱 ㄴㄴ수 : %d\n", count);

    for (int i = min; i <= max; i++)    // 제곱 ㄴㄴ수 출력
    {
        if (func(i))
            printf("%d ", i);
    }
    return 0;
}
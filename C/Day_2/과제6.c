#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int index;
int set[20] = { 0, };

void Add(int n)     //n추가, 없으면 무시 (1<=n<=20)
{
    if (n < 1 || n > 20)
        return;
    for (int i = 0; i < 20; i++)     // 이미 있는지 확인
    {
        if (set[i] == n)
            return;
    }
    for (int i = 0; i < 20; i++)
    {
        if (set[i] == 0)
        {
            set[i] = n;
            return;
        }
    }
}

void Remove(int n)      //n제거, 없으면 무시 (1<=n<=20)
{
    if (n < 1 || n > 20)
        return;
    for (int i = 0; i < 20; i++)
    {
        if (set[i] == n)
        {
            set[i] = 0;
            return;
        }
    }
}

void Check(int n)       //set에 n이 있으면 1, 없으면 0 출력
{
    for (int i = 0; i < 20; i++)
    {
        if (set[i] == n)
        {
            if (set[i] == 0)        //set의 모든 값을 0으로 놓고 시작했고 check n의 n은 따로 범위가 주어지지 않아서 check 0을 했을 때 1을 출력하는 것을 방지하는 코드
            {
                printf("0 ");
                return;
            }
            printf("1 ");
            return;
        }
    }
    printf("0 ");
}

void Toggle(int n)      //set에 n이 있으면 지우고, 없으면 추가한다 (1<=n<=20)
{
    if (n < 1 || n > 20)
        return;
    for (int i = 0; i < 20; i++)
    {
        if (set[i] == n)
        {
            set[i] = 0;
            return;
        }
    }
    for (int i = 0; i < 20; i++)
    {
        if (set[i] == 0)
        {
            set[i] = n;
            return;
        }
    }
}

void All()              //set을 {1,2,...,20}으로 바꾼다
{
    for (int i = 0; i < 20; i++)
        set[i] = i+1;
}

void Empty()            //set을 공집합으로 바꾼다
{
    for (int i = 0; i < 20; i++)
        set[i] = 0;
}

void PrintSet()         //현재 set 출력
{
    printf("집합 : { ");
    for (int i = 0; i < 20; i++)
    {
        if (set[i] != 0)
            printf("%d, ", set[i]);
    }
    printf(" }\n");
}

int main()
{
    int n;
    printf("연산을 선택하세요. (1 <= x <= 20)\nadd X\nremove X\ncheck X\ntoggle X\nall 0\nempty 0\n");
    char list[6][7] = { "add", "remove", "check", "toggle", "all", "empty" };
    char input[7];
    while (1)
    {
        printf("\ninput : ");
        scanf("%s %d", &input, &n);

        if (input[0] == 'a')        //배우지 않은 내용(strcmp를 사용하지 않고 이를 해결하기 위해 입력받는 input이 위 명령어 중 하나에 반드시 해당한다고 가정함
        {
            if (input[1] == 'd')
                Add(n);
            else
                All();
        }
        else if (input[0] == 'r')
            Remove(n);
        else if (input[0] == 'c')
            Check(n);
        else if (input[0] == 't')
            Toggle(n);
        else if (input[0] == 'e')
            Empty();
        PrintSet();
    }
    return 0;
}
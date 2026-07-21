#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define NAME_LEN 10     //구매할 상품들의 이름이 각각 9자리 이내라고 가정

typedef struct _Item
{
    char name[NAME_LEN];
    int cost;
}Item;

//string.h의 strcmp 대신, 두 문자열이 완전히 같은지 비교(같으면 1, 다르면 0을 반환)하는 함수
int is_same_name(char *a, char *b)      //문자를 하나씩 매개변수로 받아서 오늘(4일차)에 배운 포인터 개념을 이용해봄
{
    int i;
    for (i = 0; a[i] != '\0' && b[i] != '\0'; i++)
    {
        if (a[i] != b[i])
            return 0;
    }
    //두 문자열이 같은 위치에서 동시에 끝나야 완전히 같은 문자열
    return (a[i] == '\0' && b[i] == '\0');      //참이면 1인 것을 이용
}

int main(void)
{
    int i, n;
    char answer[NAME_LEN];
    int result = 0;
    int found = 0;      //일치하는 이름을 한 번이라도 찾았는지 여부 -> 출력 메세지를 0으로 할지 가격의 총합으로 할 지 오류 없이 해결하기 위해 추가함

    printf("구매할 물품의 개수를 입력하시오. : ");
    if (scanf("%d", &n) != 1 || n <= 0)
    {
        printf("올바른 개수를 입력하세요.\n");
        return 1;
    }

    //필요한 개수(n)만큼만 동적할당하여 메모리를 최소한으로 사용
    Item *list = (Item *)malloc(sizeof(Item) * n);
    if (list == NULL)
    {
        printf("malloc error\n");
        return 1;
    }

    for (i = 0; i < n; i++)
    {
        printf("물품의 이름과 금액을 입력하시오. : ");
        if (scanf("%s %d", list[i].name, &list[i].cost) != 2)      //과제1, 2에서도 언급했지만 이 구조를 통해 이름과 금액을 입력할 때 각각 문자열과 정수인지를 확인 가능하다.
        {
            printf("올바른 값을 입력하세요.\n");
            free(list);
            return 1;
        }
    }

    printf("확인할 물품의 이름을 입력하시오. : ");
    scanf("%s", answer);

    //이름이 일치하는 모든 물품의 금액을 합산
    for (i = 0; i < n; i++)
    {
        if (is_same_name(list[i].name, answer))
        {
            result += list[i].cost;
            found = 1;
        }
    }

    if (found)      //found = 1이면, if(1)의 1은 True를 의미함
        printf("%d\n", result);
    else
        printf("0\n");

    free(list);
    return 0;
}

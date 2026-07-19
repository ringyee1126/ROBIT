#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main()
{
    char inputStr[100];
    char subStr[100];
    int result[100];
    int count = 0;

    printf("최대 99개의 문자 입력 <inputStr> : ");
    scanf("%99s", &inputStr);

    printf("찾는 문자열 <subStr> : ");
    scanf("%99s", &subStr);

    // inputStr의 길이
    int lenInput = 0;
    while (inputStr[lenInput] != '\0')      //strlen은 아직 배우지 않아서 이를 대신하는 코드, 널 문자가 나오기 전까지 계속 증가시킨다
        lenInput++;

    // subStr의 길이
    int lenSub = 0;
    while (subStr[lenSub] != '\0')
        lenSub++;

    // 부분 문자열 찾기
    for (int i = 0; i <= lenInput - lenSub; i++)    //subStr이 들어갈 수 있는 마지막 시작 위치까지만 검사한다. ( 그 이후의 위치에서는 subStr 전체를 비교할 수 없다 )
    {
        int same = 1;

        for (int k = 0; k < lenSub; k++)
        {
            if (inputStr[i + k] != subStr[k])       //찾는 문자열이 2자리 이상일 때 처음 문자만 맞고 그 다음 문자가 다르면 다른 문자열로 취급함
            {
                same = 0;
                break;
            }
        }

        if (same)
        {
            result[count] = i;
            count++;
        }
    }
    printf("%s의 위치 : ", subStr);

    if (count == 0)         //만약 찾는 문자열이 없으면 메세지 출력
    {
        printf("없음");
    }
    else
    {
        for (int i = 0; i < count; i++)
        {
            printf("%d ", result[i] + 1);
        }
    }

    return 0;
}
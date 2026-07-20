#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

int main()
{
    char input[1000];
    printf("HTML 태그 구조를 입력하시오. : ");
    scanf("%s", input);

    //input문자열 길이 구하기 (널문자 포함해서 길이 측정)
    int len = 0, i = 0;
    char temp;
    do                          //1일차에 배운 do while을 사용하여 temp를 선언하며 동시에 초기화할 필요가 없이 바로 input[i]대입한 후 while문의 조건에서 temp를 사용할 수 있게 만들었다.
    {
        temp = input[i];
        len++;
        i++;
    } while (temp != '\0');     //string.h를 현재 배우지 않은 상태이므로 문자열을 구하는 방법을 문자열의 끝부분이 널문자라는 것을 이용하여 구하는 코드
    int strLen = len - 1;

    //동적할당으로 스택 생성 (태그 이름을 저장할 포인터 배열, 최대 크기 strLen으로 넉넉히 확보)
    char** stack = (char**)malloc(sizeof(char*) * strLen);
    int top = -1;
    int error = 0;

    //태그 구조가 올바른지 확인하는 코드
    i = 0;
    while (i < strLen && !error)
    {
        if (input[i] == '<')
        {
            int start = i, t = i;
            while (t < strLen && input[t] != '>')
                t++;
            if (t >= strLen)
                error = 1; break;             //>으로 닫히지 않음 (짝 없음)

            int isClose = (input[start + 1] == '/');
            int nameStart = isClose ? start + 2 : start + 1;
            int nameLen = t - nameStart;
            if (nameLen <= 0)
                error = 1; break;             //태그 이름이 비어있음 (<와>사이에 아무것도 없으면 오류 메세지 출력)

            if (isClose) {
                if (top < 0)
                    error = 1; break;         //짝없는 닫는 태그

                int k, sLen = 0;
                while (stack[top][sLen] != '\0')
                    sLen++;

                int match = (sLen == nameLen);
                if (match)
                {
                    for (k = 0; k < nameLen; k++)
                    {
                        if (stack[top][k] != input[nameStart + k])
                            match = 0; break;
                    }
                }
                if (!match) { error = 1; break; }               //여는 태그와 이름 불일치

                free(stack[top]);
                top--;
            }
            else
            {
                char* name = (char*)malloc(sizeof(char) * (nameLen + 1));
                int k;
                for (k = 0; k < nameLen; k++)
                    name[k] = input[nameStart + k];
                name[nameLen] = '\0';
                stack[++top] = name;
            }
            i = t + 1;
        }
        else
            i++;
    }
    if (!error && top != -1)
        error = 1;        //끝까지 안 닫힌 태그가 남음

    while (top >= 0)     //스택 정리
    {
        free(stack[top]);
        top--;
    }
    if (error)        //error가 True이면 1이므로 if조건문이 작동시킴으로써 여러 상황에서 if문을 통해 각각 오류임을 판단하여 메세지를 출력할 필요 없이 오류라면 이를 기억했다가 여기서 한번에 출력하는 방법으로 효율성을 챙김
    {
        printf("오류: 잘못된 태그 구조입니다.\n");
        free(stack);
        return 0;
    }

    int depth = 0;
    i = 0;
    while (i < strLen)
    {
        if (input[i] == '<')
        {
            int start = i, t = i;
            while (input[t] != '>')
                t++;
            int isClose = (input[start + 1] == '/');        
            if (isClose)        //isClose가 True이면 1이므로 if조건문이 작동
                depth--;
            int k;
            for (k = 0; k < depth; k++)
                printf("  ");            //태그(<...>)가 아닌 문자열은 다음 행의 문자열이 같은 행에 있고 그렇지 않으면 지금처럼 빈공간을 두어서 구분해줌
            for (k = start; k <= t; k++)
                printf("%c", input[k]);
            printf("\n");
            if (!isClose) depth++;
            i = t + 1;
        }
        else {
            int start = i;
            while (i < strLen && input[i] != '<')
                i++;
            int k;
            for (k = 0; k < depth; k++)
                printf("  ");            //이하 생략
            for (k = start; k < i; k++)
                printf("%c", input[k]);
            printf("\n");
        }
    }
    free(stack);
    return 0;
}
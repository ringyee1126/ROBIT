#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#define MAX_LEN 200

int main(void)
{
    char sentence[MAX_LEN] = { '\0' };      //선언과 동시에 초기화
    int count[256] = { 0 };                 //확장 아스키코드가 총 256개라서 배열의 크기를 256으로 설정
    char* end;
    int len, i;
    int max_count = 0;
    int distinct_total = 0;     //공백을 제외하고 문장에 등장한 서로 다른 문자의 개수
    int distinct_max = 0;       //그 중 최댓값(max_count)만큼 등장한 문자의 개수

    printf("문장을 입력하시오 : ");
    //%[^\n]를 사용하여 Enter로 줄넘김이 있기 전까지 공백 포함 모든 문자를 읽을 수 있음 (fgets, gets 미사용)
    if (scanf(" %[^\n]", sentence) != 1)
    {
        printf("문장이 입력되지 않았습니다.\n");
        return 1;
    }

    //문자열의 끝(널문자) 위치를 포인터로 탐색 -> 길이 계산
    end = sentence;
    while (*end != '\0')
    {
        end++;
    }
    len = (int)(end - sentence);

    //맨 뒤 인덱스부터 거꾸로 한 글자씩 출력
    for (i = len - 1; i >= 0; i--)
    {
        printf("%c", *(sentence + i));       //수업 때 배운 포인터 연산으로 문자 접근
    }
    printf('\n');

    //공백을 제외한 모든 문자(알파벳, 특수문자 등)의 등장 횟수 카운트
    //공백은 단어를 구분하는 역할일 뿐 의미있는 "등장 문자"로 보기 어려워서 제외시킴
    for (i = 0; i < len; i++)
    {
        char c = *(sentence + i);
        if (c != ' ')
        {
            count[(unsigned char)c]++;      //예상치 못하게 인덱스가 음수가 나오는 것을 방지를 위해 unsigned char 사용
        }
    }

    //최댓값(max_count) 탐색
    for (i = 0; i < 256; i++)
    {
        if (count[i] > max_count)
            max_count = count[i];
    }

    //서로 다른 문자가 총 몇 종류인지(distinct_total), 그 중 최댓값만큼 등장한 문자가 몇 종류인지(distinct_max) 계산하는 코드
    for (i = 0; i < 256; i++)
    {
        if (count[i] > 0)
        {
            distinct_total++;
            if (count[i] == max_count)
                distinct_max++;
        }
    }

    if (distinct_max == distinct_total)
        printf("모든 문자가 %d번 나왔습니다.\n", max_count);        //등장한 모든 문자의 횟수가 전부 동일한 경우 다음과 같은 메세지 출력
    else
    {
        //최다 등장 문자가 하나 또는 여러 개인 경우, 해당하는 문자중 하나만 출력하지 않고 전부 출력
        printf("최다등장문자 : ");
        for (i = 0; i < 256; i++)
        {
            if (count[i] == max_count)
                printf("%c ", (char)i);     //예기치 못한 오류가 나오는 것을 방지하기 위해 아스키 코드를 문자형(char)로 강제 변환하여 출력
        }
        printf("\n");
    }

    return 0;
}
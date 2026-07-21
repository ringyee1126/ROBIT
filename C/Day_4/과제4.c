#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

typedef struct _Date
{
    int year;
    int month;
    int day;
} DATE;

typedef struct _Time
{
    int hour;
    int min;
    int sec;
} TIME;

typedef struct _Timestamp
{
    DATE date;
    TIME time;
} TIMESTAMP;

//윤년 판별 (4의 배수이면서 100의 배수가 아니거나, 400의 배수인 경우) <- 예전 과제의 코드를 이용했다.
int is_leap(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

//해당 연도, 월의 마지막 날짜(며칠까지 있는지) 반환
int days_in_month(int year, int month)
{
    int days[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };      //달마다의 일 수를 따로 입력
    if (month == 2 && is_leap(year))        //윤년은 2월의 일 수가 다르기 때문에 따로 구해놔야 한다.(2월달이고 윤년일 경우)
        return 29;
    return days[month - 1];
}

//총 초를 계산하는 코드
long long total_seconds(TIMESTAMP t)        //크기가 매우 커질 수 있기에 long long을 사용
{
    long long total_days = 0;
    int y, m;

    for (y = 1; y < t.date.year; y++)
    {
        if (is_leap(y))
            total_days += 366;          //윤년이면 2월이 하루 더 많아서 366일
        else
            total_days += 365;
    }
    for (m = 1; m < t.date.month; m++)
    {
        total_days += days_in_month(t.date.year, m);
    }
    total_days += (t.date.day - 1);

    return total_days * 86400LL + (long long)t.time.hour * 3600 + (long long)t.time.min * 60 + t.time.sec;     //1일은 24시간, 1440분, 86400초임을 + 1시간은 60분, 3600초임을 + 1분은 60초임을 통해 초를 구함
}

int main(void)
{
    TIMESTAMP t1, t2;
    long long diff, hours, minutes, seconds;        //이하 생략

    printf("첫 번째 날짜와 시각을 입력하시오 (년 월 일 시 분 초) : ");
    if (scanf("%d %d %d %d %d %d", &t1.date.year, &t1.date.month, &t1.date.day, &t1.time.hour, &t1.time.min, &t1.time.sec) != 6)        //입력이 잘못 되었을 때 오류 메세지 출력, 이하 생략
    {
        printf("올바른 값을 입력하세요.\n");
        return 1;
    }

    printf("두 번째 날짜와 시각을 입력하시오 (년 월 일 시 분 초) : ");
    if (scanf("%d %d %d %d %d %d", &t2.date.year, &t2.date.month, &t2.date.day, &t2.time.hour, &t2.time.min, &t2.time.sec) != 6)        //입력이 잘못 되었을 때 오류 메세지 출력, 이하 생략
    {
        printf("올바른 값을 입력하세요.\n");
        return 1;
    }

    diff = total_seconds(t2) - total_seconds(t1);
    if (diff < 0)
    {
        diff = -diff; //순서와 상관없이 항상 양수 출력(첫번째가 더 클 때와 두번째가 더 클 때 부호의 차이를 없애기 위한 코드)
    }

    hours = diff / 3600;
    minutes = (diff % 3600) / 60;
    seconds = diff % 60;

    printf("%lld시 %lld분 %lld초\n", hours, minutes, seconds);

    return 0;
}

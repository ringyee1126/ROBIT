#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>       //실습에서 사용한 math.h를 사용하여 제곱근을 구할 것이다.

typedef struct _Point
{
    int x;
    int y;
}Point;

//두 좌표 사이의 거리를 계산하기 위한 함수
double get_distance(Point p1, Point p2)     //거리는 좌표의 차이의 제곱, 제곱근과 관련이 있어서 크기가 커질 수 있으므로, 실습에서 사용된 것을 참고하여 double을 사용했다.
{
    double xdiff = (double)(p1.x - p2.x);
    double ydiff = (double)(p1.y - p2.y);
    return sqrt(xdiff * xdiff + ydiff * ydiff);     //math.h의 sqrt를 사용하여 제곱근을 구했다.
}

int main()
{
    int n, i, j;

    printf("입력을 받을 좌표 수 : ");
    if (scanf("%d", &n) != 1 || n <= 0)     //입력받을 좌표 수가 0이하 이거나 정수가 아닌 등 잘못된 상황에 오류 메세지 출력
    {
        printf("올바른 개수를 입력하세요.\n");
        return 1;
    }

    //동적할당으로 좌표 배열 생성
    Point *list = (Point *)malloc(sizeof(Point) * n);
    if (list == NULL)
    {
        printf("메모리 할당에 실패했습니다.\n");
        return 1;
    }

    for (i = 0; i < n; i++)
    {
        if (scanf("%d %d", &list[i].x, &list[i].y) != 2)        //정수만 입력 받도록 한다.(아닐시 아래 오류 메세지 출력 + 코드 종료)
        {
            printf("올바른 좌표를 입력하세요.\n");
            free(list);     //오류 메세지가 출력되었을 때, 혹시 모를 문제를 방지하고자 코드를 종료하기 전 메모리 해제를 빼먹지 않고 한다.
            return 1;
        }
    }

    int max_index = 0;
    double max_sum = 0.0;       //이하 생략

    //각 좌표마다 나머지 좌표들과의 거리 총합을 구해서 최댓값 탐색(위에서 선언한 함수 사용)
    for (i = 0; i < n; i++)
    {
        double sum = 0.0;
        for (j = 0; j < n; j++)
        {
            if (i != j)
            {
                sum += get_distance(list[i], list[j]);
            }
        }

        if (i == 0 || sum > max_sum)
        {
            max_sum = sum;
            max_index = i;
        }
    }

    //과제2의 예시 입출력 값을 보고 특히 거리 총합을 통해서 모든 좌표들과의 거리의 합이 가장 큰 수를 출력하는 것이라고 판단함
    printf("가장 거리가 먼 좌표는 (%d, %d)이며, 다른 좌표의 거리 총합은 약 %.1f입니다.\n", list[max_index].x, list[max_index].y, max_sum);

    free(list);
    return 0;
}

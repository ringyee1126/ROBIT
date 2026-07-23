#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

typedef struct _Node
{
    int data;               //데이터를 저장하는 부분 (1, 2, 3)
    struct _Node* next;     //다음 node의 주소를 저장하는 포인터
} Node;


//Enqueue : Queue에 data 입력
void Enqueue(Node** front, Node** rear, int data)       //이중 포인터 사용
{
    Node* newNode;

    newNode = (Node*)malloc(sizeof(Node));      //Node 구조체 하나가 들어갈 만큼의 메모리를 동적으로 할당하는 과정에서 malloc을 사용

    newNode->data = data;       //=(*newNode).data   ->를 사용하여 newNode가 가리키는 구조체의 멤버에 접근
    newNode->next = NULL;       //새로운 Node는 현재 Queue의 가장 마지막에 들어가므로 다음 Node가 존재하지 않아 NULL을 저장

    if (*front == NULL)
    {
        *front = newNode;
        *rear = newNode;        //Queue에 Node가 하나도 없는 경우는 새로 만든 Node가 처음(front)이자 마지막(reaer)이다
    }
    else
    {
        (*rear)->next = newNode;    //Queue에 이미 Node가 존재하는 경우는 현재 가장 뒤에 있는 Node의 next에 새로운 Node의 주소를 저장하여 연결한다.

        *rear = newNode;            //새로운 Node가 Queue의 가장 뒤에 위치하게 되었으므로 rear를 새로운 Node로 변경
    }
}


//Dequeue : Dequeue(Queue의 가장 앞에 있는 값을 제거)하고 그 값 출력. Queue가 비어있을 시 비어있다고 출력
void Dequeue(Node** front, Node** rear)
{
    Node* deleteNode;

    if (*front == NULL)
    {
        printf("Queue가 비어있습니다.\n");
        return;
    }

    //현재 Queue의 가장 앞에 있는 Node를 삭제할 Node로 지정
    deleteNode = *front;

    printf("Dequeue된 값 : %d\n", deleteNode->data);      //=(*deleteNode).data   ->를 사용하여 deleteNode가 가리키는 구조체의 data에 접근

    /*
    현재 front가 삭제할 Node의 다음 Node를 가리키도록 변경한다.
    이렇게 하여 기존의 가장 앞에 있던 Node를 Queue에서 제외한다.
    */
    *front = deleteNode->next;
    free(deleteNode);       //제거된 노드의 메모리는 해제

    if (*front == NULL)
    {
        *rear = NULL;       //마지막 Node까지 삭제되어 Queue가 비게 된다면 front뿐만 아니라 rear도 NULL로 변경해야 한다. (이유는 위와 같음)
    }
}


//size : Queue의 크기 출력
int size(Node* front)
{
    int count = 0;

    while (front != NULL)
    {
        count++;
        front = front->next;       //현재 Node의 next에 저장된 다음 Node의 주소를 front에 대입
    }

    return count;
}


//front : Queue의 가장 앞에 있는 값(front에 위치한 값) 반환
int front(Node* front)
{
    if (front == NULL)
    {
        printf("Queue가 비어있습니다.\n");
        return -1;
    }

    return front->data;
}


//rear : Queue의 가장 뒤에 있는 값(rear에 위치한 값) 반환
int rear(Node* rear)
{
    if (rear == NULL)
    {
        printf("Queue가 비어있습니다.\n");
        return -1;
    }

    return rear->data;
}


//isEmpty : Queue가 비어있는지 확인
int isEmpty(Node* front)
{
    if (front == NULL)
        return 1;       //front가 NULL이면 Queue에 Node가 하나도 없다는 것이므로 true(=1)
    return 0;           //front가 NULL이 아니라는 것은 Queue에 Node가 존재한다는 것이므로 false(=0)
}


//printQueue : Queue 내 모든 값 출력
//Queue가 비어있으면 비어있다고 출력
void printQueue(Node* front)
{
    if (front == NULL)
    {
        printf("Queue가 비어있습니다.\n");
        return;
    }

    while (front != NULL)
    {
        printf("%d ", front->data);
        front = front->next;        //이하 생략
    }

    printf("\n");
}


int main()
{
    Node* frontNode = NULL;
    Node* rearNode = NULL;

    /*
    frontNode와 rearNode를 선언함과 동시에 NULL로 초기화(= 현재 Queue에 Node가 하나도 없다(는 것을 의미한다)
    Queue에서는 가장 앞에 있는 Node를 front가, 가장 뒤에 있는 Node를 rear가 가리키도록 한다.
    */
    Enqueue(&frontNode, &rearNode, 1);
    Enqueue(&frontNode, &rearNode, 2);
    Enqueue(&frontNode, &rearNode, 3);       //임시 data값으로 1, 2, 3을 넣었다.
    //위 3개를 다 주석 처리했을 때 isEmpty는 1을 front와 reaer은 -1을 출력하고 중간중간 Dequeue가 작동하면서 비어있다는 메세지까지 출력되는 것을 확인했다.

    printf("Queue 내 모든 값 출력 : ");
    printQueue(frontNode);
    printf("Queue 크기 : %d\n", size(frontNode));
    printf("front의 값 : %d\n", front(frontNode));
    printf("rear의 값 : %d\n", rear(rearNode));
    printf("isEmpty : %d\n", isEmpty(frontNode));
    Dequeue(&frontNode, &rearNode);
    printf("Dequeue 이후 Queue : ");
    printQueue(frontNode);

    return 0;
}
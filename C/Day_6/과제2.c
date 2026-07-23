#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

typedef struct _Node
{
    int data;               //데이터를 저장하는 부분 (1, 2, 3)
    struct _Node* next;     //다음 node의 주소를 저장하는 포인터
} Node;


//push : 정수 push
void push(Node** top, int data)     //이중 포인터 사용
{
    Node* newNode;

    newNode = (Node*)malloc(sizeof(Node));      //Node 구조체 하나가 들어갈 만큼의 메모리를 동적으로 할당하는 과정에서 malloc을 사용

    newNode->data = data;       //=(*newNode).data   ->를 사용하여 newNode가 가리키는 구조체의 멤버에 접근
    newNode->next = *top;       //=(*newNode).next   현재 top이 가리키는 노드의 주소를 저장

    *top = newNode;     //새로운 노드(newNode)의 주소를 *top에 저장하여 Stack의 가장 위를 가리키는 top이 새로운 노드를 가리키게 됨
}


//pop : pop하고 pop된 값 출력. stack이 비어있을 시 비어있다고 출력
void pop(Node** top)
{
    Node* deleteNode;
    if (*top == NULL)
    {
        printf("Stack이 비어있습니다.\n");
        return;
    }
    deleteNode = *top;
    printf("pop된 값 : %d\n", deleteNode->data);      //=(*deleteNode).data   deleteNode는 삭제할 노드를 가리키는 포인터이므로, ->를 사용하여 해당 노드의 data에 접근
    *top = deleteNode->next;        //이하 생략
    free(deleteNode);           //제거된 노드의 메모리는 해제
}


//size : stack 크기 출력
int size(Node* top)
{
    int count = 0;
    while (top != NULL)
    {
        count++;
        top = top->next;        //top이 현재 가리키고 있는 노드의 next에 저장된 다음 노드의 주소를 top에 대입
    }
    return count;
}


//top : top에 위치한(stack의 맨 윗부분에 있는) 값 반환
int top(Node* top)
{
    if (top == NULL)
    {
        printf("Stack이 비어있습니다.\n");
        return -1;
    }
    return top->data;
}


//isEmpty : Stack이 비어있는지 확인
int isEmpty(Node* top)
{
    if (top == NULL)
        return 1;           //맨 위 노드의 주소(top)가 NULL이면 비어있다는 것이므로 true(=1)
    return 0;               //맨 위 노드의 주소(top)가 NULL이 아니라는 것은 다른 무언가가 있다는 뜻이므로 false(=0)
}


//printStack : stack 내 모든 값 출력. (비어있으면 비어있다고 출력)
void printStack(Node* top)
{
    if (top == NULL)        //이하 생략
    {
        printf("Stack이 비어있습니다.\n");
        return;
    }

    while (top != NULL)     //이하 생략
    {
        printf("%d ", top->data);
        top = top->next;    //이하 생략
    }
    printf("\n");
}

int main()
{
    Node* topNode = NULL;
    /*
    topNode를 선언함과 동시에 NULL로 초기화(= 현재 Stack에 Node가 하나도 없다)
    그렇게하여 push에서 바로 next를 이용할 수 있도록 만들었다.
    또한 이왕 NULL로 초기화 했으니 stack이 비어있는지 확인하는 과정도 NULL을 이용하도록 효율적으로 만들었다.
    */
    push(&topNode, 1);
    push(&topNode, 2);
    push(&topNode, 3);              //임시 data값으로 1,2,3을 넣었다.
    //위 3개를 다 주석 처리했을 때 isEmpty는 1을 top은 -1을 출력하고 중간중간 pop이 작동하면서 비어있다는 메세지까지 출력되는 것을 확인했다.

    printf("Stack 내 모든 값 출력 : ");
    printStack(topNode);
    printf("Stack 크기 : %d\n", size(topNode));
    printf("top의 값 : %d\n", top(topNode));
    printf("isEmpty : %d\n", isEmpty(topNode));
    pop(&topNode);
    printf("pop 이후 Stack : ");
    printStack(topNode);

    return 0;
}
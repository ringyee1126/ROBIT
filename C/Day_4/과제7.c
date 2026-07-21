#include <stdio.h>
#include <stdlib.h>

//노드
typedef struct _Node {
    int data;
    struct _Node* next;     //next는 다음 노드의 주소를 가리키는 포인터이다.
}Node;      //자료구조 정의

//연결 리스트
typedef struct _LinkedList
{
    Node* head;     //첫번째 노드
    Node* tail;     //마지막 노드
    int size;       //현재 노드 개수 저장 -> 후에 반복문에서 사용
}LinkedList;

void list_init(LinkedList* list)
{
    list->head = NULL;
    list->tail = NULL;      //리스트를 처음 만들 때 빈 상태로 세팅하여 예기치 못한 오류를 방지한다.
    list->size = 0;
}

Node* create_node(int data)
{
    Node* new_node = (Node*)malloc(sizeof(Node));     //malloc으로 힙 메모리에 노드 하나를 새로 만들고 NULL로 바로 초기화한다.
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

//맨 처음에 노드 추가(리스트가 비어있었다면(size==0) tail도 새 노드로 설정)
void insert_first(LinkedList* list, int data) {
    Node* new_node = create_node(data);
    new_node->next = list->head;
    list->head = new_node;
    if (list->size == 0) list->tail = new_node;
    list->size++;
}

//맨 끝에 노드 추가
void insert_back(LinkedList* list, int data)
{
    Node* new_node = create_node(data);
    if (list->size == 0) {
        list->head = new_node;
        list->tail = new_node;
    }
    else
    {
        list->tail->next = new_node;
        list->tail = new_node;          //비어있으면 head와 tail 둘 다 새 노드로 설정.
    }
    list->size++;
}

//원하는 위치(index)에 노드 삽입 (0 <= index <= size)
void insert(LinkedList* list, int index, int data)
{
    if (index <= 0)
    {
        insert_first(list, data); return;
    }
    if (index >= list->size)
    {
        insert_back(list, data); return;
    }

    Node* new_node = create_node(data);
    Node* cur = list->head;
    for (int i = 0; i < index - 1; i++)
        cur = cur->next;

    new_node->next = cur->next;
    cur->next = new_node;
    list->size++;
}

//맨 처음 노드 삭제
void delete_first(LinkedList* list)
{
    if (list->size == 0)
        return;
    Node* target = list->head;
    list->head = list->head->next;
    if (list->head == NULL)
        list->tail = NULL;
    free(target);
    list->size--;
}

//맨 마지막 노드 삭제
void delete_back(LinkedList* list)
{
    if (list->size == 0) return;
    if (list->size == 1)
    {
        delete_first(list); return;
    }

    Node* cur = list->head;
    while (cur->next != list->tail)
        cur = cur->next;
    free(list->tail);
    list->tail = cur;
    list->tail->next = NULL;
    list->size--;
}

//index로 원하는 요소 삭제
void delete_by_index(LinkedList* list, int index)
{
    if (index < 0 || index >= list->size)
        return;
    if (index == 0)
    {
        delete_first(list); return;
    }
    if (index == list->size - 1)
    {
        delete_back(list); return;
    }

    Node* prev = list->head;
    for (int i = 0; i < index - 1; i++)
        prev = prev->next;

    Node* target = prev->next;
    prev->next = target->next;
    free(target);
    list->size--;
}

//data 값으로 원하는 요소 삭제 (처음 발견되는 노드 삭제)
void delete_by_data(LinkedList* list, int data) {
    Node* cur = list->head;
    int index = 0;
    while (cur != NULL)
    {
        if (cur->data == data)
        {
            delete_by_index(list, index);
            return;
        }
        cur = cur->next;
        index++;
    }
}

//index로 data 찾기 (get_entry: index -> data)
int get_entry_by_index(LinkedList* list, int index)
{
    if (index < 0 || index >= list->size)
        return -1;
    Node* cur = list->head;
    for (int i = 0; i < index; i++)
        cur = cur->next;
    return cur->data;
}

//data로 index 찾기 (get_entry: data -> index)
int get_entry_by_data(LinkedList* list, int data)
{
    Node* cur = list->head;
    int index = 0;
    while (cur != NULL)
    {
        if (cur->data == data)
            return index;
        cur = cur->next;
        index++;
    }
    return -1;
}

// 리스트 전체 길이 반환
int get_length(LinkedList* list)
{
    return list->size;
}

// 리스트의 모든 요소 출력
void print_list(LinkedList* list)
{
    Node* cur = list->head;
    while (cur != NULL) {
        printf("%d ", cur->data);
        cur = cur->next;
    }
    printf("\n");
}

// 리스트를 역순으로 만들기
void reverse(LinkedList* list)
{
    Node* prev = NULL;
    Node* cur = list->head;
    Node* next = NULL;

    list->tail = list->head;

    while (cur != NULL)
    {
        next = cur->next;
        cur->next = prev;
        prev = cur;
        cur = next;
    }
    list->head = prev;
}

int main(void)
{
    LinkedList list;
    list_init(&list);

    insert_back(&list, 100);
    insert_back(&list, 200);
    insert_back(&list, 300);
    printf("초기 리스트          : "); print_list(&list);

    insert(&list, 1, 150);
    printf("index1에 150 삽입 후 : "); print_list(&list);

    insert_first(&list, 50);
    printf("맨 앞에 50 삽입 후   : "); print_list(&list);

    delete_by_data(&list, 150);
    printf("data 150 삭제 후     : "); print_list(&list);

    delete_first(&list);
    printf("맨 앞 노드 삭제 후   : "); print_list(&list);

    delete_back(&list);
    printf("맨 뒤 노드 삭제 후   : "); print_list(&list);

    printf("index 1의 data       : %d\n", get_entry_by_index(&list, 1));
    printf("data 200의 index     : %d\n", get_entry_by_data(&list, 200));
    printf("리스트 길이          : %d\n", get_length(&list));

    reverse(&list);
    printf("역순 리스트          : "); print_list(&list);

    return 0;
}
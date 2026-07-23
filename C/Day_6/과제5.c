#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define MAX 100
#define SZ  50

typedef struct
{
    int   number;       //학생 번호
    char  name[SZ];     //학생 이름
    char  country[SZ];  //나라
    char  province[SZ]; //도
    char  city[SZ];     //시
    char  district[SZ]; //구
    float grade;        //성적
}Student;

Student db[MAX];        //최대 MAX명의 학생 정보를 저장할 구조체 배열(MAX는 100으로 정의)
int cnt = 0;            //현재 저장되어 있는 학생의 수


void trim(char* s)
{
    int i = 0;
    while (s[i] != '\0')    //i가 널문자가 나올때까지 계속 수를 증가시키고 널문자가 나오면 while문을 나오면서 하나 빼는 걸로 문자열의 길이를 찾는다
        i++;
    i--;
    while (i >= 0 && (s[i] == '\n' || s[i] == '\r'))    //fgets를 사용하면 \n이 문자열에 함께 저장될 수 있으므로, 문자열의 마지막에 '\n'이나 '\r'이 있다면 널문자로 바꾸어 제거한다.
        s[i--] = '\0';
}

/*
두 문자열이 같은지 확인하는 함수 (string.h의 strcmp 대신 사용)
두 문자열의 같은 위치에 있는 문자를 하나씩 비교 -> 문자가 다르거나 널문자가 나오면 반복을 멈춤
-> 마지막으로 같은 위치의 문자가 같은지 확인 -> 두 문자열이 처음부터 끝까지 완전히 같으면 1, 다르면 0을 반환
*/
int str_eq(char* a, char* b)
{
    int i = 0;
    while (a[i] != '\0' && a[i] == b[i])
        i++;
    return (a[i] == b[i]);
}

/*
두 문자열을 사전순으로 비교하는 함수
두 문자열의 해당하는 각 문자가 달라지는 순간까지 계속 다음 문자로 이동한다.
a의 문자가 b의 문자보다 크면 1을 반환 (정렬할 때 앞의 문자열이 뒤의 문자열보다 큰지 확인하기 위해 사용)
*/
int str_gt(char* a, char* b)
{
    int i = 0;
    while (a[i] != '\0' && a[i] == b[i])
        i++;
    return (a[i] > b[i]);
}

//입력 검증 : 문자열이 숫자로만 이루어져 있는지 확인하는 함수
int is_num(char* s) {
    int i;
    if (s[0] == '\0')
        return 0;         //아무것도 입력되지 않았다면 숫자가 아님
    for (i = 0; s[i] != '\0'; i++)
        if (s[i] < '0' || s[i] > '9') return 0;      //문자가 0보다 작거나 9보다 크면 숫자가 아님
    return 1;
}

//문자열이 올바른 실수 형태인지 확인하는 함수
int is_float_str(char* s) {
    int i, dot = 0;       //여기서 dot은 소수점이 몇 번 나왔는지 확인하는 변수이다.
    if (s[0] == '\0')
        return 0;
    for (i = 0; s[i] != '\0'; i++)
    {
        if (s[i] == '.')
        {
            if (dot)        //이미 소수점이 한번 나왔다면 소수점이 두개 이상인 잘못된실수이다(= 0 반환)
                return 0;
            dot = 1;
        }
        else if (s[i] < '0' || s[i] > '9')      //숫자와 소수점 이외의 문자가 나오면 잘못된 입력(= 0 반환)
            return 0;
    }
    return 1;       //위 경우가 전부 아닌 경우에만 1을 반환하여 올바른 실수 형태임을 확인
}

////문자열을 입력받는 함수
void get_str(char* prompt, char* buf, int sz)
{
    while (1)
    {
        printf("%s", prompt);
        fgets(buf, sz, stdin);      //공백을 포함한 문자열을 입력받기 위해 이번에 배운 fgets 사용
        trim(buf);      //입력된 문자열의 뒤에 있는 개행문자 제거를 위해 위에서 정의한 trim함수를 이용
        if (buf[0] != '\0')
            return;     //만약 문자열이 비어있지 않으면 입력 종료. 비어있다면 아래 메세지 출력
        printf("  [오류] 빈 입력입니다.\n");
    }
}

//정수를 입력받는 함수
int get_int(char* prompt) {
    char buf[20];
    while (1) {
        printf("%s", prompt);
        fgets(buf, 20, stdin);      //바로 정수로 입력받지 않고, 문자열로 입력받은 뒤 숫자로만 이루어졌는지 확인하는 과정을 통해 예기치 못한 문제를 예방
        trim(buf);
        if (is_num(buf))
            return atoi(buf);       //문자열을 정수로 변환하여 반환
        printf("  [오류] 숫자만 입력하세요.\n");
    }
}

//실수를 입력받는 함수
float get_float(char* prompt) {
    char buf[20];
    float v;
    while (1) {
        printf("%s", prompt);
        fgets(buf, 20, stdin);      //먼저 문자열로 입력받은 뒤 실수 형태인지 확인
        trim(buf);
        if (is_float_str(buf)) {
            v = (float)atof(buf);
            if (v >= 0.0f && v <= 4.5f)
                return v;       //성적은 대학교 성적을 기준으로 코드를 제작하였기에 성적은 0.0 이상 4.5 이하만 허용
            printf("  [오류] 0.0 ~ 4.5 범위로 입력하세요.\n");
        }
        else {
            printf("  [오류] 올바른 실수를 입력하세요.\n");
        }
    }
}

//출력 함수
void print_one(int i)       //db[i]를 이용하여 구조체 배열의 i번째 학생 정보에 접근한다.
{
    printf("  [%3d] %-12s | %s, %s, %s, %s | %.2f\n", db[i].number, db[i].name, db[i].country, db[i].province, db[i].city, db[i].district, db[i].grade);
}

//현재 저장된 모든 학생을 출력하는 함수
void print_all() {
    int i;
    if (cnt == 0) { printf("  학생이 없습니다.\n"); return; }
    for (i = 0; i < cnt; i++) print_one(i);
}

//정렬(버블 정렬) 함수
void swap_st(int i, int j)
{
    Student t = db[i];
    db[i] = db[j];          //구조체 전체를 서로 바꿈
    db[j] = t;
}

//주소의 하위 기준에 따라 나라, 도, 시, 구 중 하나의 문자열 주소를 반환하는 함수
char* get_addr(int i, int sub) {
    if (sub == 1)
        return db[i].country;       //나라
    if (sub == 2)
        return db[i].province;      //도
    if (sub == 3)
        return db[i].city;          //시

    return db[i].district;          //어짜피 return을 사용하면 이 함수가 끝나기 때문에 추가적인 조건문 없이 '구' 반환을 구현
}

//학생 정보를 선택한 기준에 따라 정렬하는 함수
void sort_menu() {
    int ch, sub, i, j, sw;
    sub = 0;
    printf("  정렬 기준: 1.번호  2.이름  3.주소  4.성적\n");
    ch = get_int("  선택: ");
    if (ch == 3)
    {
        printf("  주소 기준: 1.나라  2.도  3.시  4.구\n");       //만약 주소를 기준으로 정렬하는 경우라면 나라, 도, 시, 구 중 어떤 것을 기준으로 할지 다시 선택
        sub = get_int("  선택: ");
    }
    for (i = 0; i < cnt - 1; i++)       //서로 이웃한 두 학생을 비교하고 앞의 학생이 뒤의 학생보다 크면 두 학생의 위치를 바꾼다.
    {
        for (j = 0; j < cnt - 1 - i; j++)
        {
            sw = 0;      //두 학생의 위치를 바꿀지 저장하는 임시 변수
            if (ch == 1)
                sw = (db[j].number > db[j + 1].number);
            else if (ch == 2)
                sw = str_gt(db[j].name, db[j + 1].name);
            else if (ch == 3)
                sw = str_gt(get_addr(j, sub), get_addr(j + 1, sub));
            else if (ch == 4)
                sw = (db[j].grade > db[j + 1].grade);
            if (sw)
                swap_st(j, j + 1);
        }
    }
    printf("  [정렬 결과]\n");
    print_all();
}

//번호, 주소, 성적을 기준으로 학생을 찾는 함수
void search_menu() {
    int ch, i, found, sub;
    char buf[SZ];
    float g;
    found = 0;      //검색 결과가 있는지 확인하기 위한 임시 변수
    printf("  찾기 기준: 1.번호  2.주소  3.성적\n");
    ch = get_int("  선택: ");
    if (ch == 1)
    {
        int num = get_int("  번호 입력: ");
        for (i = 0; i < cnt; i++)
        {
            if (db[i].number == num)
            {
                printf("  -> %s\n", db[i].name);
                found = 1;      //검색 결과를 찾았으므로 1로 변경
            }
        }

    }
    else if (ch == 2) {
        printf("  주소 기준: 1.나라  2.도  3.시  4.구\n");
        sub = get_int("  선택: ");
        get_str("  값 입력: ", buf, SZ);
        for (i = 0; i < cnt; i++)
        {
            if (str_eq(get_addr(i, sub), buf))
            {
                printf("  -> %s\n", db[i].name);
                found = 1;
            }
        }

    }
    else if (ch == 3) {
        g = get_float("  성적 입력: ");
        for (i = 0; i < cnt; i++)
        {
            if (db[i].grade == g)
            {
                printf("  -> %s\n", db[i].name);
                found = 1;
            }
        }
    }

    if (!found)
        printf("  해당하는 학생이 없습니다.\n");
}

//새로운 학생을 출석부에 추가하는 함수
void add_menu()
{
    Student* s;
    if (cnt >= MAX)
    {
        printf("  최대 인원 초과\n");
        return;
    }
    /*
    현재 학생 수인 cnt를 배열의 위치로 사용한다.
    예를 들어 현재 학생이 3명이면
    db[3]에 새로운 학생을 저장한다.
    그리고 db[cnt]의 주소를 포인터 s에 저장하여
    s->를 이용해 해당 구조체의 멤버에 접근한다.
    */
    s = &db[cnt];
    s->number = get_int("  번호: ");

    get_str("  이름: ", s->name, SZ);
    get_str("  나라: ", s->country, SZ);
    get_str("  도: ", s->province, SZ);
    get_str("  시: ", s->city, SZ);
    get_str("  구: ", s->district, SZ);
    s->grade = get_float("  성적(0.0~4.5): ");

    cnt++;      //학생 한 명이 추가되었으므로 현재 학생 수를 1 증가시킴
    printf("  추가 완료!\n");
}

//idx 위치의 학생을 삭제하는 함수
void remove_idx(int idx)
{
    int i;
    for (i = idx; i < cnt - 1; i++)
        db[i] = db[i + 1];
    cnt--;      //학생 한 명이 삭제되었으므로 현재 학생 수를 1 감소시킴
}

//번호 또는 이름을 기준으로 학생을 삭제하는 함수
void delete_menu()
{
    int ch, i, ic, sel;
    int idxs[MAX];     //검색된 학생들의 배열 위치를 저장하는 배열
    char buf[SZ];
    ic = 0;            //검색된 학생의 수

    printf("  삭제 기준: 1.번호  2.이름\n");
    ch = get_int("  선택: ");

    if (ch == 1)
    {
        int num = get_int("  번호 입력: ");
        for (i = 0; i < cnt; i++)
        {
            if (db[i].number == num)
                idxs[ic++] = i;
        }
    }
    else
    {
        get_str("  이름 입력: ", buf, SZ);
        for (i = 0; i < cnt; i++)
        {
            if (str_eq(db[i].name, buf))
                idxs[ic++] = i;
        }
    }

    if (ic == 0)
    {
        printf("  해당 학생이 없습니다.\n");
        return;
    }
    if (ic == 1)        //검색된 학생이 한 명이라면 따로 과정 없이 바로 해당 학생을 삭제한다.
    {
        remove_idx(idxs[0]);
        printf("  삭제 완료!\n");
        return;
    }

//같은 번호나 이름을 가진 학생이 여러 명이라면 검색된 학생들을 출력한 뒤 삭제할 학생을 다시 선택한다.
    printf("  중복된 학생:\n");
    for (i = 0; i < ic; i++)
    {
        printf("  %d) ", i + 1);
        print_one(idxs[i]);
    }
    sel = get_int("  삭제할 번호 선택: ") - 1;
    if (sel < 0 || sel >= ic)
    {
        printf("  잘못된 선택\n");
        return;
    }
    remove_idx(idxs[sel]);
    printf("  삭제 완료!\n");
}

//현재 출석부의 학생 정보를 파일에 저장하는 함수
void save_file() {
    FILE* fp;
    int i;

    fp = fopen("attendance.txt", "w");      //이번에 배운 w(write : 쓰기)모드로 파일을 열어서 일이 없다면 새로 만들고 기존 파일이 있다면 내용을 새로 작성
    if (!fp)
    {
        printf("  파일 열기 실패\n");
        return;
    }
    fprintf(fp, "%d\n", cnt);               //파일의 첫 줄에는 현재 학생 수를 저장
    for (i = 0; i < cnt; i++)
        fprintf(fp, "%d\n%s\n%s\n%s\n%s\n%s\n%.2f\n", db[i].number, db[i].name, db[i].country, db[i].province, db[i].city, db[i].district, db[i].grade);
    fclose(fp);                             //파일 사용이 끝났으므로 닫아서 예기치 못한 문제를 막음
    printf("  저장 완료 → attendance.txt\n");
}

//파일에 저장된 학생 정보를 출석부로 불러오는 함수
void load_file()
{
    FILE* fp;
    int i;
    char buf[SZ];
    fp = fopen("attendance.txt", "r");      //읽기 모드(r : read)로 파일을 연다.
    if (!fp)
    {
        printf("  파일이 없습니다.\n");
    return;
    }

    fgets(buf, SZ, fp);                     //파일의 첫 번째 줄에 저장된 학생 수를 읽고, atoi를 이용하여 정수로 변환한다.
    trim(buf);
    cnt = atoi(buf);
    for (i = 0; i < cnt; i++)
    {
        fgets(buf, SZ, fp);
        trim(buf);
        db[i].number = atoi(buf);
        
        fgets(db[i].name, SZ, fp);      //이름 정보를 읽음
        trim(db[i].name);

        fgets(db[i].country, SZ, fp);   //주소(나라) 정보를 읽음
        trim(db[i].country);

        fgets(db[i].province, SZ, fp);  //주소(도) 정보를 읽음
        trim(db[i].province);

        fgets(db[i].city, SZ, fp);      //주소(시) 정보를 읽음
        trim(db[i].city);

        fgets(db[i].district, SZ, fp);  //주소(구) 정보를 읽음
        trim(db[i].district);

        fgets(buf, SZ, fp);             //성적을 문자열로 읽은 뒤 실수로 변환
        trim(buf);

        db[i].grade = (float)atof(buf);
    }
    fclose(fp);                         //이하 생략
    printf("  불러오기 완료 (%d명)\n", cnt);
}

int main()
{
    int ch;
    while (1) {
        printf("\n      출석부 프로그램\n");
        printf(" 1.전체출력  2.정렬      3.찾기\n");
        printf(" 4.추가      5.삭제\n");
        printf(" 6.저장      7.불러오기  0.종료\n");
        ch = get_int("선택: ");
        //입력받은 ch의 값에 따라 if문으로 쓰면 너무 복잡해질 수 있어서 실행할 기능을 switch문으로 선택
        switch (ch)
        {
        case 0: return 0;
        case 1:   print_all();    break;
        case 2:   sort_menu();    break;
        case 3:   search_menu();  break;
        case 4:   add_menu();     break;
        case 5:   delete_menu();  break;
        case 6:   save_file();    break;
        case 7:   load_file();    break;
        default:  printf("  잘못된 입력입니다.\n");
        }
    }
    return 0;
}
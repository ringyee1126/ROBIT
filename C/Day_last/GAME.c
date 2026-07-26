#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_ENM 20   //몬스터 종류는 넉넉하게 최대 20마리까지 가능
#define NM_LEN 50    //몬스터 이름도 넉넉하게 49글자까지
#define DS_LEN 200   //몬스터 설명, 대사도 넉넉하게 199글자까지

#define TH_YELLOW 0  // 평범한 방
#define TH_POOL 1    // 화장실
#define TH_DARK 2    // 암전 방
#define TH_RED 3     // 붉은 방
#define TH_NONE -1

//방 내용물
#define CT_EMPTY 0
#define CT_SAFE 1
#define CT_BLOCKED 2
#define CT_THREAT 3
#define CT_ITEM 4
#define CT_EXIT 5   //탈출구
#define CT_KEY  6   //열쇠

//방 구조체
typedef struct {
    int theme, content, collapsed, visited, revealed;
} Room;

//적 구조체
typedef struct {
    char name[NM_LEN];
    char desc[DS_LEN];
    char talk[DS_LEN];
    int  hp, atk, san_dmg;
} Enemy;

//플레이어 구조체
typedef struct {
    int x, y;           //위치
    int hp, hp_max;     //체력
    int san, san_max;   //정신력
    int hun, hun_max;   //허기
    int sta, sta_max;   //스테미나
    int atk;            //공격력
    int has_key;        //열쇠 보유 여부
    int turn, alive;    //턴, 생존여부
} Player;

//큐 구조체 - 동적 할당 (자료구조)
typedef struct {
    int* y_buf;
    int* x_buf;
    int head, tail, sz, cap;
} Queue;

//연결 리스트 노드 - BFS 경로 저장 (자료구조)
typedef struct _PathNode {
    int y, x;
    struct _PathNode* next;
} PathNode;

//전역 변수
int MAP_N = 0;          //동적할당으로 정할 맵 크기 (n×n). 처음에는 선언과 동시에 0으로 초기화
Room** g_map = NULL;    //맵 2D 동적 배열 (다차원배열)
int*** g_wt = NULL;     //3D WFC 가중치
Player g_pl;
Enemy* g_enm = NULL;
int g_enm_cnt = 0;

int g_exit_y = 0, g_exit_x = 0;  //탈출구 위치 선언과 동시에 0으로 초기화
int g_exit_found = 0;            //탈출구 발견 여부
int g_win = 0;                   //게임 클리어 여부

int g_dy[4] = { -1,  1,  0, 0 };
int g_dx[4] = { 0,  0, -1, 1 };

//WFC 영향 테이블 [from 테마][to 테마] (다차원 배열 사용)
int g_infl[4][4] = {
    { 6, 2, 1, 0 },
    { 2, 6, 0, 0 },
    { 1, 0, 5, 3 },
    { 0, 0, 3, 6 },
};

//내용물 확률 [테마][내용물] (합=100)
int g_ct_prob[4][5] = {
    { 30, 25, 15, 20, 10 },
    { 40, 35, 10,  5, 10 },
    { 20, 15, 25, 35,  5 },
    { 10, 10, 20, 55,  5 },
};

//이벤트 텍스트 (이중에서 랜덤으로 출력)
//기본 상태
char* txt_empty[] = {
    "텅 빈 공간이다. 과연 그럴까...",
    "형광등 소리만 울릴 뿐, 아무것도 없다.",
    "고요함이 귀를 먹먹하게 만든다.",
    NULL
};
//안전한 기본 방
char* txt_safe_room[] = {
    "형광등이 깜빡인다. 그 아래 뭔가 웅크리고 있다... 아니, 착각이겠지.",
    "평범한 복도다. 어딘가에서 들어본 적 있는 냄새가 난다.",
    "누군가 다녀간 흔적이 있다. 최근의 것 같다.",
    NULL
};
//화장실(toilet)
char* txt_safe_toilet[] = {
    "화장실이다. 수도꼭지에서 물이 뚝뚝 떨어지는 소리가 들린다.",
    "형광등이 깜빡이고 있다. 들어가기가 꺼려진다.",
    NULL
};
//암전 방(dark)
char* txt_safe_d[] = {
    "어둠 속에서 뭔가 숨 쉬는 소리가 들린다.",
    "눈을 뜨고 있지만 아무것도 보이지 않는다. 그런데 뭔가... 있다.",
    NULL
};
//붉은 방(red)
char* txt_safe_r[] = {
    "벽에 긁힌 자국이 가득하다.",
    "무언가 이 공간에서 흔적을 남겼다. 붉은 얼룩들이 있다.",
    NULL
};
//위험한 상태 (적 등장)
char* txt_threat[] = {
    "이곳에는 나보다 먼저 온 손님이 있다.",
    "무언가 이쪽을 바라보고 있다. 눈이 마주쳤다.",
    "숨소리가 들린다. 내 것이 아니다.",
    NULL
};

//fgets로 입력받으면 문자열 끝에 개행(\n)이 붙는데, 이걸 제거하는 함수
//string.h 없이 직접 끝 위치를 찾아서 뒤에서부터 제거
void trim(char* s)
{
    int i = 0;
    while (s[i] != '\0')    //널문자까지 이동 -> i가 문자열 길이가 됨
        i++;
    i--;
    while (i >= 0 && (s[i] == '\n' || s[i] == '\r'))    //개행 문자를 널문자로 교체
        s[i--] = '\0';
}

//값(v)이 lo~hi 범위를 벗어나면 강제로 범위 안으로 맞춤 (포인터로 직접 수정)
void clamp(int* v, int lo, int hi)
{
    if (*v < lo) *v = lo;
    if (*v > hi) *v = hi;
}

//NULL로 끝나는 문자열 포인터 배열의 원소 개수 반환
int msg_count(char** m)
{
    int i = 0;
    while (m[i])
        i++;
    return i; }

//문자열 배열에서 랜덤으로 하나 골라 반환 (배열이 비어있으면 ... 반환)
char* rand_msg(char** m)
{
    int n = msg_count(m);
    return (n > 0) ? m[rand() % n] : "...";
}

//지도 표시용 한 글자 코드(N=평범한방  T=화장실  D=암전방  R=붉은방  ?=미탐험(전체지도 전용))
char theme_char(int t)
{
    switch (t)
    {
    case TH_YELLOW:
        return 'N';
    case TH_POOL: 
        return 'T';
    case TH_DARK:   
        return 'D';
    case TH_RED:  
        return 'R';
    default:      
        return '?';
    }
}

//내용물 우선으로 표시 문자 결정
//탈출구(E), 열쇠(K)는 게임의 중요한 아이템이라 테마보다 우선해서 표시
char cell_char(int y, int x)
{
    if (g_map[y][x].content == CT_EXIT)
        return 'E';
    if (g_map[y][x].content == CT_KEY)  
        return 'K';
    return theme_char(g_map[y][x].theme);
}

//딜레이 함수 (time.h 활용)
//CPU를 계속 돌리면서 sec초가 지날 때까지 기다리는 방식
void delay(double sec)
{
    clock_t start = clock();
    while ((double)(clock() - start) / CLOCKS_PER_SEC < sec);    //while을 이용하여 경과시간이 sec초 미만이면 계속 대기
}

//msg 출력 후 sec초 딜레이 (fflush로 즉시 출력 보장)
void dprint(char* msg, double sec)
{
    printf("%s", msg);
    fflush(stdout);       //버퍼에 남은 출력을 즉시 콘솔에 내보냄 (딜레이 전에 출력이 먼저 되게)
    delay(sec);
}

//동적 메모리 관리 함수
//n×n 크기의 int 2D 배열을 동적 할당 (calloc으로 0 초기화)
int** alloc_2d_int(int n)
{
    int i;
    int** arr = (int**)malloc(sizeof(int*) * n);      //행 포인터 배열 할당
    for (i = 0; i < n; i++)
        arr[i] = (int*)calloc(n, sizeof(int));        //각 행의 열 배열 할당 + 0으로 초기화으로 초기화 함으로써 이 방은 방문하지 않았음을 자연스럽게 의미함
    return arr;
}

//alloc_2d_int로 만든 배열 해제
void free_2d_int(int** arr, int n)
{
    int i;
    for (i = 0; i < n; i++) free(arr[i]);
    free(arr);
}

//맵 전체 동적 할당
//g_map(Room 2D), g_wt(WFC 가중치 3D) 를 n×n 크기로 할당
void alloc_map(int n)
{
    int i, j;
    g_map = (Room**)malloc(sizeof(Room*) * n);
    g_wt = (int***)malloc(sizeof(int**) * n);
    for (i = 0; i < n; i++)
    {
        g_map[i] = (Room*)malloc(sizeof(Room) * n);
        g_wt[i] = (int**)malloc(sizeof(int*) * n);
        for (j = 0; j < n; j++)
            g_wt[i][j] = (int*)malloc(sizeof(int) * 4);
    }
}

//alloc_map으로 만든 맵 전체 메모리 해제 (안쪽부터 순서대로)
void free_map(void)
{
    int i, j;
    if (!g_map)
        return;
    for (i = 0; i < MAP_N; i++)
    {
        for (j = 0; j < MAP_N; j++)
            free(g_wt[i][j]);
        free(g_wt[i]);
        free(g_map[i]);
    }
    free(g_wt); free(g_map);
    g_map = NULL; g_wt = NULL;
}

//큐 함수 - 동적 할당 버전 (WFC + BFS에서 사용)
//cap 크기의 원형 큐 초기화 (y좌표 배열, x좌표 배열 따로 동적 할당)
void q_init(Queue* q, int cap)
{
    q->y_buf = (int*)malloc(sizeof(int) * cap);
    q->x_buf = (int*)malloc(sizeof(int) * cap);
    q->head = q->tail = q->sz = 0;
    q->cap = cap;
}

//큐에 사용한 동적 배열 해제
void q_free(Queue* q)
{
    free(q->y_buf);
    free(q->x_buf);
}

//큐가 비어있으면 1 반환
int q_empty(Queue* q)
{
    return q->sz == 0;      //true는 1이다.
}

//큐 끝(tail)에 좌표(y,x) 삽입
//(tail+1) % cap : 끝에 도달하면 처음으로 돌아오는 원형 구조
void q_push(Queue* q, int y, int x)
{
    if (q->sz >= q->cap)  //큐가 가득 차면 무시
        return;
    q->y_buf[q->tail] = y;
    q->x_buf[q->tail] = x;
    q->tail = (q->tail + 1) % q->cap;
    q->sz++;
}

//큐 앞(head)에서 좌표 꺼내기 (먼저 넣은 것 먼저 나옴)
void q_pop(Queue* q, int* y, int* x)
{
    *y = q->y_buf[q->head];
    *x = q->x_buf[q->head];
    q->head = (q->head + 1) % q->cap;
    q->sz--;
}

//연결 리스트 함수 - BFS 경로 저장
//새 노드를 리스트 맨 앞에 삽입 (헤드 삽입)
//역추적 순서(탈출구→플레이어)로 삽입하면, 리스트는 올바른 순서(플레이어→탈출구)가 됨
PathNode* pn_push(PathNode* head, int y, int x)
{
    PathNode* node = (PathNode*)malloc(sizeof(PathNode));
    node->y = y;
    node->x = x;
    node->next = head;       //기존 헤드를 뒤로 밀고 새 노드가 앞이 됨
    return node;
}

//연결 리스트 전체 메모리 해제
void pn_free(PathNode* head)
{
    PathNode* cur = head, * nxt;
    while (cur)
    {
        nxt = cur->next; free(cur);
        cur = nxt;
    }
}

//연결 리스트 노드 수 반환 (경로 총 칸 수)
int pn_len(PathNode* head)
{
    int cnt = 0;
    PathNode* cur = head;
    while (cur)
    {
        cnt++; cur = cur->next;
    }
    return cnt;
}

//WFC (파동함수 붕괴)
//가중치 배열(w)을 보고 확률적으로 테마 인덱스(0~3) 하나를 선택
//전체 합(total)에서 랜덤값을 뽑고, 누적합이 랜덤값을 넘는 순간의 인덱스 반환
int weighted_pick(int* w)
{
    int total = 0, r, i;
    for (i = 0; i < 4; i++)
        total += w[i];         //가중치 총합 계산
    if (total <= 0)
        return rand() % 4;    //가중치가 모두 0이면 완전 랜덤
    r = rand() % total;       //0 ~ total-1 사이 랜덤값
    for (i = 0; i < 4; i++)
    {
        r -= w[i];            //해당 구간에 걸리면 이 테마 선택
        if (r < 0)
            return i;
    }
    return 3;
}

//테마에 맞는 내용물을 확률 테이블(g_ct_prob)로 랜덤 선택
//0~99 랜덤값이 누적 확률 구간에 속하는 내용물 반환
int pick_content(int theme)
{
    int r = rand() % 100, acc = 0, i;
    for (i = 0; i < 5; i++)
    {
        acc += g_ct_prob[theme][i];
        if (r < acc)
            return i;
    }
    return CT_EMPTY;
}

//방이 확정됐을 때 상하좌우 인접 미확정 방들의 가중치를 수정 (WFC 전파)
//g_infl 테이블을 보고 인접 방이 특정 테마가 될 확률을 높이거나 낮춤
//이 덕분에 같은 테마끼리 지역적으로 모이는 효과가 생김
void propagate(int y, int x, int theme)
    {
    int d, ny, nx, t;
    for (d = 0; d < 4; d++)
    {
        ny = y + g_dy[d];
        nx = x + g_dx[d];
        if (ny < 0 || ny >= MAP_N || nx < 0 || nx >= MAP_N)     //맵 범위 벗어나면 스킵
            continue;       
        if (g_map[ny][nx].collapsed)                            //이미 확정된 방은 건드리지 않음
            continue;
        for (t = 0; t < 4; t++)
            g_wt[ny][nx][t] += g_infl[theme][t];                //테마별 가중치 추가
    }
}

//탈출구와 열쇠를 랜덤 위치에 배치
void place_exit_and_key(void)
{
    int sy = MAP_N / 2, sx = MAP_N / 2;
    int y, x;

    //탈출구 배치
    do
    {
        y = rand() % MAP_N; x = rand() % MAP_N;
    } while ((y == sy && x == sx) || g_map[y][x].content == CT_BLOCKED);        //시작점과 막힌 방은 제외
    g_map[y][x].content = CT_EXIT;
    g_exit_y = y; g_exit_x = x;

    //열쇠 배치
    do {
        y = rand() % MAP_N; x = rand() % MAP_N;
    } while ((y == sy && x == sx) ||
        (y == g_exit_y && x == g_exit_x) ||
        g_map[y][x].content == CT_BLOCKED ||
        g_map[y][x].content == CT_EXIT);         //시작점, 탈출구, 막힌 방 모두 제외
    g_map[y][x].content = CT_KEY;
}

//WFC 방식으로 n×n 맵 전체를 생성하는 메인 함수
void generate_map(void)
{
    Queue q;
    int** in_q;
    int y, x, t, d, ny, nx, theme;
    int sy = MAP_N / 2, sx = MAP_N / 2;

    //초기화
    for (y = 0; y < MAP_N; y++)
        for (x = 0; x < MAP_N; x++)
        {
            g_map[y][x].theme = TH_NONE;
            g_map[y][x].content = CT_EMPTY;
            g_map[y][x].collapsed = g_map[y][x].visited = g_map[y][x].revealed = 0;
            for (t = 0; t < 4; t++) g_wt[y][x][t] = 3;      //모든 방의 초기 가중치를 3으로 동일하게 설정
        }

    in_q = alloc_2d_int(MAP_N);         //큐에 넣었는지 여부 체크용 2D 배열 (calloc으로 0 초기화)
    q_init(&q, MAP_N * MAP_N);

    //시작점 확정 (시작 위치는 안전 + 기본 방)
    g_map[sy][sx].theme = TH_YELLOW;
    g_map[sy][sx].collapsed = 1;
    g_map[sy][sx].content = CT_EMPTY;
    propagate(sy, sx, TH_YELLOW);       //시작점 확정 후 바로 주변에 영향 전파

    //시작점의 상하좌우를 큐에 넣어 WFC 탐색 시작
    for (d = 0; d < 4; d++)
    {
        ny = sy + g_dy[d];
        nx = sx + g_dx[d];
        if (ny < 0 || ny >= MAP_N || nx < 0 || nx >= MAP_N)
            continue;
        q_push(&q, ny, nx);
        in_q[ny][nx] = 1;
    }

    //WFC 루프
    //큐에서 방을 하나씩 꺼내 테마 확정 -> 주변 전파 -> 인접 미확정 방 큐에 추가
    while (!q_empty(&q))
    {
        q_pop(&q, &y, &x);
        in_q[y][x] = 0;
        if (g_map[y][x].collapsed)            //이미 확정됐으면 스킵 (중복 처리 방지)
            continue;
        theme = weighted_pick(g_wt[y][x]);    //가중치 기반으로 이 방의 테마 확정
        g_map[y][x].theme = theme;
        g_map[y][x].collapsed = 1;
        g_map[y][x].content = pick_content(theme);
        propagate(y, x, theme);               //확정된 테마를 주변에 전파
        for (d = 0; d < 4; d++)
        {
            ny = y + g_dy[d];
            nx = x + g_dx[d];
            if (ny < 0 || ny >= MAP_N || nx < 0 || nx >= MAP_N)
                continue;
            if (!g_map[ny][nx].collapsed && !in_q[ny][nx])
            {
                q_push(&q, ny, nx);           //아직 큐에 없는 미확정 방만 추가
                in_q[ny][nx] = 1;
            }
        }
    }
    q_free(&q);
    free_2d_int(in_q, MAP_N);

    //미확정 방 처리  (큐가 끝났는데 아직 남은 방이 있으면 가중치로 확정)
    for (y = 0; y < MAP_N; y++)
        for (x = 0; x < MAP_N; x++)
            if (!g_map[y][x].collapsed)
            {
                g_map[y][x].theme = weighted_pick(g_wt[y][x]);
                g_map[y][x].collapsed = 1;
                g_map[y][x].content = pick_content(g_map[y][x].theme);
            }

    place_exit_and_key();    //맵 완성 후 탈출구와 열쇠 배치
}

//시야 업데이트
//플레이어 현재 위치를 방문 처리하고, 주변 3×3 범위를 미니맵에 공개
void update_vision(void)
{
    int dy, dx, ny, nx;
    g_map[g_pl.y][g_pl.x].visited = g_map[g_pl.y][g_pl.x].revealed = 1;     //현재 위치는 방문+공개 상태
    for (dy = -1; dy <= 1; dy++)
        for (dx = -1; dx <= 1; dx++)
        {
            ny = g_pl.y + dy; nx = g_pl.x + dx;
            if (ny >= 0 && ny < MAP_N && nx >= 0 && nx < MAP_N)
                g_map[ny][nx].revealed = 1;      //주변 8칸은 미니맵에만 표시 (방문은 아니라서 전체 지도에서는 안보임)
        }
}

//BFS 경로 탐색 (알고리즘) + 연결 리스트로 경로 저장
//현재 플레이어 위치에서 탈출구까지 막힌 방을 피한 최단 경로를 탐색
void find_path_to_exit(void)
{
    int** visited, ** par_y, ** par_x;
    int i, j, cy, cx, ny, nx, d, step, len, py, px;
    Queue q;
    PathNode* path = NULL;
    PathNode* cur;
    char* dir;

    if (!g_exit_found)
    {
        printf("  탈출구를 아직 발견하지 못했습니다.\n");
        return;
    }

    //BFS에 필요한 방문 여부 + 부모 좌표 2D 배열 할당
    visited = alloc_2d_int(MAP_N);
    par_y = alloc_2d_int(MAP_N);  //각 칸의 부모 y좌표 저장 (경로 역추적용)
    par_x = alloc_2d_int(MAP_N);  //각 칸의 부모 x좌표 저장 (경로 역추적용)

    for (i = 0; i < MAP_N; i++)
        for (j = 0; j < MAP_N; j++)
            par_y[i][j] = par_x[i][j] = -1;      //-1은 아직 방문 안 한 상태

    q_init(&q, MAP_N * MAP_N);
    visited[g_pl.y][g_pl.x] = 1;
    q_push(&q, g_pl.y, g_pl.x);  //플레이어 위치에서 탐색 시작

    //BFS
    while (!q_empty(&q)) 
    {
        q_pop(&q, &cy, &cx);
        if (cy == g_exit_y && cx == g_exit_x)   //탈출구 도달 시 탐색 종료
            break;
        for (d = 0; d < 4; d++)
        {
            ny = cy + g_dy[d];
            nx = cx + g_dx[d];
            if (ny < 0 || ny >= MAP_N || nx < 0 || nx >= MAP_N)
                continue;
            if (visited[ny][nx])    //이미 방문한 칸 스킵
                continue;
            if (g_map[ny][nx].content == CT_BLOCKED)   //막힌 방은 경로에서 제외
                continue;
            visited[ny][nx] = 1;
            par_y[ny][nx] = cy;     //이 칸에 오기 직전 좌표(부모) 기록
            par_x[ny][nx] = cx;
            q_push(&q, ny, nx);
        }
    }
    q_free(&q);

    if (!visited[g_exit_y][g_exit_x]) 
    {
        dprint("  경로를 찾을 수 없습니다. (막힌 길)\n", 0.5);
    }
    else 
    {
        /*
        역추적 - 연결 리스트에 앞에서 삽입->플레이어->탈출구 순서
        탈출구에서 시작해 부모를 따라가며 리스트 맨 앞에 삽입
        -> 결과적으로 리스트는 플레이어 방향에서 탈출구 방향으로 정렬됨
        */
        cy = g_exit_y; cx = g_exit_x;
        while (!(cy == g_pl.y && cx == g_pl.x)) 
        {
            path = pn_push(path, cy, cx);
            py = par_y[cy][cx];
            px = par_x[cy][cx];
            cy = py; cx = px;
        }

        len = pn_len(path);
        printf("\n  === 탈출구까지 최단 경로 (총 %d칸) ===\n", len);
        cur = path;
        step = 1;
        while (cur) 
        {
            printf("  %2d단계: (%2d, %2d)", step++, cur->x, cur->y);
            if (cur->next) 
            {
                if (cur->next->y < cur->y) 
                    dir = "[북]";
                else if 
                    (cur->next->y > cur->y)
                    dir = "[남]";
                else if 
                    (cur->next->x < cur->x)
                    dir = "[서]";
                else                         
                    dir = "[동]";
                printf("  %s", dir);
            }
            else 
            {
                printf("  [탈출구]");      //마지막 노드는 탈출구임을 표시
            }
            printf("\n");
            cur = cur->next;
        }
        pn_free(path);      //경로 출력 후 연결 리스트 메모리 해제
    }

    free_2d_int(visited, MAP_N);
    free_2d_int(par_y, MAP_N);
    free_2d_int(par_x, MAP_N);
}

//치트 모드 (발표 시간 관계상 코드 시연을 통해 빠르게 게임을 보여주기 위한 임시 기능)
void activate_cheat(void) 
{
    int y, x;
    //맵 전체 공개 (방문+공개 처리)
    for (y = 0; y < MAP_N; y++)
        for (x = 0; x < MAP_N; x++)
        {
            g_map[y][x].revealed = 1;
            g_map[y][x].visited = 1;
        }
    g_pl.hp_max = 500; g_pl.hp = 500;
    g_pl.atk = 100;
    g_pl.has_key = 1;
    g_exit_found = 1;

    dprint("\n          치트 활성화!\n", 0.3);
    printf("  HP: 500  /  공격력: 100  /  열쇠 획득\n");
    printf("  탈출구 위치: (%d, %d)\n", g_exit_x, g_exit_y);
    printf("  전체 맵 공개 완료\n");
}

//디스플레이 함수
//스탯을 [########..] 형태의 바(Bar)로 출력하여 사용자 편의성 증가
void print_bar(int v, int m) 
{
    int filled = (m > 0) ? (v * 10 / m) : 0, i;     //최대값 대비 현재값 비율로 채울 칸 수 계산
    printf("[");
    for (i = 0; i < 10; i++) 
        printf(i < filled ? "#" : ".");             //채워진 칸은 #, 빈 칸은 .
    printf("] %3d/%3d\n", v, m);
}

//현재 위치 중심의 3×3 미니맵 출력
//[@]=현재위치, [?]=미탐험, [#]=막힘, 그외=테마 문자
void print_minimap(void) 
{
    int dy, dx, ny, nx;
    printf("\n  [미니맵]  N=평범 T=화장실 D=암전 R=붉은 E=탈출구 K=열쇠\n");
    printf("  +---+---+---+\n");        //사용자 편의성을 위해 보기 편하라고 테두리를 직접 함께 출력
    for (dy = -1; dy <= 1; dy++) 
    {
        printf("  |");
        for (dx = -1; dx <= 1; dx++)
        {
            ny = g_pl.y + dy;
            nx = g_pl.x + dx;
            if (ny < 0 || ny >= MAP_N || nx < 0 || nx >= MAP_N) 
            {
                printf("   |");     //맵 범위 밖은 빈칸
            }
            else if (ny == g_pl.y && nx == g_pl.x) 
            {
                printf("[@]|");     //현재 위치
            }
            else if (g_map[ny][nx].revealed) 
            {
                if (g_map[ny][nx].content == CT_BLOCKED && g_map[ny][nx].visited)
                    printf("[#]|");     //이미 시도했던 막힌 방(시도 전에는 #으로 안떠서 막힌 방인지 모름)
                else
                    printf("[%c]|", cell_char(ny, nx));     //공개된 방의 테마 or 특수 내용물(K or E 등)
            }
            else {
                printf("[?]|");         //아직 공개 안 된 방(전체 지도 전용)
            }
        }
        printf("\n");
        if (dy < 1) 
            printf("  +---+---+---+\n");
    }
    printf("  +---+---+---+\n");
}

//M키로 맵 전체 열기 코드
//방문한 방은 테마 문자, 공개만 된 방은 ?, 미발견은 공백으로 표시
void print_fullmap(void)
{
    int y, x;
    system("cls");
    printf("===== 탐험 지도 (%dx%d) =====\n", MAP_N, MAP_N);
    printf("N=평범 T=화장실 D=암전 R=붉은 E=탈출구 K=열쇠 @=현재 #=막힘\n\n");
    for (y = 0; y < MAP_N; y++)
    {
        printf("  ");
        for (x = 0; x < MAP_N; x++) 
        {
            if (y == g_pl.y && x == g_pl.x) 
                printf("[@]");
            else if (g_map[y][x].visited) 
            {
                if (g_map[y][x].content == CT_BLOCKED)
                    printf("[#]");
                else 
                    printf("[%c]", cell_char(y, x));
            }
            else if (g_map[y][x].revealed)
            {
                printf("[?]");
            }
            else 
            {
                printf("   ");      //아직 발견도 못한 방은 공백으로 숨김 (근처도 아닌 방이 ?로 뜨면 눈이 아픔고 현재 위치 파악이 힘듦)
            }
        }
        printf("\n");
    }
    printf("\n===================================\n");
    printf("  [Enter를 눌러 게임으로 돌아갑니다]");
    getchar();
}

//메인 HUD (플레이어 상태 + 미니맵 + 행동 메뉴) 출력
void print_hud(void) 
{
    system("cls");
    printf("\n\n\n\n\n  위치: (%2d,%2d)  |  턴: %d  |  현재 공간: ", g_pl.x, g_pl.y, g_pl.turn);
    switch (g_map[g_pl.y][g_pl.x].theme)
    {
    case TH_YELLOW: 
        printf("평범한 방\n\n\n\n"); 
        break;
    case TH_POOL:   
        printf("화장실\n\n\n\n");   
        break;
    case TH_DARK:  
        printf("암전 방\n\n\n\n");  
        break;
    case TH_RED: 
        printf("붉은 방\n\n\n\n");  
        break;
    default:    
        printf("???\n\n\n\n");     
        break;
    }
    printf("  열쇠: %s\n", g_pl.has_key ? "[보유]" : "[없음]");
    printf("----------------------------------------\n");
    printf("  체력     "); 
    print_bar(g_pl.hp, g_pl.hp_max);
    printf("  정신력   ");
    print_bar(g_pl.san, g_pl.san_max);
    printf("  허기     "); 
    print_bar(g_pl.hun, g_pl.hun_max);
    printf("  스테미나 "); 
    print_bar(g_pl.sta, g_pl.sta_max);
    printf("----------------------------------------\n");
    print_minimap();
    printf("\n  1.이동   2.방 조사   3.휴식   4.상태확인\n");
    printf("  5.치트(임시)   M.전체지도   0.종료\n");
    if (g_exit_found)       //탈출구를 발견했을 때만 경로 탐색 메뉴 표시
        printf("  6.탈출구까지 경로 탐색 (탈출구: %d,%d)\n", g_exit_x, g_exit_y);
    printf("  선택: ");
}

//enemies.txt에서 적 데이터 받아오기 함수
//파일 입출력: 적 데이터 로드 (파일입출력 + 동적할당 사용) (enemies.txt: ANSI(CP949) 인코딩으로 저장 필수 <- 글자 깨짐 방지)
void load_enemies(void)
{
    FILE* fp;
    char  line[DS_LEN];

    fp = fopen("enemies.txt", "r");
    if (!fp)
    {
        printf("  [경고] enemies.txt 없음. 적 없이 시작합니다.\n");
        return;
    }
    g_enm = (Enemy*)malloc(sizeof(Enemy) * MAX_ENM);         //최대 마리 수만큼 동적 할당
    if (!g_enm)
    { 
        fclose(fp); return;
    }

    g_enm_cnt = 0;
    while (g_enm_cnt < MAX_ENM) {
        if (!fgets(g_enm[g_enm_cnt].name, NM_LEN, fp))      //이름 한 줄 읽기
            break;
        trim(g_enm[g_enm_cnt].name);
        if (g_enm[g_enm_cnt].name[0] == '\0')               //빈 줄이면 스킵 (몬스터 구분자)
            continue;
        if (!fgets(line, DS_LEN, fp))   
            break;
        trim(line);
        g_enm[g_enm_cnt].hp = atoi(line);           //체력
        if (!fgets(line, DS_LEN, fp))
            break;
        trim(line);
        g_enm[g_enm_cnt].atk = atoi(line);          //공격력
        if (!fgets(line, DS_LEN, fp)) 
            break; 
        trim(line);
        g_enm[g_enm_cnt].san_dmg = atoi(line);      //정신력 피해
        if (!fgets(g_enm[g_enm_cnt].desc, DS_LEN, fp))
            break;
        trim(g_enm[g_enm_cnt].desc);                //등장 설명
        if (!fgets(g_enm[g_enm_cnt].talk, DS_LEN, fp))
            break;
        trim(g_enm[g_enm_cnt].talk);                //대사
        fgets(line, DS_LEN, fp);                    //몬스터 사이 구분 빈 줄 소비
        g_enm_cnt++;
    }
    fclose(fp);
    printf("  [적 데이터 %d개 로드 완료]\n", g_enm_cnt);
}

///전투 함수 (구조체 포인터 - Enemy *e)
//턴제 방식: 플레이어 행동 선택 -> 적 행동 (랜덤 3가지 중 하나) 순으로 반복
void combat(Enemy* e) {
    int choice, enemy_hp;
    enemy_hp = e->hp;       //원본 Enemy 구조체 HP를 건드리지 않기 위해 지역 변수로 복사

    dprint("\n  ===== 조우! =====\n", 0.5);
    printf("  [ %s ]\n", e->name);
    dprint(e->desc, 1.0);
    printf("\n\n"); 
    delay(0.3);

    while (g_pl.hp > 0 && enemy_hp > 0)
    {
        printf("  [ %s ] HP:%d  |  나의 HP:%d  정신력:%d  스테미나:%d\n", e->name, enemy_hp, g_pl.hp, g_pl.san, g_pl.sta);
        printf("  1.공격   2.도망(스테미나 30+/40 소모)   3.대화   4.관찰\n  선택: ");
        scanf("%d", &choice);
        while (getchar() != '\n');      //버퍼에 남은 문자들을 엔터가 나올 때까지 전부 버리는 역할

        switch (choice)
        {
        case 1:
            enemy_hp -= g_pl.atk;       //플레이어 공격력만큼 적 HP 감소
            printf("  적에게 %d 피해를 입혔다!\n", g_pl.atk);
            delay(0.5);
            if (enemy_hp <= 0) 
            { 
                dprint("  처치했다!\n", 1.0); return; 
            }
            break;
        case 2:
            if (g_pl.sta < 30)
            {
                dprint("  스테미나 부족! (도망: 30 이상 필요)\n", 0.5);
                printf("  현재 스테미나: %d\n", g_pl.sta);
                break;
            }
            g_pl.sta -= 40;              //도망 시도 시 스테미나 40 소모
            clamp(&g_pl.sta, 0, g_pl.sta_max);
            printf("  스테미나 -40 (남은: %d)\n", g_pl.sta);
            if (rand() % 3 != 0)         //66% 확률로 도망
            { 
                dprint("  도망 성공!\n", 0.7);
            return; 
            }
            dprint("  도망 실패!\n", 0.7);
            break;
        case 3:
            printf("  [ %s ]: \"", e->name);
            fflush(stdout); delay(0.7);
            dprint(e->talk, 0.8);       //적의 대사 출력
            printf("\"\n");
            break;
        case 4:
            dprint(e->desc, 0.8);
            printf("\n");
            g_pl.san -= 3;              //관찰하면 정신력 소모
            clamp(&g_pl.san, 0, g_pl.san_max);
            break;
        default:
            printf("  ...\n");
        }

        //적 행동 (플레이어가 쓰러뜨리지 못한 경우)
        if (enemy_hp > 0)
        {
            int act = rand() % 3;    //0=공격, 1=정신력 공격, 2=지켜봄 (랜덤)
            delay(0.5);
            if (act == 0) 
            {
                g_pl.hp -= e->atk; 
                clamp(&g_pl.hp, 0, g_pl.hp_max);
                printf("  [ %s ]이(가) 공격! -%d HP\n", e->name, e->atk);
            }
            else if (act == 1)
            {
                g_pl.san -= e->san_dmg; 
                clamp(&g_pl.san, 0, g_pl.san_max);
                dprint("  존재 자체가 정신을 갉아먹는다...", 0.5);
                printf("  -%d 정신력\n", e->san_dmg);
            }
            else 
            {
                printf("  [ %s ]이(가) 가만히 이쪽을 바라본다...\n", e->name);
                g_pl.san -= 2;
                clamp(&g_pl.san, 0, g_pl.san_max);
            }
        }

        g_pl.san -= 3;       //전투 중 매 턴 정신력 자동 감소 (긴장 상태 표현)
        g_pl.turn++;
        clamp(&g_pl.san, 0, g_pl.san_max);
        if (g_pl.hp <= 0) 
        {
            dprint("\n  체력이 바닥났다...\n", 1.0);
            g_pl.alive = 0;
            return;
        }
        if (g_pl.san <= 0)
        { 
            dprint("\n  정신이 무너진다...\n", 1.0);
            g_pl.alive = 0; 
            return; 
        }
        printf("\n");
        delay(0.3);
    }
}

//방 이벤트 처리 (구조체 포인터 - Room *r)
//아이템이 있는 방에 진입했을 때 호출 - 즉시 사용 또는 버리기만 가능 (인벤토리 기능 현재 미구현)
void item_event(Room* r) 
{
    int choice;
    if (r->theme == TH_POOL)
    {
        dprint("  바닥에 아직 포장이 뜯기지 않은 과자가 있다.\n", 0.5);
        printf("  1.먹는다   2.버린다\n  선택: ");
        scanf("%d", &choice); 
        while (getchar() != '\n');
        if (choice == 1) 
        {
            g_pl.hp += 10; 
            clamp(&g_pl.hp, 0, g_pl.hp_max);
            g_pl.hun += 15; 
            clamp(&g_pl.hun, 0, g_pl.hun_max);
            dprint("  [HP +10, 허기 +15]\n", 0.3);
        }
    }
    else {
        dprint("  괴물 사탕이 하나 바닥에 떨어져 있다.\n", 0.5);
        printf("  1.먹는다   2.버린다\n  선택: ");
        scanf("%d", &choice); 
        while (getchar() != '\n');
        if (choice == 1) 
        {
            g_pl.hp += 15;
            clamp(&g_pl.hp, 0, g_pl.hp_max);
            g_pl.hun += 10; 
            clamp(&g_pl.hun, 0, g_pl.hun_max);
            dprint("  [HP +15, 허기 +10]\n", 0.3);
        }
    }
    r->content = CT_EMPTY;      //아이템은 1회성 - 사용 여부 관계없이 방에서 제거
}

//방 진입 시 호출 - 내용물(content)에 따라 다른 이벤트 처리
//구조체 포인터(Room *r)로 현재 방 데이터에 직접 접근 및 수정
void enter_room(void) 
{
    Room* r = &g_map[g_pl.y][g_pl.x];       //현재 위치의 방 구조체 포인터
    int   th = r->theme;
    char** safe_txt;

    //테마별 정신력 감소 (위험한 공간일수록 심리적 압박)
    if (th == TH_DARK)
    {
        g_pl.san -= 5; 
        clamp(&g_pl.san, 0, g_pl.san_max);
        dprint("\n  어둠이 정신을 갉아먹는다...", 0.8);
        printf("  [-5 정신력]\n");
    }
    else if (th == TH_RED)
    {
        g_pl.san -= 3;
        clamp(&g_pl.san, 0, g_pl.san_max);
        dprint("\n  이 공간의 공기가 불안하다...", 0.8);
        printf("  [-3 정신력]\n");
    }
    delay(0.4);

    //내용물(content)에 따라 이벤트 분기
    switch (r->content) 
    {
    case CT_EMPTY:
        dprint(rand_msg(txt_empty), 0.3); 
        printf("\n");
        break;
    case CT_SAFE:               //테마에 맞는 분위기 묘사 텍스트 선택
        if (th == TH_POOL)
            safe_txt = txt_safe_toilet;
        else if (th == TH_DARK)
            safe_txt = txt_safe_d;
        else if (th == TH_RED)
            safe_txt = txt_safe_r;
        else                
            safe_txt = txt_safe_room;
        dprint(rand_msg(safe_txt), 0.3);
        printf("\n");
        break;
    case CT_THREAT:
        dprint(rand_msg(txt_threat), 0.8); 
        printf("\n");
        delay(0.5);
        if (g_enm_cnt > 0)
            combat(&g_enm[rand() % g_enm_cnt]);     //적 랜덤 선택 후 전투
        else
            printf("  (이미 떠난 것 같다...)\n");
        break;
    case CT_ITEM:
        item_event(r);
        break;
    case CT_EXIT:
        dprint("\n  문 앞에 탈출구가 있다!\n", 0.8);
        g_exit_found = 1;       //탈출구 발견 -> 6번 메뉴 활성화
        if (g_pl.has_key)
        {
            dprint("  열쇠를 사용해 문을 열었다!\n", 1.0);
            delay(0.5);
            dprint("\n  ===== 비마관 탈출 성공! =====\n", 1.0);
            g_win = 1;
            g_pl.alive = 0;     //게임 클리어 처리(alive를 0으로 만들어서 게임 종료)
        }
        else 
        {
            dprint("  잠겨있다. 열쇠가 필요하다.\n", 0.8);
            dprint("  (탈출구 위치를 기억해두자. 열쇠를 찾아라)\n", 0.5);
            printf("  메뉴 6번으로 탈출구까지 경로를 확인할 수 있다.\n");
        }
        break;
    case CT_KEY:
        dprint("\n  반짝이는 열쇠가 있다!\n", 0.8);
        g_pl.has_key = 1;
        r->content = CT_EMPTY;      //열쇠는 1회성 아이템
        dprint("  열쇠를 획득했다!\n", 0.5);
        if (g_exit_found)
            printf("  탈출구를 이미 알고 있다. 바로 향하자!\n");
        break;
    case CT_BLOCKED:
        break;
    }

    if (g_pl.san <= 0 || g_pl.hp <= 0)
        g_pl.alive = 0;     //사망 처리
}

//플레이어 행동 함수
//방향 선택 후 이동 처리 - 이동 시 허기/스테미나 소모, 굶주림 피해 적용
void try_move(void)
{
    int dir, ny, nx;
    printf("\n  방향: 1.위쪽(북)   2.아래쪽(남)   3.왼쪽(서)   4.오른쪽(동)\n  선택: ");
    scanf("%d", &dir);
    while
        (getchar() != '\n');

    if (dir < 1 || dir > 4)
    {
        printf("  잘못된 방향입니다.\n");
        return;
    }
    ny = g_pl.y + g_dy[dir - 1];    //선택한 방향으로 좌표 계산
    nx = g_pl.x + g_dx[dir - 1];

    if (ny < 0 || ny >= MAP_N || nx < 0 || nx >= MAP_N)
    {
        dprint("  더 이상 갈 수 없다...\n", 0.5);
        return;
    }
    if (g_map[ny][nx].content == CT_BLOCKED) 
    {
        dprint("  문이 열리지 않는다...\n", 0.7);
        dprint("  아니면 무언가 필사적으로 막고 있는 것일까?\n", 0.5);
        g_map[ny][nx].revealed = 1;     //문을 열려고 했으니 공개는 함
        g_map[ny][nx].visited = 1;
        return;
    }

    g_pl.x = nx; 
    g_pl.y = ny;
    g_pl.turn++;
    g_pl.hun -= 2;      //이동할 때마다 허기 감소
    clamp(&g_pl.hun, 0, g_pl.hun_max);
    g_pl.sta -= 1;
    clamp(&g_pl.sta, 0, g_pl.sta_max);

    if (g_pl.hun <= 0)      //굶주림이 0이 되면 매 이동마다 체력 감소
    {
        dprint("  배고픔에 체력이 깎인다...", 0.5);
        printf("  [-5 HP]\n");
        g_pl.hp -= 5; 
        clamp(&g_pl.hp, 0, g_pl.hp_max);
    }
    update_vision();
    enter_room();           //이동 완료 후 해당 방 이벤트 처리
}

//휴식 - HP/정신력 회복, 허기 소모, 스테미나 완전 회복
//붉은 방에서는 휴식 불가 (너무 위험함)
void rest(void) 
{
    if (g_map[g_pl.y][g_pl.x].theme == TH_RED) 
    {
        dprint("  여기서는 쉴 수 없다. 너무 위험하다.\n", 0.5); 
        return;
    }
    g_pl.hp += 10;
    clamp(&g_pl.hp, 0, g_pl.hp_max);
    g_pl.san += 5; 
    clamp(&g_pl.san, 0, g_pl.san_max);
    g_pl.hun -= 5; 
    clamp(&g_pl.hun, 0, g_pl.hun_max);
    g_pl.sta = g_pl.sta_max;    //스테미나는 쉬면 완전 회복
    g_pl.turn++;
    dprint("  잠시 쉬었다.", 0.5);
    printf("  [HP +10, 정신력 +5, 허기 -5, 스테미나 회복]\n");
    if (g_map[g_pl.y][g_pl.x].theme == TH_POOL) 
    {
        delay(0.5);
        g_pl.hun += 10; 
        clamp(&g_pl.hun, 0, g_pl.hun_max);
        dprint("  화장실 수도물로 목을 축였다.", 0.5);
        printf("  [허기 +10 추가]\n");
    }
}

//현재 방 조사 - 방 테마 상세 설명 출력, 허기 소모
void investigate(void) 
{
    Room* r = &g_map[g_pl.y][g_pl.x];
    printf("\n  [공간 조사]\n  ");
    delay(0.3);
    switch (r->theme) 
    {
    case TH_YELLOW: 
        dprint("평범한 방 - 비마관의 일반적인 복도 공간.\n", 0.5);
        break;
    case TH_POOL:  
        dprint("화장실    - 물 떨어지는 소리가 끊이지 않는다.\n", 0.5);
        break;
    case TH_DARK:  
        dprint("암전 방   - 빛이 거의 없다. 어둠 속에 무언가 숨어있다.\n", 0.5);
        break;
    case TH_RED:   
        dprint("붉은 방   - 벽이 붉게 물들어있다. 오래된 것이다.\n", 0.5);
        break;
    }
    g_pl.turn++;
    g_pl.hun -= 1;
    clamp(&g_pl.hun, 0, g_pl.hun_max);
}

//플레이어 상태 초기화 함수
//맵 크기 결정 후 호출해야 함 (MAP_N/2(중심)로 시작 위치 계산)
void init_player(void) 
{
    int sy = MAP_N / 2, sx = MAP_N / 2;     //맵 한가운데가 시작 위치
    g_pl.x = sx;
    g_pl.y = sy;
    g_pl.hp = g_pl.hp_max = 100;
    g_pl.san = g_pl.san_max = 100;
    g_pl.hun = g_pl.hun_max = 100;
    g_pl.sta = g_pl.sta_max = 100;
    g_pl.atk = 10;      //기본 공격력 (치트 시 100으로 증가)
    g_pl.has_key = 0;   //처음엔 열쇠 없음
    g_pl.turn = 1;
    g_pl.alive = 1;
}

//맵 크기 입력 함수 (동적 할당 사용)
//5~20 사이 정수 입력 받아 MAP_N 설정 후 alloc_map 호출
void select_map_size(void)
{
    char buf[10];
    int n;
    system("cls");
    printf("\n  맵 크기를 입력하세요 (n x n, n: 5~20)\n");
    printf("  클수록 탐험이 길어집니다.\n\n");
    printf("  n = ");
    while (1)
    {
        fgets(buf, 10, stdin);
        trim(buf);
        n = atoi(buf);
        if (n >= 5 && n <= 20)   //범위 벗어나면 재입력 요청
            break;
        printf("  5~20 사이 숫자를 입력하세요: n = ");
    }
    MAP_N = n;
    alloc_map(MAP_N);           //입력받은 크기로 맵 동적 할당
    printf("  %dx%d 맵이 생성됩니다.\n", MAP_N, MAP_N);
    delay(0.5);
}

//시작 특전 선택 함수
//게임 시작 전 플레이어의 과거 생활 선택 -> 특정 스탯 +20
void select_background(void)
{
    int choice, sub;
    system("cls"); 
    delay(0.3);
    dprint("\n  당신은 대한민국 서울 노원구에 사는 평범한 대학생입니다.\n\n", 1.0);
    dprint("  당신은 평소에 무엇을 하며 시간을 보내나요?\n\n", 0.8);
    printf("  1. 게임을 하며 시간을 보낸다.\n");
    printf("  2. 운동을 하며 시간을 보낸다.\n");
    printf("  3. 숨을 쉬며 시간을 보낸다.\n\n");
    printf("  선택: ");
    scanf("%d", &choice); 
    while (getchar() != '\n');

    if (choice == 1) 
    {
        dprint("\n  게임을 하며 현실과 거리를 뒀다. 정신력이 강해졌다.\n", 0.8);
        g_pl.san = g_pl.san_max = 120;
        dprint("  [정신력 +20]\n", 0.5);
    }
    else if (choice == 2) 
    {
        printf("\n  1. 유산소 (스테미나 +20)   2. 무산소 (체력 +20)\n  선택: ");
        scanf("%d", &sub);
        while (getchar() != '\n');
        if (sub == 1) 
        {
            dprint("\n  꾸준한 유산소로 지구력이 향상됐다.\n", 0.8);
            g_pl.sta = g_pl.sta_max = 120;
            dprint("  [스테미나 +20]\n", 0.5);
        }
        else 
        {
            dprint("\n  무산소 운동으로 체력이 단련됐다.\n", 0.8);
            g_pl.hp = g_pl.hp_max = 120;
            dprint("  [체력 +20]\n", 0.5);
        }
    }
    else 
    {
        dprint("\n  ... 그냥 살아있었다. 밥은 잘 먹었다.\n", 0.8);
        g_pl.hun = g_pl.hun_max = 120;
        dprint("  [허기 +20]\n", 0.5);
    }
    delay(1.0);
}

//시작 인트로 스토리 출력 함수
void intro_story(void)
{
    system("cls"); printf("\n\n"); delay(0.5);
    dprint("  새빛관으로 향하던 당신은 갑작스러운 사건에 휘말렸다.\n", 1.5);
    dprint("  날개 달린 말이 당신에게 [몸통 박치기]를 시전했다.\n", 1.5);
    dprint("\n  .", 0.5);
    dprint(".", 0.5);
    dprint(".\n", 0.5);
    dprint("\n  효과는 굉장했다!\n", 1.0);
    dprint("\n  기절한 당신은 잠시 후 눈을 떴다.\n", 1.5);
    dprint("  큰일이다.\n", 1.0);
    dprint("\n  복잡하기로 소문난 비마관에 들어오다니...\n", 1.5);
    dprint("  당신은 이곳을 탈출하기 위해 앞으로 나아가야 한다는 것을 깨달았다.\n\n", 1.5);
    printf("  [Enter]를 눌러 계속...");
    getchar();
}

//타이틀 화면 + 게임 시작 / 나가기 메뉴
void main_menu(void) 
{
    int choice;
    system("cls");
    printf("\n\n");
    printf("  =========================\n");
    printf("        비   마   관\n");
    printf("  =========================\n\n");
    printf("  1. 게임 시작\n");
    printf("  2. 나가기\n\n");
    printf("  선택: ");
    scanf("%d", &choice);
    while (getchar() != '\n');
    if (choice == 2)
    { 
        dprint("\n  ...\n", 1.0);
        exit(0); 
    }
}

//메인 함수
int main(void)
{
    char buf[10];
    int  choice;

    srand((int)time(NULL)); //현재 시각을 시드로 사용 -> 실행마다 다른 랜덤 맵 생성

    main_menu();            //메인 메뉴 UI 출력
    select_map_size();      //맵 크기 입력
    init_player();          //플레이어 상태 초기화 함수
    select_background();    //시작 특전 선택
    intro_story();          //시작 인트로 스토리 출력

    system("cls");
    dprint("\n  맵 생성 중...\n", 0.5);
    generate_map();         //맵 생성
    load_enemies();         //enemies.txt에서 적 데이터 받아오기
    update_vision();        //시작 위치 주변 공개
    dprint("  준비 완료!\n", 0.5);
    delay(0.5);

    //메인 게임 루프 - 플레이어가 살아있는 동안 반복
    while (g_pl.alive)
    {
        print_hud();
        fgets(buf, 10, stdin);  //숫자나 M 키 입력
        trim(buf);

        if (buf[0] == 'M' || buf[0] == 'm') 
        { 
            print_fullmap();    //M 입력 시 전체 지도 표시
            continue;
        }
        if (buf[0] == '0')      //0 입력 시 게임 종료
            break;

        choice = atoi(buf);     //입력 문자열을 정수로 변환해 switch에 사용
        switch (choice) 
        {   
        case 1:
            try_move();         
            break;
        case 2: 
            investigate();  
            break;
        case 3: 
            rest();          
            break;
        case 4:
            printf("\n  [상세 상태]\n");
            printf("  위치: (%d,%d)  턴: %d  열쇠: %s\n",
                g_pl.x, g_pl.y, g_pl.turn,
                g_pl.has_key ? "보유" : "없음");
            printf("  체력     "); print_bar(g_pl.hp, g_pl.hp_max);
            printf("  정신력   "); print_bar(g_pl.san, g_pl.san_max);
            printf("  허기     "); print_bar(g_pl.hun, g_pl.hun_max);
            printf("  스테미나 "); print_bar(g_pl.sta, g_pl.sta_max);
            break;
        case 5:
            activate_cheat();    
            break;
        case 6:
            find_path_to_exit(); 
            break;
        default:
            printf("  잘못된 입력입니다.\n");
        }

        //게임 클리어 체크
        if (g_win) 
        {
            dprint("\n  ===== 비마관 탈출 성공! =====\n", 0.5);
            printf("  %d턴 만에 탈출했습니다!\n", g_pl.turn);
            break;
        }
        //게임 오버 체크 (체력 또는 정신력이 0)
        if (!g_pl.alive) 
        {
            delay(1.0);
            dprint("\n  ===== GAME OVER =====\n", 0.5);
            printf("  %d턴 동안 비마관을 탐사했습니다.\n", g_pl.turn);
            break;
        }
        //굶주림 사망 체크 (허기와 체력 동시에 0)
        if (g_pl.hun <= 0 && g_pl.hp <= 0) 
        {
            dprint("\n  ===== DEAD =====\n", 0.5);
            dprint("  굶주림에 쓰러졌다...\n", 1.0);
            break;
        }

        printf("\n  [Enter를 눌러 계속...]");
        getchar();
    }

    //게임 종료 - 동적 할당 메모리 해제
    free_map();
    if (g_enm) 
        free(g_enm);
    printf("\n  게임 종료. %d턴을 플레이했습니다.\n", g_pl.turn);
    return 0;
}
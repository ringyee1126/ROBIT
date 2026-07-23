#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

/*회문인지 아닌지 판별해주는 코드
스택에서 떠낸 문자와 큐에서 꺼낸 문자가 같은지를 기준으로 판별할 계획*/

int main()
{
	int i;
	int input_Len = 0;
	int top = -1;			//스택 : 데이터 입출력
	int front = 0;			//큐 : 선입선출이므로 꺼낼 위치가 맨 앞
	int rear = 0;			//큐 : 같은 이유로 넣을 위치는 맨 뒤
	char queue[100];		//ㄱ
	char stack[100];		//	ㄱ
	char input[100];		//입력받는 문자열의 크기는 넉넉하게 99자로 잡음
	printf("회문인지 아닌지 판별하기 위한 문자열을 입력하세요. : ");
	gets(input);		//새로 배운 fgets이 파일에 gets를 사용하는 것이라는 것을 활용하여 gets의 역할을 깨달았고, 바로 이를 활용하여 입력받는 문자열에 공백도 함께 입력받도록 설계함

	for (i = 0; input[i] != '\0'; i++)		//input의 길이를 널문자를 활용해서 찾음
	{
		input_Len++;
	}
	
	for (i = 0; i < input_Len; i++)
	{
		top++;			//top을 -1로 정의해서 먼저 ++를 해서 0으로 만든 이후 배열에 사용
		stack[top] = input[i];

		queue[rear] = input[i];
		rear++;
	}

	for (i = 0; i < input_Len; i++)
	{
		if (queue[front] != stack[top])
		{
			printf("회문이 아닙니다.");
			return 0;
		}
		front++;		//front은 처음에 0으로 정의한 이후 한번도 안사용되었기에 하나씩 키우면서 사용, queue에서는 선입선출이므로 맨 밑에서부터 하나씩 증가시키면서 확인
		top--;			//top은 위에서 사용된 변수인 만큼 커진 상태이므로 맨위에서부터 하나씩 낮추면서 확인
	}
	printf("회문이 맞습니다.");
	return 0;
}
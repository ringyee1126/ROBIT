#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

typedef struct _Studient
{
	int grade;
	float score;
	char name[10];		//과제1의 입출력 내용을 보면 이름이 9개를 넘는 상황은 없는 것으로 간주한다고 판단
}Studient;

int main()
{
	int i, t, k;
	Studient list[5];		//과제1에서 5명의 학생이 있다해서 리스트를 5개만 만듦
	Studient temp;			//정렬 과정에서 잠시 학생의 정보를 저장해놓기 위한 코드

	char inputName[100];
	int result;
	int nameLength;

	for (i = 0; i < 5; i++)
	{
		printf("%d번 학생의 학년, 성적, 이름을 입력하시오.(띄어쓰기로 구분) : ", i + 1);		//과제1의 예시 입력과 같이 학년, 성적, 이름을 한 번에 입력받도록 만듦
		result = scanf("%d %f %s", &list[i].grade, &list[i].score, inputName);		//이름은 임시 배열에 저장하여 길이를 검사하도록 한다

		//세 가지 입력이 모두 정상적으로 입력된 경우
		if (result != 3)
		{
			printf("ERROR : 학년은 정수, 성적은 실수, 이름은 문자열로 입력해야 합니다.\n");
			return 1;
		}
		//학년 검사
		if (list[i].grade <= 0)
		{
			printf("ERROR : 학년은 1 이상의 정수로 입력해야 합니다.\n");
			return 1;
		}

		//이름 길이 검사(9글자만 받을 수 있도록 가정하였기에 10글자가 넘으면 다시 입력해야함)
		nameLength = 0;
		while (inputName[nameLength] != '\0')
			nameLength++;

		if (nameLength > 9)
		{
			printf("ERROR : 이름은 9자를 초과할 수 없습니다.\n");
			return 1;
		}

		//길이가 9글자를 넘지 않는 이름을 배운 내용을 토대로 list[i].name에 직접 복사
		for (k = 0; k <= nameLength; k++)
			list[i].name[k] = inputName[k];
		printf("\n");
	}


	// 정렬
	for (i = 0; i < 4; i++)
	{
		for (t = i + 1; t < 5; t++)
		{
			//1순위 : 학년
			if (list[i].grade > list[t].grade)		//학년이 낮은 쪽이 더 먼저 출력되도록
			{
				temp = list[i];
				list[i] = list[t];
				list[t] = temp;
			}

			//2순위 : 성적 (학년이 같을 때)
			else if (list[i].grade == list[t].grade)
			{
				if (list[i].score > list[t].score)		//성적이 낮은 쪽이 더 먼저 출력되도록
				{
					temp = list[i];
					list[i] = list[t];
					list[t] = temp;
				}

				//3순위 : 이름 (학년과 성적이 같을 때)
				else if (list[i].score == list[t].score)
				{
					for (k = 0; list[i].name[k] != '\0'; k++)		//배우지 않은 내용을 사용하지 않고 list[i]속 name의 끝이 널문자임을 활용하여 문자열의 끝까지 분석
					{
						if (list[i].name[k] > list[t].name[k])			//배우지 않은 내용을 사용하지 않고 첫시간 때 배운 아스키 코드를 이용하여 첫글자만 비교 -> 알파벳이 먼저 오는 쪽이 먼저 출력되도록
						{
							temp = list[i];
							list[i] = list[t];
							list[t] = temp;
							break;
						}
						else if (list[i].name[k] < list[t].name[k])		//학생 이름 ooo와 oooo를 구분하면 ooo가 먼저 옴
							break;
					}
				}
			}
		}
	}
	//출력
	for (i = 0; i < 5; i++)
		printf("%d %.1f %s\n", list[i].grade, list[i].score, list[i].name);
	return 0;
}
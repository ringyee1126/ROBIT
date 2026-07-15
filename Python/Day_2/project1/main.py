import os
os.chdir(os.path.dirname(__file__))    #현재 파이썬 파일이 있는 폴더를 작업 경로로 설정
import csv
import json

with open('studients.csv', 'r', newline="", encoding='utf-8') as file:
    reader = csv.reader(file)
    header = next(reader)
    students = []
    score_list = []      #json으로 옮길 때 편하게 하기 위해 점수만 따로 저장
    with open("clean_studients.csv", "w", newline="", encoding="utf-8") as file:          #16행에서 w를 사용시 마지막 학생만 저장 되어서 a를 사용하기로 함 -> 코드를 실행할 때마다 계속 결과값이 쌓이는 것을 방지하고자 9행에서 이를 초기화하는 명령어를 추가함
        writer = csv.writer(file)
    for row in reader:
        try:
            score = int(row[1])
            if 0 <= score <= 100:
                students.append((row[0], score))
                with open("clean_studients.csv", "a", newline="", encoding="utf-8") as file:
                    writer = csv.writer(file)
                    writer.writerow([row[0], score])
                score_list.append(score)
            else:
                score = "허용 범위 초과"
                students.append(score)          #누가 오류가 떴는지를 표시하라는 말은 없고 오류 '행'이라 써있어서 행 자체를 오류로 보는 것으로 이해함
                continue
        except ValueError:
            score = "숫자 변환 실패"
            students.append(score)
            continue

count = len(score_list)
highest = max(score_list)
average = sum(score_list)/len(score_list)

summary = {"인원수":count, "평균":average, "최고점":highest}

with open("summary.json", "w", newline="", encoding="utf-8") as file:
    json.dump(summary, file, ensure_ascii=False, indent=2)


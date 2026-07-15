import os
os.chdir(os.path.dirname(__file__))    #현재 파이썬 파일이 있는 폴더를 작업 경로로 설정
import csv
import json

with open('studients.csv', 'r', newline="", encoding='utf-8') as file:
    dictreader = csv.DictReader(file)       #reader -> DictReader
    students = []
    score_list = []
    with open("clean_studients.csv", "w", newline="", encoding="utf-8") as file:
        dictwriter = csv.DictWriter(file, fieldnames=["name", "score"])     #writer -> DictWriter +  fieldnames로 열 순서를 정하기
        dictwriter.writeheader()    #writeheader()로 첫 행에 열 이름 쓰기
        #추가 구현 전에는 header로 건너뛰었지만 dict를 사용하는 추가 구현에서는 2일차 수업 자료 속에 있는 내용을 직접 복습할 겸 사용해 보았다    
    for row in dictreader:
        try:
            score = int(row["score"])
            if 0 <= score <= 100:
                students.append((row["name"], score))
                with open("clean_studients.csv", "a", newline="", encoding="utf-8") as file:
                    dictwriter = csv.DictWriter(file, fieldnames=["name", "score"])     #writer -> DictWriter +  fieldnames로 열 순서를 정하기
                    dictwriter.writerow({"name": row["name"],"score": score})
                score_list.append(score)
            else:
                score = "허용 범위 초과"
                students.append(score)
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


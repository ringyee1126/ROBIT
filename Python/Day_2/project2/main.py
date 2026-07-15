import os
os.chdir(os.path.dirname(__file__))    #현재 파이썬 파일이 있는 폴더를 작업 경로로 설정
import numpy as np
import csv
import json

with open('studients.csv', 'r', newline="", encoding='utf-8') as file:
    reader = csv.reader(file)
    header = next(reader)
    score_list = []
    for row in reader:
        try:
            score = int(row[1])
            if 0 <= score <= 100:
                score_list.append(score)
            else:
                continue
        except ValueError:
            continue

scores = np.array(score_list)

result = {"인원수":len(score_list), "평균":float(np.average(scores)), "최고값":int(np.max(scores)), "최저값":int(np.min(scores)), "표준편차":float(np.std(scores))}
with open("result.json", "w", newline="", encoding="utf-8") as file:
    json.dump(result, file, ensure_ascii=False, indent=2)


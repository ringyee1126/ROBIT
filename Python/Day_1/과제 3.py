list = []

while True:             #명령을 어디까지 받으라는 말이 없어서 while True로 무한 반복하게 했다.
    command = input().split()
    if command[0] == "append":
        if len(command) < 2:
            print("값을 입력해주시오.")
            continue
        list.append(command[1])

    elif command[0] == "remove":
        if len(command) < 2:
            print("값을 입력해주시오.")
            continue
        elif command[1] not in list:
            print(f"{command[1]}은(는) 리스트에 존재하지 않습니다.")
            continue
        list.remove(command[1])

    elif command[0] == "pop":
        try:                            #인덱스 값을 제대로 입력받았는지 확인하는 조건문(정수인지)을 추가했다.
            int(command[1])
        except ValueError:
            print("인덱스는 정수여야 합니다.")
            continue
        if len(command) == 0:
            print("리스트가 비어 있으므로 pop할 수 없습니다.")
            continue
        elif len(command) < 2:
            print("인덱스를 입력해주시오.")
            continue
        elif int(command[1]) >= len(list):
            print("리스트의 길이보다 큰 인덱스입니다.")
            continue
        list.pop(int(command[1]))

    elif command[0] == "insert":
        try:
            int(command[1])
        except ValueError:
            print("인덱스는 정수여야 합니다.")
            continue
        if len(command) < 2:
            print("인덱스를 입력해주시오.")
            continue
        elif len(command) < 3:
            print("값을 입력해주시오.")
            continue
        elif int(command[1]) >= len(list) +1:
            print(f"{command[1]}보다 작은 인덱스의 값이 비어 있습니다.")
            continue
        list.insert(int(command[1]), command[2])

    elif command[0] == "clear":
        list.clear()

    elif command[0] == "len":
        print(len(list))

    elif command[0] == "print":
        print(list)

    elif command[0] == "exit":          #종료 조건은 exit로 임의로 지정했다.
        break

    else:                               #위 조건 속 명령어와 하나도 일치하는 것이 없을 시 오류 대신 이 문구를 띄우고 다시 처음으로 돌아간다(continue).
        print("잘못된 명령어입니다.")
        continue
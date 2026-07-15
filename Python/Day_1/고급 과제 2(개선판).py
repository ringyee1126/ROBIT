books = {}      # {책이름: 남은 수량}
users = {}      # {사용자: set(대출중인 책)}
book_order = [] # 등록 순서 저장
E = "ERROR"
O = "OK"


def add_book(name, amount):         #책 등록 함수
    if amount < 0:
        print(E + ": 수량은 0 이상이어야 합니다.")
        return

    if name in books:
        books[name] += amount
    else:
        books[name] = amount
        book_order.append(name)

    print(O)


def borrow_book(user, book):        #책 대출 함수

    if book not in books:
        print(E + ": 존재하지 않는 책입니다.")
        return

    if books[book] <= 0:
        print(E + ": 재고가 없습니다.")
        return

    if user not in users:
        users[user] = set()

    if book in users[user]:
        print(E + ": 이미 대출한 책입니다.")
        return

    users[user].add(book)
    books[book] -= 1
    print(O)


def return_book(user, book):        #책 반납 함수

    if user not in users:
        print(E + ": 대출 기록이 없습니다.")
        return

    if book not in users[user]:
        print(E + ": 해당 책을 대출하지 않았습니다.")
        return

    users[user].remove(book)
    books[book] += 1

    if len(users[user]) == 0:
        del users[user]

    print(O)


def status_book(book):          #책 재고 확인 함수

    if book not in books:
        print(E)
        return

    print(books[book])


def user_info(user):            #사용자 대출 정보 확인 함수

    if user not in users or len(users[user]) == 0:
        print("EMPTY")
        return

    print(*sorted(users[user]))


def list_books():               #책 목록 확인 함수


    for book in book_order:
        print(book, books[book])


def process(command):           #명령어 처리 함수

    parts = command.split()

    if not parts:
        print(E)
        return True

    cmd = parts[0]

    # --------------------
    if cmd == "add":

        if len(parts) != 3:
            print(E)
            return True

        book = parts[1]

        try:
            amount = int(parts[2])
        except:
            print(E)
            return True

        add_book(book, amount)

    # --------------------
    elif cmd == "borrow":

        if len(parts) != 3:
            print(E)
            return True

        borrow_book(parts[1], parts[2])

    # --------------------
    elif cmd == "return":

        if len(parts) != 3:
            print(E)
            return True

        return_book(parts[1], parts[2])

    # --------------------
    elif cmd == "status":

        if len(parts) != 2:
            print(E)
            return True

        status_book(parts[1])

    # --------------------
    elif cmd == "user":

        if len(parts) != 2:
            print(E)
            return True

        user_info(parts[1])

    # --------------------
    elif cmd == "list":

        if len(parts) != 1:
            print(E)
            return True

        list_books()

    # --------------------
    elif cmd == "Exit":

        return False

    else:
        print(E)

    return True


def main():             #명령어 입력 처리 루프(main 함수)

    while True:
        command = input()

        if not process(command):
            break


if __name__ == "__main__":
    main()
# ===============================
# 명령어 기반 도서 대출 관리 시스템
# 모든 기능을 함수로 분리
# eval() 사용하지 않음
# ===============================

books = {}      # {책이름: 남은 수량}
users = {}      # {사용자: set(대출중인 책)}
book_order = [] # 등록 순서 저장


def add_book(name, amount):
    if amount < 0:
        print("ERROR: 수량은 0 이상이어야 합니다.")
        return

    if name in books:
        books[name] += amount
    else:
        books[name] = amount
        book_order.append(name)

    print("OK")


def borrow_book(user, book):

    if book not in books:
        print("ERROR: 존재하지 않는 책입니다.")
        return

    if books[book] <= 0:
        print("ERROR: 재고가 없습니다.")
        return

    if user not in users:
        users[user] = set()

    if book in users[user]:
        print("ERROR: 이미 대출한 책입니다.")
        return

    users[user].add(book)
    books[book] -= 1
    print("OK")


def return_book(user, book):

    if user not in users:
        print("ERROR: 대출 기록이 없습니다.")
        return

    if book not in users[user]:
        print("ERROR: 해당 책을 대출하지 않았습니다.")
        return

    users[user].remove(book)
    books[book] += 1

    if len(users[user]) == 0:
        del users[user]

    print("OK")


def status_book(book):

    if book not in books:
        print("ERROR")
        return

    print(books[book])


def user_info(user):

    if user not in users or len(users[user]) == 0:
        print("EMPTY")
        return

    print(*sorted(users[user]))


def list_books():

    for book in book_order:
        print(book, books[book])


def process(command):

    parts = command.split()

    if not parts:
        print("ERROR")
        return True

    cmd = parts[0]

    # --------------------
    if cmd == "add":

        if len(parts) != 3:
            print("ERROR")
            return True

        book = parts[1]

        try:
            amount = int(parts[2])
        except:
            print("ERROR")
            return True

        add_book(book, amount)

    # --------------------
    elif cmd == "borrow":

        if len(parts) != 3:
            print("ERROR")
            return True

        borrow_book(parts[1], parts[2])

    # --------------------
    elif cmd == "return":

        if len(parts) != 3:
            print("ERROR")
            return True

        return_book(parts[1], parts[2])

    # --------------------
    elif cmd == "status":

        if len(parts) != 2:
            print("ERROR")
            return True

        status_book(parts[1])

    # --------------------
    elif cmd == "user":

        if len(parts) != 2:
            print("ERROR")
            return True

        user_info(parts[1])

    # --------------------
    elif cmd == "list":

        if len(parts) != 1:
            print("ERROR")
            return True

        list_books()

    # --------------------
    elif cmd == "Exit":

        return False

    else:
        print("ERROR")

    return True


def main():

    while True:
        command = input()

        if not process(command):
            break


if __name__ == "__main__":
    main()
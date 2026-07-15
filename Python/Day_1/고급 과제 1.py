def compress(text):

    count = 1
    result = ""

    for i in range(len(text)-1):

        if text[i] == text[i+1]:
            count += 1

        else:
            result += text[i] + str(count)
            count = 1

    result += text[-1] + str(count)

    return result

def decompress(code):
    if is_valid_code(code) is False:
        return "ERROR"
    i = 0

    result = ""

    while i < len(code):

        letter = code[i]
        i += 1

        number = ""

        while i < len(code) and code[i].isdigit():
            number += code[i]
            i += 1

        result += letter * int(number)
    return result

def is_valid_code(code):

    if len(code) == 0:
        return False

    i = 0

    while i < len(code):

        if not code[i].isalpha():        # 1. 현재 문자가 알파벳인지 확인
            return False

        i += 1

        if i >= len(code) or not code[i].isdigit():       # 2. 문자 뒤에 숫자가 하나 이상 있는지 확인
            return False

        number = ""

        while i < len(code) and code[i].isdigit():        # 3. 숫자를 끝까지 읽기
            number += code[i]
            i += 1

        if int(number) < 1:                               # 4. 숫자가 1 이상인지 확인
            return False

    return True


print(compress("aaabbccccd"))
print(decompress("a3b2c4d1"))
print(decompress("a12b3"))
print(decompress("a0"))
print(decompress("3a"))
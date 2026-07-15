def add_log(robot_name, battery, logs=[]):            #로그가 의도치 않게 중첩되는 이유는 리스트(logs)를 참조했기 때문이므로, 이를 리스트를 복사하는 방법으로 수정했다.
    logs = logs.copy()
    log = robot_name + " battery: " + str(battery)      #기존의 log는 robot_name + " battery: " + battery에서 battery는 정수(int)이므로 문자열과 바로 연결할 수 없다.
    logs.append(log)                                    #battery의 형태는 정수형 이므로 str()로 변환해주어서 robot_name(문자열)과 같은 형태가 되어야 함
    return logs

print(add_log("frontbot", 80))
print(add_log("rearbot", 50))
print(add_log("armbot", 20))
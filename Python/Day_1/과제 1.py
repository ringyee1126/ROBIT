robot_status = [
    {"name": "mobilebot", "battery": 82, "position": (1.2, 0.5), "distance": 0.8},
    {"name": "drone", "battery": 18, "position": (0.3, 1.5), "distance": 0.4},
    {"name": "manipulator", "battery": 45, "position": (2.0, 1.0), "distance": 1.2},
]

for robot in robot_status:
    name = robot["name"]
    battery = robot["battery"]
    position = robot["position"]
    distance = robot["distance"]

    def robot_battery(battery):     #배터리 상태 확인 함수
        if battery >= 60:
            return ("배터리 충분")
        elif 20<=battery<60:
            return ("배터리 주의")
        else:
            return ("충전 필요")

    x,y = position                  #로봇 위치 튜플 언패킹

    def robot_distance(distance):   #장애물 감지 함수
        if distance < 0.5:
            return ("장애물 감지")
        else:
            return ("전진 가능")

    print(f"[{name}]")
    print(f"배터리 :",robot_battery(battery))
    print(f"위치 : x={x}, y={y}")
    print(f"상태 : ",robot_distance(distance))
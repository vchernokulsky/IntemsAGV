#!/usr/bin/python
import requests
import json
import sys
import socket


IP = '192.168.22.177'
PORT = 23
BUFFER_SIZE = 1024

STOP = 0
FORWARD = 1
BACKWARD = 2

def get_direction(value):
    global STOP
    global FORWARD
    global BACKWARD
    if value < 0:
        return BACKWARD
    if value > 0:
        return FORWARD
    return STOP


def send_speeds(left_speed, right_speed):
    data_str = str(get_direction(left_speed)) + ";" + str(abs(left_speed)) + ";" + str(get_direction(right_speed)) + ";" + str(abs(right_speed))
    print(data_str)
    data = bytearray([get_direction(left_speed) ,abs(left_speed),get_direction(right_speed),abs(right_speed)])
    print (data)
    print(len(data))
    try:
        s=socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        print("socket set")
        s.connect((IP, PORT))
        print("socket connected")
        s.sendall(data)
        print("data sent")
        #ans = s.recv(BUFFER_SIZE)
        #rospy.loginfo('Received', repr(ans))
    except Exception as e:
        print("!!!")
        print(e)


if __name__ == '__main__':
    if len(sys.argv) == 3:
        send_speeds(int(sys.argv[1]), int(sys.argv[2]))
    elif len(sys.argv) == 2:
        send_speeds(int(sys.argv[1]), 0)
    else:
        send_speeds(0,0)


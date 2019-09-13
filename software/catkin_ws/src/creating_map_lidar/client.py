import socket
import cv2
import numpy as np

HOST = '192.168.55.100'
PORT = 5000

print('connection is accepted')

while True:
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((HOST, PORT))

    img_len = int(s.recv(1024))
    total_data = b''
    received_data_len = 0

    while received_data_len < img_len:
        data = s.recv(1024)
        received_data_len += len(data)
        total_data += data

    data_in_bytes = np.fromstring(total_data, np.uint8)
    frame = cv2.imdecode(data_in_bytes, cv2.IMREAD_COLOR)
    cv2.imshow('frame',frame)
    cv2.waitKey(1)

s.close()
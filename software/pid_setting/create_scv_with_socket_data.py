import csv
import socket
import robot_controlling.socket_client as st_cl
import json
import time

IP_motor = '192.168.22.177'
IP_encoder = '192.168.22.178'
PORT = 23
delay_after_pushing_data = 5
delay_to_get_speed = 2
start = 25
end = 255
step = 5
path_to_dataset_l = 'datasets/dataset_l.csv'
path_to_dataset_r = 'datasets/dataset_r.csv'

dataset_l = []
dataset_r = []

for x in range(start, end + step, step):
    st_cl.send_speeds(x, x, IP_motor)

    time.sleep(delay_to_get_speed)

    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((IP_encoder, PORT))
    json_dict= json.loads(s.recv(1024).decode('utf-8'))

    dataset_l += [[x, json_dict['l_vel']]]
    dataset_r += [[x, json_dict['r_vel']]]

    time.sleep(delay_after_pushing_data)

st_cl.send_speeds(0, 0, IP_motor)

with open(path_to_dataset_l, 'w') as f:
    writer = csv.writer(f)

    for data in dataset_l:
        writer.writerow(data)

with open(path_to_dataset_r, 'w') as f:
    writer = csv.writer(f)

    for data in dataset_r:
        writer.writerow(data)
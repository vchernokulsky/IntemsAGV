import robot_controlling.socket_client as st_cl
import socket
import json
import csv
import time

k_l = 0.1937502312673451
k_r = 0.18875208140610553


def left_wheel_speed_control(w):
    speed = int(w / k_l)
    return 255 if speed > 255 else speed


def right_wheel_speed_control(w):
    if w < 4:
        return w / 0.5

    speed = int(w / k_r)
    return 255 if speed > 255 else speed


def write_dataset_csv(dataset, path_to_dataset):
    with open(path_to_dataset, 'w') as f:
        writer = csv.writer(f)

        for data in dataset:
            writer.writerow(data)


IP_motor = '192.168.22.177'
IP_encoder = '192.168.22.178'
PORT = 23
delay = 5
start = 10
end = 50
step = 4
path_to_dataset_l = 'datasets/dataset_l_test.csv'
path_to_dataset_r = 'datasets/dataset_r_test.csv'

dataset_l = []
dataset_r = []

for w in range(start, end + step, step):
    U_l = left_wheel_speed_control(w)
    U_r = right_wheel_speed_control(w)

    st_cl.send_speeds(U_l, U_r, IP_motor)

    time.sleep(2)
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((IP_encoder, PORT))
    json_dict = json.loads(s.recv(1024).decode('utf-8'))

    dataset_l += [[json_dict['l_vel'], w]]
    dataset_r += [[json_dict['r_vel'], w]]

    time.sleep(delay)

st_cl.send_speeds(0, 0, IP_motor)

write_dataset_csv(dataset_l, path_to_dataset_l)
write_dataset_csv(dataset_r, path_to_dataset_r)

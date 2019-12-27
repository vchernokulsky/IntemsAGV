from SLMP.slmp_binary import *
import socket
from scapy.all import *
import matplotlib.pyplot as plt
import time
import statistics

HOST = '192.168.88.2'
PORT = 5002  # 5002 if 192.168.55.100
REGISTER = 1024
SLMPBinary.value_type = 'dump'


def do_read_session(no_of_device_points):
    COMMAND = READ_COMMAND

    req = SLMPBinary.prepare_request(REGISTER, MEM['binary']['D'], COMMAND, no_of_device_points=no_of_device_points)
    # print(req.show())
    # print('\nsent: ', bytes(req))
    start = time.time()
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s.sendto(bytes(req), (HOST, PORT))
    data, address = s.recvfrom(1024)
    end = time.time() - start

    # res = SLMPBinary.form_response(data)
    # print(res.show())
    return end


def do_write_session(no_of_device_points):
    COMMAND = WRITE_COMMAND

    dump = b''.join([b'\xff\xff'] * no_of_device_points)
    req = SLMPBinary.prepare_request(REGISTER, MEM['binary']['D'], COMMAND, value=dump)
    # print(req.show())
    #print('\nsent: ', bytes(req))
    start = time.time()
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s.sendto(bytes(req), (HOST, PORT))
    data, address = s.recvfrom(1024)
    end = time.time() - start

    #res = SLMPBinary.form_response(data)
    #print(res.show())
    return end


if __name__ == '__main__':
    max_no_of_device_points = 960
    num_of_experiments_per_n = 50
    ns = [i for i in range(1, max_no_of_device_points + 1)]
    ts_median = []

    for n in ns:
        t = []
        for _ in range(num_of_experiments_per_n):
            t += [do_write_session(n)]
        ts_median += [statistics.median(t)]
        print(f'\rprogress is {n}/{max_no_of_device_points}', end='')

    plt.figure(figsize=(20, 5))
    plt.plot(ns, ts_median, color='red')
    plt.legend(['median time'])
    plt.xticks([i * 10 for i in range(0, 100, 5)])
    plt.xlabel('no. of device points')
    plt.ylabel('t, sec')
    plt.savefig('analytics/write.png')
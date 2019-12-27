from SLMP.slmp_binary import *
import socket
from scapy.all import *
import matplotlib.pyplot as plt
import time
HOST = '192.168.88.2'
PORT = 5002  # 5002 if 192.168.55.100
REGISTER = 900
SLMPBinary.value_type = 'dump'
VALUE = 67
COMMAND = READ_COMMAND

if __name__ == '__main__':
    req = SLMPBinary.prepare_request(REGISTER, MEM['binary']['D'], COMMAND, no_of_device_points=5)
    print(req.show())
    print('\nsent: ', bytes(req))

    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s.sendto(bytes(req), (HOST, PORT))
    data, address = s.recvfrom(1024)

    print('got: ', data, '\n')

    res = SLMPBinary.form_response(data)
    print(res.show())
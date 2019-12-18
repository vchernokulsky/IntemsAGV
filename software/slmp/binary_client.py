from SLMP.slmp_binary import *
import socket
from scapy.all import *

HOST = '192.168.88.2'
PORT = 5002  # 5002 if 192.168.55.100
REGISTER = 1000
VALUE_TYPE = 'sw'
VALUE = 67
COMMAND = WRITE_COMMAND

if __name__ == '__main__':
    SLMPBinary.value_type = VALUE_TYPE
    req = SLMPBinary.prepare_request(REGISTER, MEM['binary']['D'], COMMAND, no_of_device_points=4)
    print(req.show())
    print('\nsent: ', bytes(req))

    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s.sendto(bytes(req), (HOST, PORT))
    data, address = s.recvfrom(1024)

    print('got: ', data, '\n')

    res = SLMPBinary.form_response(data)
    print(res.show())
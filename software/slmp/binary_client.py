from SLMP.slmp_binary import *
import socket
from scapy.all import *

HOST = '192.168.88.2'
PORT = 5002  # 5002 if 192.168.55.100
REGISTER = 1024
VALUE_TYPE = 'unsigned double word'
VALUE = 19
COMMAND = WRITE_COMMAND

if __name__ == '__main__':
    req = SLMPBinaryRequest()
    req.initialize_request(register=REGISTER, device_code=D_MEM['binary'],  value=VALUE, value_type=VALUE_TYPE, command=COMMAND)
    print(req.show())
    print('\nsent: ', bytes(req))

    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s.sendto(bytes(req), (HOST, PORT))
    data, address = s.recvfrom(1024)

    print('got: ', data, '\n')

    res = SLMPBinaryResponse(data, VALUE_TYPE)
    print(res.show())
    print(res.Value)
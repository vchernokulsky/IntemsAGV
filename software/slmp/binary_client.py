from SLMP.slmp_binary import *
import socket
from scapy.all import *

HOST = '192.168.88.2'
PORT = 5002  # 5002 if 192.168.55.100
REGISTER = 104


req = SLMPBinaryRequest()
req.initialize_request(register=REGISTER, value=2.6, value_type='float', command=WRITE_COMMAND)

print(req.show())
print('\nsent: ', bytes(req))

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s.sendto(bytes(req), (HOST, PORT))
data, address = s.recvfrom(1024)

print('got: ', data, '\n')

"""res = SLMPBinaryResponse(data)
print(res.show())

print(res.decode_value_field_from_le().decode())"""

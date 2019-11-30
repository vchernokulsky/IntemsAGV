from SLMP.slmp_binary import *
import socket
from scapy.all import *

HOST = '192.168.88.2'
PORT = 5002  # 5002 if 192.168.55.100
REGISTER = 100


req = SLMPBinaryRequest()
req.read(register=REGISTER, no_of_device_points=9)
#req.write(REGISTER, 0x416c6f68612066726f6d2044656e69732100)

print(req.show())
print('\nsent: ', bytes(req))

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s.sendto(bytes(req), (HOST, PORT))
data, address = s.recvfrom(1024)

print('got: ', data, '\n')

res = SLMPBinaryResponse(data)
print(res.show())

print(res.decode_value_field_from_le())

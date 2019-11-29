#!/usr/bin/env python2
from SLMP.slmp_ascii import *
import socket
from scapy.all import *

HOST = '192.168.88.2'
PORT = 5003  # 5002 if 192.168.55.100
REGISTER = 0x100


req = SLMPASCIIRequest()
#req.write(register=REGISTER, value='cafecafecafecafe')
req.read(register=REGISTER, no_of_device_points=8)

print(req.show())
print('\nsent: ', bytes(req))

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s.sendto(bytes(req), (HOST, PORT))
data, address = s.recvfrom(1024)

print('got: ', data, '\n')

res = SLMPASCIIResponse(data)
print(res.show())

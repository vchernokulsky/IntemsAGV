#!/usr/bin/env python2
from SLMP.SLMP import *
import socket
from scapy.all import *

HOST = '192.168.88.2'
PORT = 5002
REGISTER = 0xb57#2902 - 8000, 2903 - 10000, 2904 - 16000


req = SLMPASCIIRequest()
req.read(REGISTER)

print(req.show())

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s.sendto(bytes(req), (HOST, PORT))

data, address = s.recvfrom(1024)

res = SLMPASCIIResponse(data)
print(res.show())

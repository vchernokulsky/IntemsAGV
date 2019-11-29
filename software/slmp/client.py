#!/usr/bin/env python2
from SLMP.slmp_ascii import *
import socket
from scapy.all import *

HOST = '192.168.88.2'
PORT = 5002
REGISTER = 0x11


req = SLMPASCIIRequest()
req.write(REGISTER, 1, no_of_device_points=4)
print(req.show())
print(bytes(req))

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

s.sendto(bytes(req), (HOST, PORT))

data, address = s.recvfrom(1024)
print(data)

res = SLMPASCIIResponse(data)
print(res.show())

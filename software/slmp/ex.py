from SLMP.utilities import *
from scapy.all import *

data = [IntField('i', 0), IntField('i2', 2)]


class B(Packet):
    fields_desc = [
        StrField('sb', 'a'),
        *data
    ]


class A(Packet):
    fields_desc = [
        StrField('sa', 'b'),
        *data
    ]

b = B()
a = A()

b.i2 = 2
a.i2 = 1

print(a.show())
print(b.show())

b.i2 = 2
a.i2 = 31

print(a.show())
print(b.show())

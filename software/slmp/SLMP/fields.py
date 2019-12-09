from scapy.all import *


class LEFloatField(Field):
    def __init__(self, name, default):
        Field.__init__(self, name, default, '< f')


class LESignedShortField(Field):
    def __init__(self, name, default):
        Field.__init__(self, name, default, '< h')
from scapy.all import *


class LEFloatField(Field):
    def __init__(self, name, default):
        Field.__init__(self, name, default, '< f')


class LESignedShortField(Field):
    def __init__(self, name, default):
        Field.__init__(self, name, default, '< h')


class BooleanField(Field):
    def __init__(self, name, default):
        if not isinstance(default, bool):
            raise Exception('default value is not boolean')
        Field.__init__(self, name, default, '< h')

    def m2i(self, pkt, x):
        return bool(x)


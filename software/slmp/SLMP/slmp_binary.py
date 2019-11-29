from scapy.all import *

from .constants import *
from .exceptions import *

binary_common_block = [
    XShortField('isSerialNo', 0x5000),
    ConditionalField(ShortField('SerialNo', 0), lambda pkt: pkt.isSerialNo == IS_SERIAL_NO),
    ConditionalField(ShortField('ThirdDuoOfSubheader', 0), lambda pkt: pkt.isSerialNo == IS_SERIAL_NO),
    ByteField('ReqDestNetNo', 0),
    XByteField('ReqDestStationNo', 0),
    ShortField('ReqProcessor', 0),
    ByteField('ReqReserved', 0),
    LEShortField('DataLength', 0)
]


class SLMPBinaryRequest(Packet):
    name = 'SLMP'

    fields_desc = [
        *binary_common_block,
        ShortField('MonitoringTimer', 1),
        # Request data starts
        LEShortField('Command', 0),
        ShortField('SubCommand', 0),
        LEThreeBytesField('HeadDeviceNo', 0x8),
        ByteField('DeviceCode', 0xA8),
        ShortField('NoOfDevicePoints', 0x0100),
        ConditionalField(LEShortField('Value', 0), lambda pkt: pkt.Command == WRITE_COMMAND)
        # Request data ends
    ]

    def read(self, register):
        self.ReqDataLength = 12
        self.Command = READ_COMMAND
        self.HeadDeviceNo = register

    def write(self, register, value):
        self.ReqDataLength = 14
        self.Command = WRITE_COMMAND
        self.HeadDeviceNo = register
        self.Value = value

    def add_serial_no(self, serial_no):
        self.isSerialNo = IS_SERIAL_NO
        self.SerialNo = serial_no


class SLMPBinaryResponse(Packet):
    name = 'SLMP'

    def __init__(self, data_in_bytes):
        Packet.__init__(self, data_in_bytes)

        if self.EndCode > 0x4000:
            raise PLCException(self.EndCode)

    fields_desc = [
        *binary_common_block,
        LEShortField('EndCode', 0),
        # if success:
        ConditionalField(LEShortField('Value', 0), lambda pkt: pkt.EndCode == 0),
        # else:
        ConditionalField(ByteField('RespNetNo', 0), lambda pkt: pkt.EndCode != 0),
        ConditionalField(ByteField('RespStationNo', 0), lambda pkt: pkt.EndCode != 0),
        ConditionalField(XShortField('RespProcessor', 0), lambda pkt: pkt.EndCode != 0),
        ConditionalField(ByteField('RespReserved', 0), lambda pkt: pkt.EndCode != 0),
        ConditionalField(LEShortField('Command', 0), lambda pkt: pkt.EndCode != 0),
        ConditionalField(LEShortField('SubCommand', 0), lambda pkt: pkt.EndCode != 0)
    ]
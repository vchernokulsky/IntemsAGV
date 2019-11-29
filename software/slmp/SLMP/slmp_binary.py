from scapy.all import *

from .constants import *
from .exceptions import *

binary_common_block = [
    XShortField('isSerialNo', SERIAL_NO_FALSE),
    ConditionalField(ShortField('SerialNo', 0), lambda pkt: pkt.isSerialNo == SERIAL_NO_TRUE),
    ConditionalField(ShortField('FieldIfSerialNo', 0), lambda pkt: pkt.isSerialNo == SERIAL_NO_TRUE),
    ByteField('ReqDestNetNo', 0),
    XByteField('ReqDestStationNo', DEFAULT_STATION_NO),
    ShortField('ReqProcessor', DEFAULT_PROCESSOR),
    ByteField('ReqReserved', 0),
    LEShortField('DataLength', 0)
]


class SLMPBinaryRequest(Packet):
    name = 'SLMP'

    fields_desc = [
        *binary_common_block,
        ShortField('MonitoringTimer', 1),
        LEShortField('Command', 0),
        ShortField('SubCommand', 0),
        LEThreeBytesField('HeadDeviceNo', 0x8),
        ByteField('DeviceCode', D_MEM['binary']),
        ShortField('NoOfDevicePoints', 0x0100),
        ConditionalField(LEShortField('Value', 0), lambda pkt: pkt.Command == WRITE_COMMAND)
    ]

    def read(self, register):
        self.DataLength = 12
        self.Command = READ_COMMAND
        self.HeadDeviceNo = register

    def write(self, register, value):
        self.DataLength = 14
        self.Command = WRITE_COMMAND
        self.HeadDeviceNo = register
        self.Value = value

    def add_serial_no(self, serial_no):
        self.isSerialNo = SERIAL_NO_TRUE
        self.SerialNo = serial_no


class SLMPBinaryResponse(Packet):
    name = 'SLMP'

    def __init__(self, data_in_bytes):
        Packet.__init__(self, data_in_bytes)

        if self.EndCode > END_CODE_THRESHOLD:
            raise PLCException(self.EndCode)

    fields_desc = [
        *binary_common_block,
        LEShortField('EndCode', 0),
        ConditionalField(LEShortField('Value', 0), lambda pkt: pkt.EndCode == 0),
        ConditionalField(ByteField('RespNetNo', 0), lambda pkt: pkt.EndCode != 0),
        ConditionalField(ByteField('RespStationNo', 0), lambda pkt: pkt.EndCode != 0),
        ConditionalField(XShortField('RespProcessor', 0), lambda pkt: pkt.EndCode != 0),
        ConditionalField(ByteField('RespReserved', 0), lambda pkt: pkt.EndCode != 0),
        ConditionalField(LEShortField('Command', 0), lambda pkt: pkt.EndCode != 0),
        ConditionalField(LEShortField('SubCommand', 0), lambda pkt: pkt.EndCode != 0)
    ]
from scapy.all import *

from .constants import *
from .exceptions import *
from .utilities import *

binary_common_block = [
    XShortField('isSerialNo', SERIAL_NO_FALSE),
    ConditionalField(XShortField('SerialNo', 0), lambda pkt: pkt.isSerialNo == SERIAL_NO_TRUE),
    ConditionalField(XShortField('FieldIfSerialNo', 0), lambda pkt: pkt.isSerialNo == SERIAL_NO_TRUE),
    ByteField('ReqDestNetNo', 0),
    XByteField('ReqDestStationNo', DEFAULT_STATION_NO),
    XLEShortField('ReqProcessor', DEFAULT_PROCESSOR),
    XByteField('ReqReserved', 0),
    LEShortField('DataLength', 0)
]


class SLMPBinaryRequest(Packet):
    name = 'SLMP'

    fields_desc = [
        *binary_common_block,
        ShortField('MonitoringTimer', 1),
        XLEShortField('Command', 0),
        XLEShortField('SubCommand', 0),
        LEX3BytesField('HeadDeviceNo', 0x8),
        XByteField('DeviceCode', D_MEM['binary']),
        XLEShortField('NoOfDevicePoints', 0x0100),
        ConditionalField(StrField('Value', 0), lambda pkt: pkt.Command == WRITE_COMMAND)
    ]

    def read(self, register, no_of_device_points):
        self.DataLength = 12
        self.Command = READ_COMMAND
        self.HeadDeviceNo = register
        self.NoOfDevicePoints = no_of_device_points

    def write(self, register, value):
        self.Command = WRITE_COMMAND
        self.HeadDeviceNo = register
        self.Value = int_16_to_binary_le(value)

        num_of_digits = len(list(self.Value))

        self.DataLength = 12 + num_of_digits
        self.NoOfDevicePoints = num_of_digits // 2

    def add_serial_no(self, serial_no):
        self.isSerialNo = SERIAL_NO_TRUE
        self.SerialNo = serial_no


class SLMPBinaryResponse(Packet):
    name = 'SLMP'

    def __init__(self, data_in_bytes):
        Packet.__init__(self, data_in_bytes)

        if self.EndCode > END_CODE_THRESHOLD:
            raise PLCException(self.EndCode)

    def decode_value_field_from_le(self):
        return binary_le_to_binary_be(self.Value)

    fields_desc = [
        *binary_common_block,
        LEShortField('EndCode', 0),
        ConditionalField(StrField('Value', 0), lambda pkt: pkt.EndCode == 0),
        ConditionalField(ByteField('RespNetNo', 0), lambda pkt: pkt.EndCode != 0),
        ConditionalField(ByteField('RespStationNo', 0), lambda pkt: pkt.EndCode != 0),
        ConditionalField(XShortField('RespProcessor', 0), lambda pkt: pkt.EndCode != 0),
        ConditionalField(ByteField('RespReserved', 0), lambda pkt: pkt.EndCode != 0),
        ConditionalField(LEShortField('Command', 0), lambda pkt: pkt.EndCode != 0),
        ConditionalField(LEShortField('SubCommand', 0), lambda pkt: pkt.EndCode != 0)
    ]
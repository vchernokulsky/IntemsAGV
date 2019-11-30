from scapy.all import *

from .constants import *
from .exceptions import *
from .utilities import *

ascii_common_block = [
    XIntField('isSerialNo', int_16_to_ascii(SERIAL_NO_FALSE, 4)),
    ConditionalField(XIntField('SerialNo', ASCII_ZERO_4_BYTE), lambda pkt: pkt.isSerialNo == int_16_to_ascii(SERIAL_NO_TRUE, 4)),
    ConditionalField(XIntField('FieldIfSerialNo', create_ascii_zero_sequence(4)), lambda pkt: pkt.isSerialNo == int_16_to_ascii(SERIAL_NO_TRUE, 4)),
    XShortField('ReqDestNetNo', ASCII_ZERO_2_BYTE),
    XShortField('ReqDestStationNo', int_16_to_ascii(DEFAULT_STATION_NO, 2)),
    XIntField('ReqProcessor', int_16_to_ascii(DEFAULT_PROCESSOR, 4)),
    XShortField('ReqReserved', ASCII_ZERO_2_BYTE),
    XIntField('DataLength', ASCII_ZERO_4_BYTE),
]


class SLMPASCIIRequest(Packet):
    name = 'SLMP'

    fields_desc = [
        *ascii_common_block,
        XIntField('MonitoringTimer', int_16_to_ascii(0x10, 4)),
        XIntField('Command', ASCII_ZERO_4_BYTE),
        XIntField('SubCommand', ASCII_ZERO_4_BYTE),
        XLongField('DeviceCode_and_DeviceNo', concat_ascii_sequences(D_MEM['ascii'], int_16_to_ascii(0x30, 6))),
        XIntField('NoOfDevicePoints', int_16_to_ascii(0x1, 4)),
        ConditionalField(StrField('Value', 0), lambda pkt: pkt.Command == int_16_to_ascii(WRITE_COMMAND, 4))
    ]

    def read(self, register, no_of_device_points):
        self.DataLength = int_16_to_ascii(0x18, 4)
        self.Command = int_16_to_ascii(READ_COMMAND, 4)
        self.DeviceCode_and_DeviceNo = concat_ascii_sequences(D_MEM['ascii'], int_16_to_ascii(register, 6))
        self.NoOfDevicePoints = int_16_to_ascii(no_of_device_points, 4)

    def write(self, register, value):
        value_str = str(value)
        remainder = len(value_str) % 4
        if remainder != 0:
            value_str += ''.join(['0'] * (4 - remainder))

        self.Command = int_16_to_ascii(WRITE_COMMAND, 4)
        self.DeviceCode_and_DeviceNo = concat_ascii_sequences(D_MEM['ascii'], int_16_to_ascii(register, 6))
        self.Value = value_str
        self.DataLength = int_16_to_ascii(int(hex(24 + len(self.Value)), 16), 4)
        self.NoOfDevicePoints = int_16_to_ascii(len(self.Value) // 4, 4)

    def add_serial_no(self, serial_no):
        self.isSerialNo = int_16_to_ascii(SERIAL_NO_TRUE, 4)
        self.SerialNo = int_16_to_ascii(serial_no, 4)


class SLMPASCIIResponse(Packet):
    name = 'SLMP'

    def __init__(self, data_in_bytes):
        Packet.__init__(self, data_in_bytes)

        decoded_end_code = ascii_to_int_16(self.EndCode)

        if decoded_end_code > END_CODE_THRESHOLD:
            raise PLCException(decoded_end_code)

    fields_desc = [
        *ascii_common_block,
        XIntField('EndCode', 0),
        ConditionalField(StrField('Value', 0), lambda pkt: pkt.EndCode == ASCII_ZERO_4_BYTE),
        ConditionalField(XShortField('RespNetNo', 0), lambda pkt: pkt.EndCode != ASCII_ZERO_4_BYTE),
        ConditionalField(XShortField('RespStationNo', 0), lambda pkt: pkt.EndCode != ASCII_ZERO_4_BYTE),
        ConditionalField(XIntField('RespProcessor', 0), lambda pkt: pkt.EndCode != ASCII_ZERO_4_BYTE),
        ConditionalField(XShortField('RespReserved', 0), lambda pkt: pkt.EndCode != ASCII_ZERO_4_BYTE),
        ConditionalField(XIntField('Command', 0), lambda pkt: pkt.EndCode != ASCII_ZERO_4_BYTE),
        ConditionalField(XIntField('SubCommand', 0), lambda pkt: pkt.EndCode != ASCII_ZERO_4_BYTE)
    ]
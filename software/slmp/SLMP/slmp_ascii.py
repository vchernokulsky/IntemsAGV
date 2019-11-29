from scapy.all import *

from .constants import *
from .exceptions import *
from .utilities import *

ascii_common_block = [
    XIntField('isSerialNo', int_16_to_ascii(0x5000, 4)),
    ConditionalField(XIntField('SerialNo', ASCII_ZERO_4_BYTE),
                     lambda pkt: pkt.isSerialNo == int_16_to_ascii(IS_SERIAL_NO, 4)
                     ),
    ConditionalField(XIntField('ThirdDuoOfSubheader', create_ascii_zero_sequence(4)),
                     lambda pkt: pkt.isSerialNo == int_16_to_ascii(IS_SERIAL_NO, 4)
                     ),
    XShortField('ReqDestNetNo', 0),
    XShortField('ReqDestStationNo', 0),
    XIntField('ReqProcessor', 0),
    XShortField('ReqReserved', 0),
    XIntField('DataLength', 0),
]


class SLMPASCIIRequest(Packet):
    name = 'SLMP'
    fields_desc = [
        *ascii_common_block,
        XIntField('MonitoringTimer', int_16_to_ascii(0x10, 4)),
        # Request data starts
        XIntField('Command', ASCII_ZERO_4_BYTE),
        XIntField('SubCommand', ASCII_ZERO_4_BYTE),
        XLongField('DeviceCode_and_DeviceNo', concat_ascii_sequences(0x442A, int_16_to_ascii(0x30, 6))),
        XIntField('NoOfDevicePoints', int_16_to_ascii(0x1, 4)),
        ConditionalField(StrField('Value', 0), lambda paket: paket.Command == int_16_to_ascii(WRITE_COMMAND, 4))
        # Request data ends
    ]

    def read(self, register, no_of_device_points):
        self.ReqDataLength = int_16_to_ascii(0x18, 4)
        self.Command = int_16_to_ascii(READ_COMMAND, 4)
        self.DeviceCode_and_DeviceNo = concat_ascii_sequences(0x442A, int_16_to_ascii(register, 6))
        self.NoOfDevicePoints = int_16_to_ascii(no_of_device_points, 4)

    def write(self, register, value, no_of_device_points):
        self.ReqDataLength = int_16_to_ascii(0x20, 4)
        self.Command = int_16_to_ascii(WRITE_COMMAND, 4)
        self.DeviceCode_and_DeviceNo = concat_ascii_sequences(0x442A, int_16_to_ascii(register, 6))
        self.Value = str(value)
        self.NoOfDevicePoints = int_16_to_ascii(no_of_device_points, 4)

    def add_serial_no(self, serial_no):
        self.isSerialNo = int_16_to_ascii(0x5400, 4)
        self.SerialNo = int_16_to_ascii(serial_no, 4)


class SLMPASCIIResponse(Packet):
    name = 'SLMP'

    def __init__(self, data_in_bytes):
        Packet.__init__(self, data_in_bytes)

        decoded_end_code = ascii_to_int_16(self.EndCode)

        if decoded_end_code > 0x4000:
            raise PLCException(decoded_end_code)

    fields_desc = [
        *ascii_common_block,
        XIntField('EndCode', 0),
        # if success:
        ConditionalField(StrField('Value', 0), lambda pkt: pkt.EndCode == ASCII_ZERO_4_BYTE),
        # else:
        ConditionalField(XShortField('RespNetNo', 0), lambda pkt: pkt.EndCode != ASCII_ZERO_2_BYTE),
        ConditionalField(XShortField('RespStationNo', 0), lambda pkt: pkt.EndCode != ASCII_ZERO_2_BYTE),
        ConditionalField(XIntField('RespProcessor', 0), lambda pkt: pkt.EndCode != ASCII_ZERO_4_BYTE),
        ConditionalField(XShortField('RespReserved', 0), lambda pkt: pkt.EndCode != ASCII_ZERO_2_BYTE),
        ConditionalField(XIntField('Command', 0), lambda pkt: pkt.EndCode != ASCII_ZERO_4_BYTE),
        ConditionalField(XIntField('SubCommand', 0), lambda pkt: pkt.EndCode != ASCII_ZERO_4_BYTE)
    ]
from scapy.all import *

from .constants import *
from .exceptions import *
from .utilities import *
from .fields import *

binary_common_block = [
    XShortField('isSerialNo', SERIAL_NO_FALSE),
    ConditionalField(XShortField('SerialNo', 0), lambda pkt: pkt.isSerialNo == SERIAL_NO_TRUE),
    ConditionalField(XShortField('FieldIfSerialNo', 0), lambda pkt: pkt.isSerialNo == SERIAL_NO_TRUE),
    ByteField('ReqDestNetNo', 0),
    XByteField('ReqDestStationNo', DEFAULT_STATION_NO),
    XLEShortField('ReqProcessor', DEFAULT_PROCESSOR),
    XByteField('ReqReserved', 0),
    LEShortField('DataLength', 12)
]

types_and_sizes = {  # size in words, one word of controller is short
    'signed word': 1,
    'unsigned word': 1,
    'float': 2,
    'signed double word': 2,
    'unsigned double word': 2,
    'string': 0,
    'time': 0,
    'boolean': 1
}


class SLMPBinaryRequest(Packet):
    name = 'SLMP'
    value_type = 'signed word'

    fields_desc = [
        *binary_common_block,
        ShortField('MonitoringTimer', 1),
        XLEShortField('Command', 0),
        XLEShortField('SubCommand', 0),
        LEX3BytesField('HeadDeviceNo', 0x8),
        XByteField('DeviceCode', D_MEM['binary']),
        XLEShortField('NoOfDevicePoints', 2),
        ConditionalField(LESignedShortField('Value', 0),
                         lambda pkt: pkt.Command == WRITE_COMMAND and pkt.value_type == 'signed word'),
        ConditionalField(LEShortField('Value', 0),
                         lambda pkt: pkt.Command == WRITE_COMMAND and pkt.value_type == 'unsigned word'),
        ConditionalField(LEFloatField('Value', 0),
                         lambda pkt: pkt.Command == WRITE_COMMAND and pkt.value_type == 'float'),
        ConditionalField(LESignedIntField('Value', 0),
                         lambda pkt: pkt.Command == WRITE_COMMAND and pkt.value_type == 'signed double word'),
        ConditionalField(LEIntField('Value', 0),
                         lambda pkt: pkt.Command == WRITE_COMMAND and pkt.value_type == 'unsigned double word'),
    ]

    def initialize_request(self, register, command, value, value_type):
        SLMPBinaryRequest.value_type = value_type

        self.Command = command
        self.Value = value
        self.HeadDeviceNo = register

        self.NoOfDevicePoints = types_and_sizes[value_type]
        self.DataLength = 12 + (self.NoOfDevicePoints * 2 if command == WRITE_COMMAND else 0)


class SLMPBinaryResponse(Packet):
    name = 'SLMP'
    value_type = 'signed word'

    fields_desc = [
        *binary_common_block,
        LEShortField('EndCode', 0),
        ConditionalField(LEFloatField('Value', 0), lambda pkt: pkt.EndCode == 0 and pkt.value_type == 'float'),
        ConditionalField(LESignedShortField('Value', 0), lambda pkt: pkt.EndCode == 0 and pkt.value_type == 'signed word'),
        ConditionalField(LEShortField('Value', 0), lambda pkt: pkt.EndCode == 0 and pkt.value_type == 'unsigned word'),
        ConditionalField(LESignedIntField('Value', 0), lambda pkt: pkt.EndCode == 0 and pkt.value_type == 'signed double word'),
        ConditionalField(LEIntField('Value', 0), lambda pkt: pkt.EndCode == 0 and pkt.value_type == 'unsigned double word'),
        ConditionalField(ByteField('RespNetNo', 0), lambda pkt: pkt.EndCode != 0),
        ConditionalField(ByteField('RespStationNo', 0), lambda pkt: pkt.EndCode != 0),
        ConditionalField(XShortField('RespProcessor', 0), lambda pkt: pkt.EndCode != 0),
        ConditionalField(ByteField('RespReserved', 0), lambda pkt: pkt.EndCode != 0),
        ConditionalField(XLEShortField('Command', 0), lambda pkt: pkt.EndCode != 0),
        ConditionalField(XLEShortField('SubCommand', 0), lambda pkt: pkt.EndCode != 0)
    ]

    def __init__(self, data_in_bytes, value_type):
        SLMPBinaryResponse.value_type = value_type
        Packet.__init__(self, data_in_bytes)

        if self.EndCode > END_CODE_THRESHOLD:
            raise PLCException(self.EndCode)
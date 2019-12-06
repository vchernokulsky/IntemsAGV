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
    LEShortField('DataLength', 16)
]


class SLMPBinaryRequest(Packet):
    name = 'SLMP'
    value_type = 'signed word'

    def initialize_request(self, register, value, command, value_type):
        SLMPBinaryRequest.value_type = value_type

        self.Command = command
        self.Value = value
        self.HeadDeviceNo = register

        self.NoOfDevicePoints = SLMPBinaryRequest.types_sizes_dict[value_type]

    types_sizes_dict = {
        'float': 2
    }
    fields_desc = [
        *binary_common_block,
        ShortField('MonitoringTimer', 1),
        XLEShortField('Command', 0),
        XLEShortField('SubCommand', 0),
        LEX3BytesField('HeadDeviceNo', 0x8),
        XByteField('DeviceCode', D_MEM['binary']),
        XLEShortField('NoOfDevicePoints', 2),
        ConditionalField(LEFloatField('Value', 0), lambda pkt: pkt.Command == WRITE_COMMAND and pkt.value_type == 'float'),
    ]


class SLMPBinaryResponse(Packet):
    name = 'SLMP'

    fields_desc = [
        *binary_common_block,
        LEShortField('EndCode', 0),
        #ConditionalField(StrField('Value', 0), lambda pkt: pkt.EndCode == 0),
        #ConditionalField(LEFloatField('Value', 0), lambda pkt: pkt.EndCode == 0),
        ConditionalField(ByteField('RespNetNo', 0), lambda pkt: pkt.EndCode != 0),
        ConditionalField(ByteField('RespStationNo', 0), lambda pkt: pkt.EndCode != 0),
        ConditionalField(XShortField('RespProcessor', 0), lambda pkt: pkt.EndCode != 0),
        ConditionalField(ByteField('RespReserved', 0), lambda pkt: pkt.EndCode != 0),
        ConditionalField(LEShortField('Command', 0), lambda pkt: pkt.EndCode != 0),
        ConditionalField(LEShortField('SubCommand', 0), lambda pkt: pkt.EndCode != 0)
    ]

    def __init__(self, data_in_bytes):
        Packet.__init__(self, data_in_bytes)

        if self.EndCode > END_CODE_THRESHOLD:
            raise PLCException(self.EndCode)

    def decode_value_field_from_le(self):
        return binary_le_to_binary_be(self.Value)
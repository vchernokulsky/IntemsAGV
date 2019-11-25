# -*- coding: utf-8 -*-

"""
ConditionalField - если условие не выполнено,
поле не заполняется нулями -его просто нет.
No - No. - номер
Req - Request
Resp - Response
"""

from .SLMP_exception import PLCException
from .utilities import *

from scapy.all import *

READ_COMMAND = 0x0401
WRITE_COMMAND = 0x1401
FIRST_DUO_OF_SUBHEADER_CONST = 0x5400
ASCII_ZERO_WITH_FIELD_BYTE_SIZE_4 = create_ascii_zero_sequence(4)
ASCII_ZERO_WITH_FIELD_BYTE_SIZE_2 = create_ascii_zero_sequence(2)


class SLMPBinaryRequest(Packet):
    name = 'SLMP'

    fields_desc = [
        # Subheader starts
        XShortField('FirstDuoOfSubheader', 0x5000),
        ConditionalField(ShortField('SerialNo', 0),
                         lambda paket: paket.FirstDuoOfSubheader == FIRST_DUO_OF_SUBHEADER_CONST
                         ),
        ConditionalField(ShortField('ThirdDuoOfSubheader', 0),
                         lambda paket: paket.FirstDuoOfSubheader == FIRST_DUO_OF_SUBHEADER_CONST
                         ),
        # Subheader ends
        ByteField('ReqDestNetNo', 0),
        XByteField('ReqDestStationNo', 0xFF),
        XShortField('ReqProcessor', 0xFF03),
        ByteField('ReqReserved', 0),
        LEShortField('ReqDataLength', 0),
        ShortField('MonitoringTimer', 1),
        # Request data starts
        LEShortField('Command', 0),
        ShortField('SubCommand', 0),
        LEThreeBytesField('HeadDeviceNo', 0x8),
        ByteField('DeviceCode', 0xA8),
        ShortField('NoOfDevicePoints', 0x0100),
        ConditionalField(LEShortField('Value', 0),
                         lambda paket: paket.Command == WRITE_COMMAND
                         )
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
        self.FirstDuoOfSubheader = FIRST_DUO_OF_SUBHEADER_CONST
        self.SerialNo = serial_no


class SLMPBinaryResponse(Packet):
    name = 'SLMP'

    def __init__(self, data_in_bytes):
        Packet.__init__(self, data_in_bytes)

        if self.EndCode > 0x4000:
            raise PLCException(self.EndCode)

    fields_desc = [
        # Subheader starts
        XShortField('FirstDuoOfSubheader', 0xD000),
        ConditionalField(ShortField('SerialNo', 0),
                         lambda paket: paket.FirstDuoOfSubheader == 0xD400
                         ),
        ConditionalField(ShortField('ThirdDuoOfSubheader', 0),
                         lambda paket: paket.FirstDuoOfSubheader == 0xD400
                         ),
        # Subheader ends
        ByteField('ReqDestNetNo', 0),
        XByteField('ReqDestStationNo', 0),
        ShortField('ReqProcessor', 0),
        ByteField('ReqReserved', 0),
        LEShortField('RespDataLength', 0),
        LEShortField('EndCode', 0),
        # Response data starts
        # if success:
        #     Successful case starts
        ConditionalField(LEShortField('Value', 0),
                         lambda packet: packet.EndCode == 0
                         ),
        #     Successful case ends
        # else:
        #     Error case starts
        ConditionalField(ByteField('RespNetNo', 0),
                         lambda packet: packet.EndCode != 0
                         ),
        ConditionalField(ByteField('RespStationNo', 0),
                         lambda packet: packet.EndCode != 0
                         ),
        ConditionalField(XShortField('RespProcessor', 0),
                         lambda packet: packet.EndCode != 0
                         ),
        ConditionalField(ByteField('RespReserved', 0),
                         lambda packet: packet.EndCode != 0
                         ),
        ConditionalField(LEShortField('Command', 0),
                         lambda packet: packet.EndCode != 0
                         ),
        ConditionalField(LEShortField('SubCommand', 0),
                         lambda packet: packet.EndCode != 0
                         )
        #     Error case ends
        # Response data ends
    ]


class SLMPASCIIRequest(Packet):
    name = 'SLMP'
    fields_desc = [
        # Subheader starts
        XIntField('FirstDuoOfSubheader', int_16_to_ascii(0x5000, 4)),
        ConditionalField(XIntField('SerialNo', ASCII_ZERO_WITH_FIELD_BYTE_SIZE_4),
                         lambda paket: paket.FirstDuoOfSubheader == int_16_to_ascii(FIRST_DUO_OF_SUBHEADER_CONST, 4)
                         ),
        ConditionalField(XIntField('ThirdDuoOfSubheader', create_ascii_zero_sequence(4)),
                         lambda paket: paket.FirstDuoOfSubheader == int_16_to_ascii(FIRST_DUO_OF_SUBHEADER_CONST, 4)
                         ),
        # Subheader ends
        XShortField('ReqDestNetNo', ASCII_ZERO_WITH_FIELD_BYTE_SIZE_2),
        XShortField('ReqDestStationNo', int_16_to_ascii(0xff, 2)),
        XIntField('ReqProcessor',  int_16_to_ascii(0x3ff, 4)),
        XShortField('ReqReserved', ASCII_ZERO_WITH_FIELD_BYTE_SIZE_2),
        XIntField('ReqDataLength', ASCII_ZERO_WITH_FIELD_BYTE_SIZE_4),
        XIntField('MonitoringTimer', int_16_to_ascii(0x10, 4)),
        # Request data starts
        XIntField('Command', ASCII_ZERO_WITH_FIELD_BYTE_SIZE_4),
        XIntField('SubCommand', ASCII_ZERO_WITH_FIELD_BYTE_SIZE_4),
        XLongField('DeviceCode_and_DeviceNo', concat_ascii_sequences(0x442A, int_16_to_ascii(0x30, 6))),
        XIntField('NoOfDevicePoints', int_16_to_ascii(0x1, 4)),
        ConditionalField(XIntField('Value', ASCII_ZERO_WITH_FIELD_BYTE_SIZE_4),
                         lambda paket: paket.Command == int_16_to_ascii(WRITE_COMMAND, 4)
                         )
        # Request data ends
    ]

    def read(self, register):
        self.ReqDataLength = int_16_to_ascii(0x18, 4)
        self.Command = int_16_to_ascii(READ_COMMAND, 4)
        self.DeviceCode_and_DeviceNo = concat_ascii_sequences(0x442A, int_16_to_ascii(register, 6))

    def write(self, register, value):
        self.ReqDataLength = int_16_to_ascii(0x20, 4)
        self.Command = int_16_to_ascii(WRITE_COMMAND)
        self.HeadDeviceNo = int_16_to_ascii(register)
        self.Value = int_16_to_ascii(value)

    def add_serial_no(self, serial_no):
        self.FirstDuoOfSubheader = int_16_to_ascii(0x5400, 4)
        self.SerialNo = int_16_to_ascii(serial_no, 4)


class SLMPASCIIResponse(Packet):
    name = 'SLMP'

    def __init__(self, data_in_bytes):
        Packet.__init__(self, data_in_bytes)

        decoded_end_code = ascii_to_int_16(self.EndCode)

        if decoded_end_code > 0x4000:
            raise PLCException(decoded_end_code)

    fields_desc = [
        # Subheader starts
        XIntField('FirstDuoOfSubheader', int_16_to_ascii(0xD000, 4)),
        ConditionalField(XIntField('SerialNo', 0),
                        lambda paket: paket.FirstDuoOfSubheader == int_16_to_ascii(0xD400, 4)
                         ),
        ConditionalField(XIntField('ThirdDuoOfSubheader', 0),
                         lambda paket: paket.FirstDuoOfSubheader == int_16_to_ascii(0xD400, 4)
                        ),
        # Subheader ends
        XShortField('ReqDestNetNo', 0),
        XShortField('ReqDestStationNo', 0),
        XIntField('ReqProcessor', 0),
        XShortField('ReqReserved', 0),
        XIntField('RespDataLength', 0),
        XIntField('EndCode', 0),
        # Response data starts
        # if success:
        #     Successful case starts
        ConditionalField(XIntField('Value', 0),
                         lambda packet: packet.EndCode == ASCII_ZERO_WITH_FIELD_BYTE_SIZE_4
                         ),
        #     Successful case ends
        # else:
        #     Error case starts
        ConditionalField(XShortField('RespNetNo', 0),
                         lambda packet: packet.EndCode != ASCII_ZERO_WITH_FIELD_BYTE_SIZE_4
                         ),
        ConditionalField(XShortField('RespStationNo', 0),
                         lambda packet: packet.EndCode != ASCII_ZERO_WITH_FIELD_BYTE_SIZE_4
                         ),
        ConditionalField(XIntField('RespProcessor', 0),
                         lambda packet: packet.EndCode != ASCII_ZERO_WITH_FIELD_BYTE_SIZE_4
                         ),
        ConditionalField(XShortField('RespReserved', 0),
                         lambda packet: packet.EndCode != ASCII_ZERO_WITH_FIELD_BYTE_SIZE_4
                         ),
        ConditionalField(XIntField('Command', 0),
                         lambda packet: packet.EndCode != ASCII_ZERO_WITH_FIELD_BYTE_SIZE_4
                         ),
        ConditionalField(XIntField('SubCommand', 0),
                         lambda packet: packet.EndCode != ASCII_ZERO_WITH_FIELD_BYTE_SIZE_4
                         )
        #     Error case ends
        # Response data ends
    ]
# -*- coding: utf-8 -*-

"""
ConditionalField - если условие не выполнено,
поле не заполняется нулями -его просто нет.
No - No. - номер
Req - Request
Resp - Response
"""

from scapy.all import *

READ_COMMAND = 0x0401
WRITE_COMMAND = 0x1401
FIRST_QUADRO_F_SUBHEADER_CONST = 0x5400


class SLMPRequest(Packet):
    name = 'SLMP'

    fields_desc = [
        # Subheader starts
        XShortField('FirstQuadroOfSubheader', 0x5000),
        ConditionalField(ShortField('SerialNo', 0),
                         lambda paket: paket.FirstQuadroOfSubheader == FIRST_QUADRO_F_SUBHEADER_CONST
                         ),
        ConditionalField(ShortField('ThirdQuadroOfSubheader', 0),
                         lambda paket: paket.FirstQuadroOfSubheader == FIRST_QUADRO_F_SUBHEADER_CONST
                         ),
        # Subheader ends
        ByteField('ReqDestNetNo',0),
        XByteField('ReqDestStationNo', 0xFF),
        XShortField('Processor',0xFF03),
        ByteField('Reserved', 0),
        LEShortField('ReqDataLength',0),                              # _
        ShortField('MonitoringTimer', 1),                             #  |
        # Request data starts                                         #  |
        LEShortField('Command',0),                                    #  |
        ShortField('SubCommand',0),                                   #  |
        LEThreeBytesField('HeadDeviceNo',0x8),                        #  | = tmp. RequestDataLength = \
        ByteField('DeviceCode',0xA8),                                 #  |        sum([len(field) for field in tmp])
        ShortField('NoOfDevicePoints', 0x0100),                       #  |
        ConditionalField(LEShortField('Value', 0),                      #  |
                         lambda paket: paket.Command == WRITE_COMMAND #  |
                         )                                            # _|
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
        self.FirstQuadroOfSubheader = FIRST_QUADRO_F_SUBHEADER_CONST
        self.SerialNo = serial_no


class SLMPResponse(Packet):
    name = 'SLMP'

    fields_desc = [
        # Subheader starts
        XShortField('FirstQuadroOfSubheader', 0xD000),
        ConditionalField(ShortField('SerialNo', 0),
                         lambda paket: paket.FirstQuadroOfSubheader == 0xD400
                         ),
        ConditionalField(ShortField('ThirdQuadroOfSubheader', 0),
                         lambda paket: paket.FirstQuadroOfSubheader == 0xD400
                         ),
        # Subheader ends
        ByteField('ReqDestNetNo',0),
        XByteField('ReqDestStationNo', 0),
        XShortField('Processor',0),
        ByteField('Reserved', 0),
        LEShortField('RespDataLength',0),
        ShortField('EndCode', 0),
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
        ConditionalField(XShortField('Processor', 0),
                         lambda packet: packet.EndCode != 0
                         ),
        ConditionalField(ByteField('Reserved', 0),
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

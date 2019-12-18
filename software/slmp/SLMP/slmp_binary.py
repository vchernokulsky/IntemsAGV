from scapy.all import *

from .constants import *
from .exceptions import *
from .utilities import *
from .fields import *


class SLMPBinary(Packet):
    name = 'SLMP'
    value_type = 'sw'
    direction = 'request'

    fields_desc = [
        XShortField('isSerialNo', SERIAL_NO_FALSE),
        ConditionalField(XShortField('SerialNo', 0), lambda pkt: pkt.isSerialNo == SERIAL_NO_TRUE),
        ConditionalField(XShortField('FieldIfSerialNo', 0), lambda pkt: pkt.isSerialNo == SERIAL_NO_TRUE),
        ByteField('ReqDestNetNo', 0),
        XByteField('ReqDestStationNo', DEFAULT_STATION_NO),
        XLEShortField('ReqProcessor', DEFAULT_PROCESSOR),
        XByteField('ReqReserved', 0),
        LEShortField('DataLength', 0),
        ConditionalField(ShortField('MonitoringTimer', 1), lambda pkt: pkt.direction == 'request'),
        ConditionalField(XLEShortField('Command', 0), lambda pkt: pkt.direction == 'request'),
        ConditionalField(XLEShortField('SubCommand', 0), lambda pkt: pkt.direction == 'request'),
        ConditionalField(LEX3BytesField('HeadDeviceNo', 0), lambda pkt: pkt.direction == 'request'),
        ConditionalField(XByteField('DeviceCode', 0), lambda pkt: pkt.direction == 'request'),
        ConditionalField(XLEShortField('NoOfDevicePoints', 0), lambda pkt: pkt.direction == 'request'),
        ConditionalField(LEShortField('EndCode', 0), lambda pkt: pkt.direction == 'response'),
        ConditionalField(LESignedShortField('Value', 0), lambda pkt: (
                                                                                 pkt.direction == 'response' or pkt.Command == WRITE_COMMAND) and pkt.value_type == 'sw'),
        ConditionalField(LEShortField('Value', 0), lambda pkt: (
                                                                           pkt.direction == 'response' or pkt.Command == WRITE_COMMAND) and pkt.value_type == 'uw'),
        ConditionalField(LEFloatField('Value', 0), lambda pkt: (
                                                                           pkt.direction == 'response' or pkt.Command == WRITE_COMMAND) and pkt.value_type == 'f'),
        ConditionalField(LESignedIntField('Value', 0), lambda pkt: (
                                                                               pkt.direction == 'response' or pkt.Command == WRITE_COMMAND) and pkt.value_type == 'sdw'),
        ConditionalField(LEIntField('Value', 0), lambda pkt: (
                                                                         pkt.direction == 'response' or pkt.Command == WRITE_COMMAND) and pkt.value_type == 'udw'),
        ConditionalField(StrField('Value', ''), lambda pkt: (
                                                                        pkt.direction == 'response' or pkt.Command == WRITE_COMMAND) and pkt.value_type == 's'),
        ConditionalField(BooleanField('Value', False), lambda pkt: (
                                                                               pkt.direction == 'response' or pkt.Command == WRITE_COMMAND) and pkt.value_type == 'bool'),
    ]

    @classmethod
    def prepare_request(cls, register, device_code, command, value=None, no_of_device_points=None, dump=None):
        SLMPBinary.direction = 'request'

        if SLMPBinary.value_type not in types_and_sizes.keys():
            raise Exception('value_type does not match anything')

        if no_of_device_points is None and dump is None and value is None:
            raise Exception('no_of_device_points or dump or value must be specified')

        if dump is not None and dump is not isinstance(dump, bytes):
            raise Exception('dump must belong to type bytes')

        instance = cls(Command=command,
                       Value=value,
                       DeviceCode=device_code,
                       HeadDeviceNo=int(str(register), 8) if device_code == MEM['binary']['Y'] else register,
                       NoOfDevicePoints=types_and_sizes[SLMPBinary.value_type],
                       DataLength=12 + (types_and_sizes[SLMPBinary.value_type] * 2 if command == WRITE_COMMAND else 0)
                       )

        if SLMPBinary.value_type != 'dump':
            return instance

        if command == READ_COMMAND:
            instance.NoOfDevicePoints = no_of_device_points
            instance.DataLength = 12

            return instance

        instance.NoOfDevicePoints = len(dump) / 2
        instance.DataLength = 12 + instance.NoOfDevicePoints * 2

        return instance / dump

    @classmethod
    def form_response(cls, data_in_bytes):
        SLMPBinary.direction = 'response'

        if SLMPBinary.value_type not in types_and_sizes.keys():
            raise Exception('value_type does not match anything')

        instance = cls(data_in_bytes)

        if instance.EndCode > END_CODE_THRESHOLD:
            raise PLCException(instance.EndCode)

        return instance

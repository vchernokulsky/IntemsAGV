import pytest
from json import load
import sys

from SLMP.slmp_binary import *

sys.path.append('../')

with open('test_suit.json', 'r') as f:
    test_suit = load(f)['SLMPBinary']


@pytest.mark.parametrize('test_case', test_suit['correctReqPacketSize'])
def test_correct_request_packet_size_0(test_case):
    method, operatin_params, whole_size, data_length_field_value = test_case

    pkt = SLMPBinaryRequest()

    exec('pkt.' + method + '(' + ','.join(operatin_params) + ')')
    assert len(list(bytes(pkt))) == eval(whole_size)
    assert pkt.DataLength == eval(data_length_field_value)


@pytest.mark.parametrize('test_case', test_suit['correctRespPacketConsistenseFromByteSequense'])
def test_correct_request_packet_size_1(test_case):
    byte_sequence, fields_to_be, fields_not_to_be = test_case

    pkt = SLMPBinaryRequest(byte_sequence.encode())

    for field in fields_to_be:
        assert hasattr(pkt, field)

    for field in fields_not_to_be:
        assert not hasattr(pkt, field)
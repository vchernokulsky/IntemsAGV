import pytest
from json import load
import sys

sys.path.append('../')

from SLMP.slmp_ascii import *
from SLMP.utilities import *

with open('test_suit.json', 'r') as f:
    test_suit = load(f)['SLMPASCII']


@pytest.mark.parametrize('test_case', test_suit['correctReqPacketSize'])
def test_correct_request_packet_size_0(test_case):
    method, method_params, whole_size, data_length_field_value = test_case

    pkt = SLMPASCIIRequest()

    exec('pkt.' + method + '(' + ','.join(method_params) + ')')
    assert len(list(bytes(pkt))) == eval(whole_size)
    assert pkt.DataLength == int_16_to_ascii(eval(data_length_field_value), 4)


@pytest.mark.parametrize('test_case', test_suit['correctBadRespPacketBehavior'])
def test_correct_request_packet_size_2(test_case):
    byte_sequence = test_case

    with pytest.raises(PLCException):
        pkt = SLMPASCIIResponse(eval(byte_sequence))
"""
Скрипт тестирует корректное формирование SLMP-пакета.
Проверяется:
	1) размер сформированного пакета для запроса,
	2) значение поля RequestDataLength (ReqDataLength в коде),
	3) состав пакета, полученного от устройства.
"""

import pytest
from json import load

from SLMP.SLMP import *


with open('test_suit.json', 'r', encoding='utf-8') as f:
	test_suit = load(f)


@pytest.mark.parametrize('test_case', test_suit['correctRequestPacketSize'])
def test_correct_request_packet_size(test_case):
	operation, params, answer = test_case

	packet = SLMPRequest()

	exec('packet.' + operation + '(' + ','.join(params) + ')')
	assert len(list(bytes(packet))) == eval(answer)


@pytest.mark.parametrize('test_case', test_suit['correctValueOfRequestDataLengthField'])
def test_correct_value_of_data_length_field(test_case):
	operation, params, answer = test_case

	packet = SLMPRequest()

	exec('packet.' + operation + '(' + ','.join(params) + ')')
	assert packet.ReqDataLength == eval(answer)


@pytest.mark.parametrize('test_case', test_suit['correctResponsePacketConsistense'])
def test_correct_response_package_consistence(test_case):
	packet_in_bytes_represented_as_str, fields = test_case

	packet = SLMPResponse(packet_in_bytes_represented_as_str.encode())
	
	for field in fields:
		assert hasattr(packet, field)

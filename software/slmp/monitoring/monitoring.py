import json
from recordclass import make_dataclass
from collections import defaultdict
import math
from scapy.all import Raw

from .network import *
from .bytes_parser import *
from SLMP.slmp_binary import *

with open('2CAR/conn.json', 'r') as f:
    conns = json.load(f)['connectors']

AnalogConnector = make_dataclass('AnalogConnector',
                                 ('value_type', 'type', 'name', 'threshold', 'register', 'description', 'direction'))
DiscreteConnector = make_dataclass('DiscreteConnector',
                                   ('value_type', 'type', 'name', 'register', 'description', 'direction'))


class Connection(object):
    def __init__(self, host: int, port: int, protocol: str):
        # conn_id = index + 1
        self.conns = tuple(AnalogConnector(value_type=conn['valueType'],
                                           type=conn['type'],
                                           name=conn['name'],
                                           threshold=conn['threshold'],
                                           register=conn['register'],
                                           description=conn['description'],
                                           direction=conn['direction']
                                           ) if conn['type'] == 'analog'
                           else
                           DiscreteConnector(value_type=conn['valueType'],
                                             type=conn['type'],
                                             name=conn['name'],
                                             register=conn['register'],
                                             description=conn['description'],
                                             direction=conn['direction']
                                             )
                           for conn in conns)
        self.word_size = 2  # size of word of controller
        self.host = host
        self.port = port
        self.protocol = protocol

    def read_connectors(self, ids: list):
        self._is_ids_correct(ids, 'input')

        ids_and_values = []

        for mem_area, id_list in self._group_ids_by_mem_area(ids).items():
            ids_and_values += self._make_read_area_request(mem_area, id_list)

        return ids_and_values

    def write_to_connectors(self, ids_and_values: list):
        ids_and_values = dict(ids_and_values)  # variable ids_and_values won't change outside

        self._is_ids_correct(ids_and_values.keys(), 'output')

        for mem_area, ids in self._group_ids_by_mem_area(ids_and_values.keys()).items():
            ids = sorted(ids, key=lambda id: int(conns[id - 1].register[1:]))  # sorting by registers order in memory

            def ids_cmp(a, b):
                return int(self.conns[a - 1].register[1:]) - self.word_size == int(conns[b - 1].register[1:])

            list_of_blocks_of_ids = []  # contains of lists of ids of connectors which registers are placed together
            buffer = [ids[0]]

            for i in range(1, len(ids)):
                # below there is ids comparing by closeness of there registers in memory
                if ids_cmp(ids[i], ids[i - 1]):
                    buffer += [ids[i]]
                else:
                    list_of_blocks_of_ids += [buffer]
                    buffer = [ids[i]]
            list_of_blocks_of_ids += [buffer]

            for block_of_ids in list_of_blocks_of_ids:
                block_of_ids_and_values = list(map(lambda id: (id, ids_and_values[id]), block_of_ids))
                self._make_write_to_area_request(mem_area, block_of_ids_and_values)

    def _is_ids_correct(self, ids: list, direction: str):
        if any(id > len(self.conns) for id in ids):
            raise Exception('id is out of connectors id range')

        if any(self.conns[id - 1].direction != direction for id in ids):
            raise Exception('connection id belongs other connection')
        
    def _group_ids_by_mem_area(self, ids: list):
        mem_area_and_id_list = defaultdict(list)

        for mem_area, id in list(map(lambda id: (self.conns[id - 1].register[0], id), ids)):
            mem_area_and_id_list[mem_area] += [id]

        return mem_area_and_id_list

    def _make_read_area_request(self, mem_area: str, id_list: list):
        min_addr, max_addr = self._count_size_of_data_field(id_list)

        SLMPBinary.value_type = 'dump'
        no_of_device_points = math.ceil((max_addr - min_addr) / self.word_size)
        req = SLMPBinary.prepare_dump_request(min_addr, MEM['binary'][mem_area], READ_COMMAND, no_of_device_points)

        resp_data = connect(self.protocol, self.host, self.post, bytes(req))
        resp = SLMPBinary(resp_data)

        return parse_bytes(resp.getlayer(Raw), self.conns, id_list, min_addr)

    def _count_size_of_data_field(self, id_list: list):
        min_addr = min([int(self.conns[id - 1].register[1:]) for id in id_list])
        max_addr = max(
            [int(self.conns[id - 1].register[1:]) + types_and_sizes[self.conns[id - 1].value_type] for id in
             id_list])
        return min_addr, max_addr

    def _make_write_to_area_request(self, mem_area: str, block_of_ids_and_values: list):
        min_addr, max_addr = self._count_size_of_data_field(mem_area, block_of_ids_and_values.keys())
        dump = dump_bytes(self.conns, block_of_ids_and_values)
        SLMPBinary.value_type = 'dump'
        req = SLMPBinary.prepare_request(min_addr, MEM['binary'][mem_area], WRITE_COMMAND, dump)
        resp_data = connect('udp', self.host, self.post, bytes(req))

        SLMPBinary(resp_data)  # it's a check if request was successfull, there is an exception clause inside


connector = Connection()
connector.read_connectors([i for i in range(1, 6)])

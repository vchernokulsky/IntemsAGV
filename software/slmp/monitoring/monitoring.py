from .network import *
from .bytes_parser import *
from .settings import *
from SLMP.slmp_binary import *

from collections import defaultdict
import math
from scapy.all import Raw


class Singleton(type):
    def __call__(cls, *args, **kwargs):
        if not hasattr(cls, 'instance'):
            cls.instance = super(Singleton, cls).__call__(*args, **kwargs)
        return cls.instance


class Connection(metaclass=Singleton):
    def __init__(self, host: int, port: int, protocol: str):
        # conn_id = index + 1
        self.conns = tuple(AnalogConnector(value_type=conn['valueType'],
                                           type=conn['type'],
                                           name=conn['name'],
                                           threshold=conn['threshold'],
                                           register=conn['register'],
                                           description=conn['description'],
                                           direction=conn['direction'],
                                           cashed=conn['cashed']
                                           ) if conn['type'] == 'analog'
                           else
                           DiscreteConnector(value_type=conn['valueType'],
                                             type=conn['type'],
                                             name=conn['name'],
                                             register=conn['register'],
                                             description=conn['description'],
                                             direction=conn['direction'],
                                             cashed=conn['cashed']
                                             )
                           for conn in conns)
        # cash: key - id, value - reg value
        self.cash = dict(list(map(lambda e: (e[0] + 1, 0), list(filter(lambda e: e[1].cashed, list(enumerate(self.conns)))))))
        self.word_size = 2  # size of word of controller
        self.max_no_of_device_points = 960
        self.host = host
        self.port = port
        self.protocol = protocol

    def read_connectors(self, ids: list):
        self._is_ids_correct(ids, 'input')

        ids_and_values = []

        for mem_area, id_list in self._group_ids_by_mem_area(ids).items():
            ids_and_values += self._make_read_area_request(mem_area, id_list)

            done = dict(ids_and_values)
            cashed_ids = self.cash.keys() & done.keys()

            for id in cashed_ids:
                self.cash[id] = done[id]

        return ids_and_values

    def write_to_connectors(self, ids_and_values: list):
        ids_and_values = dict(ids_and_values)  # variable ids_and_values won't change outside

        self._is_ids_correct(ids_and_values.keys(), 'output')

        for mem_area, ids in self._group_ids_by_mem_area(ids_and_values.keys()).items():
            ids = sorted(ids, key=lambda id: int(self.conns[id - 1].register[1:]))  # sorting by registers order in memory

            def ids_cmp(conns, a, b):
                return int(conns[a - 1].register[1:]) - self.word_size == int(conns[b - 1].register[1:])

            list_of_blocks_of_ids = []  # contains of lists of ids of connectors which registers are placed together
            buffer = [ids[0]]

            for i in range(1, len(ids)):
                # below there is ids comparing by closeness of there registers in memory
                if ids_cmp(self.conns, ids[i], ids[i - 1]) and len(buffer) <= self.max_no_of_device_points:
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
            raise Exception('connection id belongs other direction')
        
    def _group_ids_by_mem_area(self, ids: list):
        mem_area_and_id_list = defaultdict(list)

        for mem_area, id in list(map(lambda id: (self.conns[id - 1].register[0], id), ids)):
            mem_area_and_id_list[mem_area] += [id]

        return mem_area_and_id_list

    def _make_read_area_request(self, mem_area: str, id_list: list):
        min_addr, max_addr = self._count_size_of_data_field(id_list)

        SLMPBinary.value_type = 'dump'
        no_of_device_points = math.ceil((max_addr - min_addr) / self.word_size)

        req = SLMPBinary.prepare_request(min_addr, MEM['binary'][mem_area], READ_COMMAND, no_of_device_points=no_of_device_points)

        resp_data = connect(self.protocol, self.host, self.port, bytes(req))
        resp = SLMPBinary.form_response(resp_data)

        return parse_bytes(bytes(resp.getlayer(Raw)), self.conns, id_list, min_addr, self.word_size)

    def _count_size_of_data_field(self, id_list: list):
        min_addr = min([int(self.conns[id - 1].register[1:]) for id in id_list])
        max_addr = max(
            [int(self.conns[id - 1].register[1:]) + types_and_sizes[self.conns[id - 1].value_type] * self.word_size for id in
             id_list])

        return min_addr, max_addr

    def _make_write_to_area_request(self, mem_area: str, block_of_ids_and_values: list):
        ids, values = list(zip(*block_of_ids_and_values))
        min_addr, max_addr = self._count_size_of_data_field(ids)
        dump = dump_bytes(self.conns, block_of_ids_and_values)
        SLMPBinary.value_type = 'dump'
        req = SLMPBinary.prepare_request(min_addr, MEM['binary'][mem_area], WRITE_COMMAND, value=dump)
        resp_data = connect('udp', self.host, self.port, bytes(req))

        SLMPBinary.form_response(resp_data)  # it's a check if request was successfull, there is an exception clause inside

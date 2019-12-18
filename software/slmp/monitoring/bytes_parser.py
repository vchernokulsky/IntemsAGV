from SLMP.slmp_binary import types_and_sizes
import struct


def parse_bytes(data_in_bytes, conns, id_list, offset):
    data_in_bytearray = bytearray(data_in_bytes)
    parsed_data = []

    for id in id_list:
        begin = conns[id - 1 - offset]
        end = conns[id - 1 - offset] + types_and_sizes[conns[id - 1].value_type]
        value_in_bytes = data_in_bytearray[begin: end]

        value = struct.unpack(types_and_fmt[conns[id - 1].value_type], value_in_bytes)

        parsed_data += [(id, value)]

    return parsed_data


def dump_bytes(conns, ids_and_values_list: list):
    dump = b''

    for id, value in ids_and_values_list.sort(key=lambda x: x[0]):
        dump += struct.pack(types_and_fmt[conns[id - 1].value_type], value)

    return dump

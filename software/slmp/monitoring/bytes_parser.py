from SLMP.slmp_binary import types_and_sizes, types_and_fmt
import struct


def parse_bytes(data_in_bytes, conns, id_list, offset, word_size):
    data_in_bytearray = bytearray(data_in_bytes)
    parsed_data = []

    for id in id_list:
        begin = int(conns[id - 1].register[1:]) - offset

        end = begin + types_and_sizes[conns[id - 1].value_type] * word_size
        value_in_bytes = data_in_bytearray[begin: end]

        value = struct.unpack(types_and_fmt[conns[id - 1].value_type], value_in_bytes)[0]

        parsed_data += [(id, value)]

    return parsed_data


def dump_bytes(conns, ids_and_values_list: list):
    dump = b''
    ids_and_values_list.sort(key=lambda x: x[0])

    for _, value in ids_and_values_list:
        dump += value

    return dump

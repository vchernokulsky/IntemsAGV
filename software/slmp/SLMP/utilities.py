def create_ascii_zero_sequence(zero_count):
    return int('0x' + ''.join(['30'] * zero_count), 16) if zero_count != 0 else ''


def int_16_to_ascii(value, field_byte_size):
    list_of_value_16_digits = list(hex(value).upper())[2:]
    forward_zero_count = field_byte_size - len(list_of_value_16_digits)

    ascii_str = '0x' + \
                (hex(create_ascii_zero_sequence(forward_zero_count))[2:] if forward_zero_count > 0 else '') + \
                ''.join(map(lambda ch: hex(ord(ch))[2:], list_of_value_16_digits))
    return int(ascii_str, 16)


def concat_ascii_sequences(seq_begin, seq_end):
    return int(hex(seq_begin) + hex(seq_end)[2:], 16)


def ascii_to_int_16(value):
    ascii_str = hex(value)
    list_of_ascii_digits = list(ascii_str)[2:]

    not_30_index = -1

    for i in range(0, len(list_of_ascii_digits), 2):
        if ''.join(list_of_ascii_digits[i:i + 2]) != '30':
            not_30_index = i
            break

    if not_30_index == -1:
        return 0

    ascii_str = ''.join(list_of_ascii_digits[not_30_index:])
    
    value_16_str = '0x' + ''.join([chr(int(''.join([ascii_str[i: i + 2]]), 16)) for i in range(0, len(ascii_str), 2)])

    value_16 = int(value_16_str, 16)

    return value_16
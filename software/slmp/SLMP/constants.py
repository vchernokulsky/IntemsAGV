from .utilities import create_ascii_zero_sequence

READ_COMMAND = 0x0401
WRITE_COMMAND = 0x1401
IS_SERIAL_NO = 0x5400
ASCII_ZERO_4_BYTE = create_ascii_zero_sequence(4)  # creates field of ascii zeros size of 4
ASCII_ZERO_2_BYTE = create_ascii_zero_sequence(2)  # creates field of ascii zeros size of 2

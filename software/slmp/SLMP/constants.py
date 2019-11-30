from .utilities import create_ascii_zero_sequence

READ_COMMAND = 0x0401
WRITE_COMMAND = 0x1401
SERIAL_NO_TRUE = 0x5400  # it means that there is serial no. (number) in packet
SERIAL_NO_FALSE = 0x5000  # it means that there is no serial no. (number) in packet
DEFAULT_STATION_NO = 0xff
DEFAULT_PROCESSOR = 0x3ff
D_MEM = {'binary': 0xa8, 'ascii': 0x442a}  # device code to read D area of memory
END_CODE_THRESHOLD = 0x4000  # code which upper 0x4000 connected with slmp functions
ASCII_ZERO_4_BYTE = create_ascii_zero_sequence(4)  # creates sequence of 4 ascii zeros
ASCII_ZERO_2_BYTE = create_ascii_zero_sequence(2)  # creates sequence of 2 ascii zeros

end_code_dict = {
        0xC050: 'When the communication data code is set to “ASCII”,'
                'ASCII code data which cannot be converted to binary is received.',
        0xC051: 'Maximum number of bit devices for which data can be read/written'
                ' all at once is outside the allowable range.',
        0xC052: 'Maximum number of word devices for which data can be read/written'
                ' all at once is outside the allowable range.',
        0xC053: 'Maximum number of bit devices for which data can be random'
                'read/written all at once is outside the allowable range.',
        0xC054: 'Maximum number of word devices for which data can be random'
                'read/written all at once is outside the allowable range.',
        0xC056: 'Read or write request exceeds maximum address.',
        0xC058: 'Request data length after ASCII-to-binary conversion does not'
                'match the number of data in the character section (part of text).',
        0xC059: '• Error in command or subcommand specification.'
                '• There is a command or subcommand that cannot be used by the CPU module.',
        0xC05B: 'CPU module cannot read or write from/to specified device.',
        0xC05C: 'Error in request contents. (Reading or writing by bit unit for word device, etc.)',
        0xC05F: 'There is a request that cannot be executed for the target CPU module.',
        0xC060: 'Error in request contents. (Error in specification of data for bit device, etc.)',
        0xC061: 'Request data length does not match the number of data in the'
                'character section (part of text).',
        0xC06F: 'When the communication data code is set to “Binary”, a request.'
                'message of ASCII is received. (Error history of this error code is'
                'registered but no error response is sent.)',
        0xC0D8: 'The number of specified blocks exceeds the range.',
        0xC200: 'Error in remote password.',
        0xC201: 'Locked status of the remote password of the port which is used'
                'for communication.',
        0xC204: 'Different device requested remote password to be unlocked.',
        0xC810: 'Error in remote password.',
        0xC815: 'Error in remote password.',
        0xC816: 'Remote password authentication is locked out.'
    }
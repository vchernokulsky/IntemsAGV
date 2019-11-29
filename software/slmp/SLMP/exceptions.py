from .constants import end_code_dict


class PLCException(Exception):

    def __init__(self, end_code):
        Exception.__init__(self)

        self.description = str(hex(end_code)) + ':'

        if end_code in end_code_dict.keys():
            self.description += end_code_dict[end_code]
        elif 0x3FFF < end_code < 0x5000:
            self.description += 'Error detected by CPU module.'
        else:
            self.description += 'EndCode belongs SLMP function error code interval, ' \
                                'but doesn`t match anything.'

    def __str__(self):
        return self.description

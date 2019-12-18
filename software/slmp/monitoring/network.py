import socket


def connect_udp(req_data, host, port):
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s.sendto(req_data, (host, port))
    resp_data, address = s.recvfrom(1024)

    return resp_data


def connect_tcp(req, host, port):
    pass


def connect(protocol, host, port, req_data):
    resp_data = b''

    if protocol == 'udp':
        resp_data = connect_udp(req_data, host, port)
    elif protocol == 'tcp':
        resp_data = connect_tcp(req_data, host, port)
    else:
        raise Exception('connection protocol does not match anything')

    return resp_data
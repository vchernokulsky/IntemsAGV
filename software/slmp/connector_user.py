from monitoring import Connection

HOST = '192.168.88.2'
PORT = 5002  # 5002 if 192.168.55.100
PROTOCOL = 'udp'

if __name__ == '__main__':
    connection = Connection(HOST, PORT, PROTOCOL)
    print(connection.read_connectors([1, 4]))
    #connection.write_to_connectors([(9, b'\x56\x34')])
    #print(connection.cash)
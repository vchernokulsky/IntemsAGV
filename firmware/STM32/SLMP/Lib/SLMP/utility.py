import struct

i = 2
bytes = struct.pack('>h', i)
print(bytes);
bytes = struct.pack('>d', i)
print(bytes);
i1 = struct.unpack('d', bytes)
print(*i1)
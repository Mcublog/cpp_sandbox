#!/usr/bin/env python3

import ctypes as ct
from dataclasses import dataclass

from cobs import cobsr


class CunkLL(ct.Structure):
    _fields_ = [
        ('channel',  ct.c_uint8),  # логический номер канала
        ('reserved', ct.c_uint8),  # резерв
        ('size',     ct.c_uint16), # количестов данных в чанке
        ('number',   ct.c_uint16), # номер чанка
        ('total',    ct.c_uint16), # всего чанков
        ('crc',      ct.c_uint32), # контрольная сумма payload
        # ('reserved', ct.c_uint8), # данные
    ]

@dataclass
class Chunk:
    channel: int
    size: int
    number:int
    total: int
    crc: int
    payload: bytes = b''

    def __repr__(self) -> str:
        return ''.join(f"Chunk(channel=0x{self.channel:x},"
                       f"size={self.size},"
                       f"number={self.number},"
                       f"total={self.total},"
                       f"crc=0x{self.crc:x}, payload={self.payload})")



    @staticmethod
    def from_buffer(data: bytes) -> 'Chunk':
        ll = CunkLL.from_buffer_copy(data)
        return Chunk(channel=ll.channel,
                     size=ll.size,
                     number=ll.number,
                     total=ll.total,
                     crc=ll.crc,
                     payload=data[12:])

def main():
    TEST_DATA = b'\x02\x11\x02\x06\x01\x01\x02\x01\x0b\xff\xff\xff\xff\x02 D000\x00\x02\x11\x02\n\x01\x01\x02\x01\x0f\xff\xff\xff\xff0\x1c95bfc1f\x1c\x00\x02\x11\x02\x03\x01\x01\x02\x01\x08\xff\xff\xff\xff\x035B\x00'

    chunks_raw = [c for c in TEST_DATA.split(b'\x00') if len(c)]
    for c in chunks_raw:
        print(c)
    chunks_raw = [cobsr.decode(c) for c in chunks_raw]
    chunk_raw = chunks_raw[0]
    print('-' * 10)
    print(chunk_raw)
    chunk = Chunk.from_buffer(chunk_raw)
    print(chunk)

if __name__ == "__main__":
    main()

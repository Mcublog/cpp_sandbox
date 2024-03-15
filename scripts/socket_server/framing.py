#!/usr/bin/env python3

import ctypes as ct
from dataclasses import dataclass

from cobs import cobs


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

CHUNK_FLOW_CONTROL_ACK = b'\x00'
MINIMAL_SIZE = ct.sizeof(CunkLL)
COBS_OVERHEAD = 3
PAYLOAD_MAX_IN_CHUNK = 128 - MINIMAL_SIZE - COBS_OVERHEAD

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

    @staticmethod
    def to_buffer(channel:int, data: bytes) -> list['Chunk']:
        payloads = [
            data[i:i + PAYLOAD_MAX_IN_CHUNK]
            for i in range(0, len(data), PAYLOAD_MAX_IN_CHUNK)
        ]
        chunks = []
        # total = 1 if len(payloads) <= PAYLOAD_MAX_IN_CHUNK else len(payloads)
        for i, p in enumerate(payloads):
            chunks.append(
                Chunk(channel=channel,
                      size=len(p),
                      number=i,
                      total=len(payloads),
                      crc=0x1234_4321,
                      payload=p))
        return chunks

    def to_bytes(self) -> bytes:
        return bytes(
            CunkLL(
                channel=self.channel,
                size=self.size,
                number=self.number,
                total=self.total,
                crc=self.crc,
            )) + self.payload

    def to_cobs(self) -> bytes:
        return cobs.encode(self.to_bytes()) + b'\x00'

def data_to_bytes(channel:int, data: bytes) -> bytes:
    output = b''
    for c in Chunk.to_buffer(channel, data):
        output += c.to_cobs()
    return output

def test_out():
    TEST_DATA = b'Hello World'
    LONG_DATA = b'Hello World' * 20

    chunks = Chunk.to_buffer(0x11, TEST_DATA)
    print(chunks)

    print('-' * 10)

    chunks = Chunk.to_buffer(0x11, LONG_DATA)
    print(chunks)

def test_in():
    TEST_DATA = b'\x02\x11\x02\x06\x01\x01\x02\x01\x0b\xff\xff\xff\xff\x02 D000\x00\x02\x11\x02\n\x01\x01\x02\x01\x0f\xff\xff\xff\xff0\x1c95bfc1f\x1c\x00\x02\x11\x02\x03\x01\x01\x02\x01\x08\xff\xff\xff\xff\x035B\x00'

    chunks_raw = [c for c in TEST_DATA.split(b'\x00') if len(c)]
    for c in chunks_raw:
        print(c)
    chunks_raw = [cobs.decode(c) for c in chunks_raw]
    chunk_raw = chunks_raw[0]
    print('-' * 10)
    print(chunk_raw)
    chunk = Chunk.from_buffer(chunk_raw)
    print(chunk)

def main():
    test_out()
    # test_in()

if __name__ == "__main__":
    main()

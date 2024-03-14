#!/usr/bin/env python3

import logging
import socket

import pylogus
import serial
import serial.tools.list_ports
from cobs import cobs
from framing import Chunk

log = pylogus.logger_init(__name__, logging.INFO)

def main():
    # DATA_OUT = b'\x02\x11\x02\x06\x01\x01\x02\x01\x0b\xff\xff\xff\xff\x02 D000\x00'
    payload = b'biba'
    chunk = Chunk(0x11, len(payload), 0, 1, 0x1234_5678, payload)
    with serial.Serial(port="/dev/ttyUSB0", baudrate=921_600, rtscts=True) as s:
        raw = chunk.to_bytes()
        chunk_cobs = cobs.encode(raw) + b'\x00'
        chunk_raw = cobs.decode(chunk_cobs[:-1])
        log.info(f"chunk_raw: {chunk_raw}")
        log.info(f"send: {chunk} => {raw} => {chunk_cobs}")
        s.write(chunk_cobs)



if __name__ == "__main__":
    main()

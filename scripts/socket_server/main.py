#!/usr/bin/env python3

import logging
import socket

import pylogus
import serial
import serial.tools.list_ports
from cobs import cobsr
from framing import Chunk

HOST = "127.0.0.1"  # Standard loopback interface address (localhost)
PORT = 9000  # Port to listen on (non-privileged ports are > 1023)

SERIAL_PORT = "/dev/ttyUSB0"
# SERIAL_PORT = "/dev/ttyACM0"
TIMEOUT_S = 8
BAUDRATE = 921_600

log = pylogus.logger_init(__name__, logging.DEBUG)


def main():
    for port in serial.tools.list_ports.comports():
        log.info(port)
    p = serial.Serial(port=SERIAL_PORT, baudrate=BAUDRATE)
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        log.info(f"Work on {p.name}")
        s.bind((HOST, PORT))
        s.listen()
        conn, addr = s.accept()
        with conn:
            log.info(f"Connected by {addr}")
            while True:
                data = conn.recv(1024 * 8)
                if not data:
                    break
                p.write(data)
                log.info(f"tx[{len(data)}]: {data}")
                mult = 10 if len(data) > 5000 else 1
                data = b''
                p.timeout = TIMEOUT_S
                while (r := p.read()) != b'':
                    if p.timeout == TIMEOUT_S:
                        p.timeout = 0.025 * mult
                    data += r
                if not data:
                    log.info("empty ack")
                    continue

                log.info(f"rx[{len(data)}]: {data}")
                for c in [cobsr.decode(c) for c in data.split(b'\x00') if len(c)]:
                    if len(c) == 1:
                        continue
                    chunk = Chunk.from_buffer(c)
                    conn.send(chunk.payload)
                # conn.send(data)
                # log.info(f"rx[{len(data)}]: {data}")


if __name__ == "__main__":
    while True:
        main()

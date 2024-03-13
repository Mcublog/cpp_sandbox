#!/usr/bin/env python3

import logging
import socket

import pylogus
import serial
import serial.tools.list_ports
from connector import HwConnector
from framing import Chunk

HOST = "127.0.0.1"  # Standard loopback interface address (localhost)
PORT = 9000  # Port to listen on (non-privileged ports are > 1023)

SERIAL_PORT = "/dev/ttyUSB0"
# SERIAL_PORT = "/dev/ttyACM0"
TIMEOUT_S = 8
BAUDRATE = 921_600

log = pylogus.logger_init(__name__, logging.INFO)




def main():
    connection: socket = None

    def on_chunks_getting(chunks: list[Chunk]):
        payloads = {0x00: b'', 0x01: b''}

        if connection is None:
            return
        for c in chunks:
            try:
                payloads[c.channel] += c.payload
            except Exception as e:
                log.error(e)

        if payloads[0]:
            print(payloads[0].decode(encoding='cp866', errors='ingnore'), end='')
        if payloads[1]:
            connection.send(payloads[1])
            log.info(f'to host[{len(payloads[1])}]: {payloads[1]}')

    for port in serial.tools.list_ports.comports():
        log.info(port)
    p = HwConnector()
    p.on_chunks_getting = on_chunks_getting

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        log.info(f"Work on {SERIAL_PORT}")
        s.bind((HOST, PORT))
        s.listen()
        conn, addr = s.accept()
        with conn:
            p.connect(SERIAL_PORT)
            connection = conn
            log.info(f"Connected by {addr}")
            while True:
                try:
                    data = conn.recv(1024 * 8)
                except Exception as e:
                    log.error(e)
                    break
                if not data:
                    break
                p.write(data)
                log.info(f"from host[{len(data)}]: {data}")
                continue

        log.info("Disconnected")
        connection = None
        p.disconnect()


if __name__ == "__main__":
    while True:
        main()

#!/usr/bin/env python3

import logging
import socket
import time
from dataclasses import dataclass, field
from threading import Event, Lock, Thread

import framing
import pylogus
from colorama import Fore as Clr
from connector import HwConnector
from framing import Chunk

HOST = "127.0.0.1"  # Standard loopback interface address (localhost)
PORT = 9000  # Port to listen on (non-privileged ports are > 1023)

SERIAL_PORT = "/dev/ttyUSB0"
# SERIAL_PORT = "/dev/ttyACM0"
TIMEOUT_S = 8
BAUDRATE = 921_600


@dataclass
class ServiceSocketConfig:
    channel: int
    host: str
    port: int

log = pylogus.logger_init(__name__, logging.INFO)

@dataclass
class ServiceConnection:
    config: ServiceSocketConfig
    kill_switch: Event = field(init=False, repr=False, default_factory=Event)
    _connection: socket.socket = field(init=False, repr=False, default=None)

    def _running(self):
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.bind((self.config.host, self.config.port))
            s.listen()
            s.settimeout(1)
            try:
                conn, addr = s.accept()
            except socket.timeout:
                # timeout try to restart
                return
            with conn:
                self._connection = conn
                log.info(f"Connected {conn} by {addr}")
                while not self.kill_switch.wait(0.001):
                    try:
                        data = conn.recv(1024 * 8)
                    except Exception as e:
                        log.error(e)
                        break
                    if not data:
                        break
                    log.info(f"from host[{len(data)}]: {data}")
                    raw = framing.data_to_bytes(self.config.channel, data)
                    self.send_to_channel(raw)
            self._connection = None
            log.info(f"disconnect {conn}")

    def run(self):
        while not self.kill_switch.wait(0.1):
            self._running()

    def kill(self):
        self.kill_switch.set()

    def send(self, data: bytes):
        if self._connection is None:
            return
        self._connection.send(data)

    def send_to_channel(self, data: bytes):
        pass

@dataclass
class KonnProxy:
    hwport: HwConnector
    configs: tuple[ServiceSocketConfig]
    connections: list[ServiceConnection] = field(
        init=False, repr=False, default_factory=list)
    thandles: list[Thread] = field(init=False, repr=False, default_factory=list)
    _portlock: Lock = field(init=False, repr=False, default_factory=Lock)

    def __post_init__(self):
        self.hwport.on_chunks_getting = self._on_to_host_data
        if not self.hwport.connect():
            log.error(f"{self.hwport} not connected")
            return

        for c in self.configs:
            conn = ServiceConnection(c)
            t = Thread(name=f"{conn}", target=conn.run)
            conn.send_to_channel = self._on_from_host_data
            self.connections.append(conn)
            self.thandles.append(t)
            t.start()
        for conn in self.connections:
            log.info(f"{Clr.LIGHTBLUE_EX}Serving on {conn.config}{Clr.RESET}")

    def disconnect(self):
        for c, t in zip(self.connections, self.thandles):
            c.kill()
            t.join()

    def _on_from_host_data(self, data: bytes):
        # with self._portlock:
        self.hwport.write(data)

    def _on_to_host_data(self, chunks: list[Chunk]):
        # with self._portlock:
        payloads = {}
        for conn in self.connections:
            payloads.update({conn.config.channel: b''})
        for c in chunks:
            for conn in self.connections:
                if conn.config.channel != c.channel:
                    continue
                payloads[c.channel] += c.payload
        for k, v in payloads.items():
            if v == b'':
                continue
            for conn in self.connections:
                if conn.config.channel != k:
                    continue
                conn.send(v)
                if conn.config.channel == 1:
                    if log.level > logging.DEBUG:
                        return
                    print(v.decode(encoding='cp866', errors='ingnore'),
                          end='')
                log.info(f"  to host[{len(v)}]: {v}")


def main():
    configs = (ServiceSocketConfig(channel=0, host="127.0.0.1", port=9000),
               ServiceSocketConfig(channel=1, host="127.0.0.1", port=9001))
    konn = KonnProxy(hwport=HwConnector(SERIAL_PORT), configs=configs)
    try:
        while True:
            time.sleep(0.5)
    except KeyboardInterrupt as e:
        log.error(e)
    log.info("Kill all")
    konn.disconnect()

if __name__ == "__main__":
    main()

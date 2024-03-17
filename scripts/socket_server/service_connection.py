import logging
import socket
from dataclasses import dataclass, field
from threading import Event

import framing
import pylogus

log = pylogus.logger_init(__name__, logging.INFO)


@dataclass
class ServiceSocketConfig:
    channel: int
    host: str
    port: int


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
        log.info(f"  to host[{len(data)}]: {data}")
        self._connection.send(data)

    def send_to_channel(self, data: bytes):
        pass

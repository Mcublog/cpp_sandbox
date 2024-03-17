import logging
import socket
from dataclasses import dataclass, field
from threading import Event, Lock, Thread

import framing
import pylogus
from colorama import Fore as Clr
from connector import HwConnector
from framing import Chunk
from service_connection import ServiceConnection, ServiceSocketConfig

log = pylogus.logger_init(__name__, logging.INFO)


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
        log.debug(f"{self.hwport.name} write: {data}")
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

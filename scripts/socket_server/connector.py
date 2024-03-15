#!/usr/bin/env python3


import logging
import queue
import time
from dataclasses import dataclass, field
from enum import Enum, auto
from threading import Event, Thread

import pylogus
from cobs import cobsr
from framing import CHUNK_FLOW_CONTROL_ACK, Chunk
from serial import Serial

log = pylogus.logger_init(__name__, logging.INFO)

DEFAULT_BAUDRATE = 921_600


class HwConnectorErrors(Enum):
    OK = auto()
    ERROR = auto()
    TIMEOUT = auto()
    SERIAL_ERROR = auto()
    DISCONNECT = auto()


@dataclass
class HwConnector:
    name: str = ""
    port: Serial = field(init=False, default=None)
    writeq: queue.Queue = field(init=False, repr=False, default=queue.Queue())

    _error: HwConnectorErrors = field(init=False,
                                      repr=False,
                                      default=HwConnectorErrors.OK)
    _kill_evt: Event = field(init=False, repr=False, default=Event())
    _poll_thread: Thread = field(init=False, repr=False)

    @staticmethod
    def _port_is_free(portname: str) -> bool:
        try:
            _ = Serial(
                portname,
                baudrate=DEFAULT_BAUDRATE,
            )
            return True
        except Exception as e:
            log.error(e)
            return False

    @staticmethod
    def _queue_clear(q: queue.Queue):
        while not q.empty():
            try:
                q.get(block=False)
            except queue.Empty:
                continue
            q.task_done()

    def _read(self) -> bytes:
        data = b''
        while (r := self.port.read()) != b'':
            data += r
        return data

    def _polling(self, kill_evt: Event):
        raw = b''
        while not kill_evt.wait(0.001):
            while not self.writeq.empty():
                data = self.writeq.get_nowait()
                for c in data.split(b'\x00'):
                    if c == b'':
                        continue
                    log.debug(f"send bytes {c}")
                    c += b'\x00'
                    self.port.write(c)
                    for _ in range(5):
                        if (ack := self.port.read_until(b'\x00')) == b'':
                            time.sleep(0.001)
                        log.debug(f"getting {ack}")
                        break
            try:
                raw += self._read()
            except Exception as e:
                log.error(e)
                return HwConnectorErrors.SERIAL_ERROR
            if raw == b'':
                continue
            if raw.find(b'\x00') == -1:
                continue
            rem = b''
            chunks = []
            for c in [cobsr.decode(c) for c in raw.split(b'\x00') if len(c)]:
                try:
                    chunks.append(Chunk.from_buffer(c))
                except Exception as e:
                    log.info(e)
                    log.info(c)
                    rem += c
            if chunks:
                self.on_chunks_getting(chunks=chunks)
            raw = rem

    def connect(self, portname: str = "") -> bool:
        if portname:
            self.name = portname
        if not HwConnector._port_is_free(self.name):
            log.error(f"{self.name} is busy")
            return False
        try:
            self.port = Serial(self.name,
                               baudrate=DEFAULT_BAUDRATE,
                               rtscts=True,
                               timeout=0.015)
        except Exception as e:
            log.error(f"{e}")
            return False
        self.writeq = queue.Queue()
        self._kill_evt = Event()
        self._poll_thread = Thread(name=f"poll: {self.name}",
                                   target=self._polling,
                                   args=([self._kill_evt]),
                                   daemon=True)
        self._poll_thread.start()
        return True

    def disconnect(self) -> bool:
        self.writeq = queue.Queue()
        self._kill_evt.set()
        try:
            self._poll_thread.join()
        except Exception as e:
            log.warning(e)
        try:
            self.port.close()
        except Exception as e:
            log.warning(e)
        self._poll_thread = None
        self._error = HwConnectorErrors.DISCONNECT
        return True

    def write(self, data: bytes):
        self.writeq.put(data)

    def on_chunks_getting(self, chunks: list[Chunk]):
        log.info(chunks)

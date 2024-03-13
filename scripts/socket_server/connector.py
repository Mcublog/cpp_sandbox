#!/usr/bin/env python3


import logging
import queue
from dataclasses import dataclass, field
from enum import Enum, auto
from threading import Event, Lock, Thread

import pylogus
from cobs import cobsr
from framing import Chunk
from serial import Serial

log = pylogus.logger_init(__name__, logging.INFO)

DEFAULT_BAUDRATE = 921_600
TIMEOUT_S = 8

class HwConnectorErrors(Enum):
    OK = auto()
    ERROR = auto()
    TIMEOUT = auto()
    SERIAL_ERROR = auto()
    DISCONNECT = auto()

@dataclass
class HwConnector:
    port: Serial = field(init=False)
    writeq: queue.Queue = field(init=False, repr=False, default=queue.Queue())

    _error: HwConnectorErrors = field(
        init=False, repr=False, default=HwConnectorErrors.OK)
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
                self.port.write(data)
            try:
                raw += self._read()
            except Exception as e:
                log.error(e)
                return HwConnectorErrors.SERIAL_ERROR
            if raw == b'':
                continue
            if raw.find(b'\x00') == -1:
                continue
            # remainder = raw[raw.rindex('\x00'):]
            # log.info(raw)
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
            # raw = raw[raw.rindex(b'\x00'):]



    def connect(self, portname: str) -> bool:
        if not HwConnector._port_is_free(portname):
            log.error(f"{portname} is busy")
            return False
        try:
            self.port = Serial(
                portname,
                baudrate=DEFAULT_BAUDRATE,
                timeout=0.015
            )
        except Exception as e:
            log.error(f"{e}")
            return False
        self.writeq = queue.Queue()
        self._kill_evt = Event()
        self._poll_thread = Thread(name=f"poll: {portname}",
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

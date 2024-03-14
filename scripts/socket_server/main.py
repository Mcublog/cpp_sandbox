#!/usr/bin/env python3

import logging
import socket
import time
from dataclasses import dataclass

import pylogus
import serial.tools.list_ports
from connector import HwConnector
from konn_proxy import KonnProxy, ServiceSocketConfig

SERIAL_PORT = "/dev/ttyUSB0"
# SERIAL_PORT = "/dev/ttyACM0"
log = pylogus.logger_init(__name__, logging.INFO)


def main():
    for port in serial.tools.list_ports.comports():
        log.info(port)

    configs = (ServiceSocketConfig(channel=1, host="127.0.0.1", port=9000),
               ServiceSocketConfig(channel=0, host="127.0.0.1", port=9001))
    konn = KonnProxy(hwport=HwConnector(SERIAL_PORT), configs=configs)

    try:
        while True:
            time.sleep(0.5)
    except KeyboardInterrupt as e:
        log.error(e)
    konn.disconnect()
    log.info("Kill all")


if __name__ == "__main__":
    main()

#!/usr/bin/env python3

import logging
import time

import pylogus
import serial.tools.list_ports
from colorama import Fore as Clr
from connector import HwConnector
from konn_proxy import KonnProxy, ServiceSocketConfig

SERIAL_PORT = "/dev/ttyUSB0"

log = pylogus.logger_init(__name__, logging.INFO)


def main():
    for port in serial.tools.list_ports.comports():
        log.info(port)

    configs = (ServiceSocketConfig(channel=0, host="192.168.0.111", port=9000),
               ServiceSocketConfig(channel=1, host="192.168.0.111", port=9001),
               ServiceSocketConfig(channel=2, host="192.168.0.111", port=9002))
    konn = KonnProxy(hwport=HwConnector(SERIAL_PORT), configs=configs)

    try:
        while True:
            time.sleep(0.5)
    except KeyboardInterrupt:
        pass
    konn.disconnect()
    log.info(f"{Clr.YELLOW}Kill all{Clr.RESET}")


if __name__ == "__main__":
    main()

#!/bin/env python3

import serial

POERTNAME = "/dev/ttyS11"

def main():
    with serial.Serial(POERTNAME, timeout=1, rtscts=True) as ser:
        ser.write(b"Hello")
        print("sended")


if __name__ == "__main__":
    main()
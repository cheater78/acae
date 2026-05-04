#!/bin/python3
# NOTE: This only works if the chip has this projects baremetal exit() implementation!
import serial
import time

port = "/dev/ttyUSB0"
baud = 115200

ser = serial.Serial(port, 115200)
try:
    ser.write(b"RSTDFU\n")
except:
    print("Entering DFU failed!")
finally:
    ser.close()
    time.sleep(2) # give MCU some time to reset
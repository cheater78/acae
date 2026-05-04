#!/bin/python3
import serial
import sys

port = "/dev/ttyUSB0"
baud = 115200

ser = serial.Serial(port, 115200)

print(f"Listening for serial input on {port}, baud: {baud}, closing when recieving \"EXIT\" or cancel with Ctrl+C.")

try:
    while True:
        raw_data = ser.readline()
        
        line = ""
        try:
            line = raw_data.decode('utf-8').strip()
            print(f"{line}")
        except UnicodeDecodeError:
            hex_line: str = ""
            for byte in raw_data:
                if chr(byte).isprintable():
                    hex_line += chr(byte)
                else:
                    hex_line +=f'{byte:02X}'
            print(f"HEX8>: {hex_line}")
        
        if line == "EXIT":
            print("Received 'EXIT' command. Closing connection...")
            break
except KeyboardInterrupt:
    print("\nInterrupted by user (Ctrl+C). Closing connection...")
finally:
    ser.close()
    print("Serial connection closed.")
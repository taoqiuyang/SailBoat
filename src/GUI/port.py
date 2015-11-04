#!/usr/bin/python

import serial

ser = serial.Serial(
    port='COM6',\
    baudrate=9600,\
    parity=serial.PARITY_NONE,\
    stopbits=serial.STOPBITS_ONE,\
    bytesize=serial.EIGHTBITS,\
    timeout=1)

print("connected to: " + ser.portstr)

while True:
	line = ser.readline()
	print(line)
	input = raw_input("enter your input: ")
	ser.write(input)

ser.close()

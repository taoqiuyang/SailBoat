
import serial

def index(req,data):
	''' uncomment this block give me 500 server error
	ser = serial.Serial(
	    port='/dev/ttyACM0',\
	    baudrate=9600,\
	    parity=serial.PARITY_NONE,\
	    stopbits=serial.STOPBITS_ONE,\
	    bytesize=serial.EIGHTBITS,\
	    timeout=1)

	ser.open()
	line = ser.write('R')

	ser.close()
	'''
	return data;

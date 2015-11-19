import sys
import serial
import time
from PyQt4 import QtGui, QtCore
from PyQt4.QtCore import *
from PyQt4.QtGui import *
from PyQt4.QtWebKit import *


class MyThread(QtCore.QThread):
	trigger = QtCore.pyqtSignal(int)
	def __init__(self, parent=None):
		super(MyThread, self).__init__(parent)
	
	def run(self):
		while 1:
			time.sleep(5)  # random sleep to imitate working
			self.trigger.emit(1)

class Window(QWidget):
	def __init__(self):
		super(Window, self).__init__()
		self.setGeometry(50, 50, 750, 800)
		self.setWindowTitle("Sailboat")
		
		self.ser = serial.Serial(
			port='COM6',\
			baudrate=9600,\
			parity=serial.PARITY_NONE,\
			stopbits=serial.STOPBITS_ONE,\
			bytesize=serial.EIGHTBITS,\
			timeout=1)
			
		self.create_widgets()
		self.start_threads()
		
	def create_widgets(self):
		self.browser = QWebView()
		self.browser.load(QUrl("index.html"))
		self.browser.page().mainFrame().addToJavaScriptWindowObject('self', self) # bind the html with python
		
		'''
		self.textarea = QTextEdit()
		self.textarea.setReadOnly(True)
		self.textarea.setText("asdasd")
		self.textarea.resize(100, 100)
		'''
		grid = QGridLayout()
		grid.addWidget(self.browser, 2, 0)
		#grid.addWidget(self.textarea, 1, 0)
		self.setLayout(grid)
		self.show()
	
	# this function will be called by javascript
	@pyqtSlot(float, float, float)
	def send_to_boat_path(self, lat, lng, id):
		print ("@SET=PATH, " + str(lat) + ", " + str(lng)+ ", " + str(id) + "#")
		self.ser.write("@SET=PATH, " + str(lat) + ", " + str(lng)+ ", " + str(id) + "#")
	
	
	# this function will be called by javascript
	@pyqtSlot(float, float)
	def send_to_boat_servo(self, servo, degree):
		if servo == 0.0:
			servo = "RUDDER"
		else:
			servo = "WING"
		print ("@SET=SERVO, " + str(servo) + ", " + str(degree) + "#")
		self.ser.write("@SET=SERVO, " + str(servo) + ", " + str(degree) + "#")
		
	def start_threads(self):
		thread = MyThread(self)    # create a thread
		thread.trigger.connect(self.update_info)  # connect to it's signal
		thread.start()             # start the thread
		self.thread = thread

	def update_info(self):
		
		self.ser.write("@GET=IMU_Y#")
		imu_y = ""
		while not imu_y:
			imu_y = self.ser.readline()
		
		self.ser.write("@GET=IMU_P#")
		imu_p = ""
		while not imu_p:
			imu_p = self.ser.readline()
		
		self.ser.write("@GET=IMU_R#")
		imu_r = ""
		while not imu_r:
			imu_r = self.ser.readline()
		
		self.ser.write("@GET=GPS_Quality#")
		gps_quality = ""
		while not gps_quality:
			gps_quality = self.ser.readline()
			
		self.ser.write("@GET=GPS_Latitude#")
		lat = ""
		while not lat:
			lat = self.ser.readline()
		
		self.ser.write("@GET=GPS_Longitude#")
		lng = ""
		while not lng:
			lng = self.ser.readline()
		
		self.ser.write("@GET=GPS_Altitude#")
		alt = ""
		while not alt:
			alt = self.ser.readline()
		
		'''
		self.textarea.clear()
		self.textarea.append(content)
		'''
		#command = "update_GPS_table1('5')"
		#command = "update_GPS_table(" + '\'' + imu_y + '\'' + ',\'' + imu_p + '\'' + ',\'' + imu_r + '\'' + ")"
		command = "update_GPS_table(" + imu_y + "," + imu_p + "," + imu_r + ")"
		#command = "update_GPS_table(" + imu_y + "," + imu_p + "," + imu_r + "," + gps_quality + "," + lat + "," + lng + "," + alt + ")"
	
		#command = 'update_GPS_table(' + "\"" + imu_y + "\"," + "\"" + imu_p + "\"," + "\"" + imu_r + "\"," + "\"" + gps_quality + "\"," + "\"" + lat + "\"," + "\"" + lng + "\"," + "\"" + alt + "\""')'
		print command
		frame = self.browser.page().currentFrame()
		frame.evaluateJavaScript(command)
		
		
app = QtGui.QApplication(sys.argv)
Gui = Window()
sys.exit(app.exec_())
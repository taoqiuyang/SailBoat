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

#evaluateJavaScript cannot parse string variable
NAdouble = "3000"
	
class Window(QWidget):
	def __init__(self):
		super(Window, self).__init__()
		self.setGeometry(50, 50, 750, 800)
		self.setWindowTitle("Sailboat")
		
		self.ser = serial.Serial(
			port='COM4',\
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
		global NAdouble

		self.ser.write("@GET=IMU_Y#")
		imu_y = ""
		while not imu_y:
			imu_y = self.ser.readline()
			imu_y = self.parse_double(imu_y)
			
		self.ser.write("@GET=IMU_P#")
		imu_p = ""
		while not imu_p:
			imu_p = self.ser.readline()
			imu_p = self.parse_double(imu_p)
			
		self.ser.write("@GET=IMU_R#")
		imu_r = ""
		while not imu_r:
			imu_r = self.ser.readline()
			imu_r = self.parse_double(imu_r)
					
		self.ser.write("@GET=GPS_Quality#")
		gps_quality = ""
		while not gps_quality:
			gps_quality = self.ser.readline()
			gps_quality = self.parse_double(gps_quality)

		self.ser.write("@GET=GPS_UTC#")
		gps_utc = ""
		while not gps_utc:
			gps_utc = self.ser.readline()
			gps_utc = self.parse_double(gps_utc)
			if (gps_utc==NAdouble):
				gps_utc_flag = "0"
				gps_utc_hour = "0"
				gps_utc_minute = "0"
				gps_utc_second = "0"
			else:
				gps_utc_flag = "1"
				gps_utc = str(gps_utc)
				gps_utc_hour = gps_utc[0:2]
				gps_utc_minute = gps_utc[2:4]
				gps_utc_second = gps_utc[4:6]
			
		self.ser.write("@GET=GPS_Latitude#")
		lat = ""
		while not lat:
			lat = self.ser.readline()
			lat = self.parse_double(lat)
		
		self.ser.write("@GET=GPS_Longitude#")
		lng = ""
		while not lng:
			lng = self.ser.readline()
			lng = self.parse_double(lng)
						
		self.ser.write("@GET=GPS_Altitude#")
		alt = ""
		while not alt:
			alt = self.ser.readline()
			alt = self.parse_double(alt)

		self.ser.write("@GET=GPS_Num_Satellite#")
		num_satellite = ""
		while not num_satellite:
			num_satellite = self.ser.readline()
			num_satellite = self.parse_double(num_satellite)

		self.ser.write("@GET=GPS_HDOP#")
		hdop = ""
		while not hdop:
			hdop = self.ser.readline()
			hdop = self.parse_double(hdop)
			
		self.ser.write("@GET=GPS_VDOP#")
		vdop = ""
		while not vdop:
			vdop = self.ser.readline()
			vdop = self.parse_double(vdop)

		self.ser.write("@GET=GPS_PDOP#")
		pdop = ""
		while not pdop:
			pdop = self.ser.readline()
			pdop = self.parse_double(pdop)

		self.ser.write("@GET=GPS_Date#")
		date = ""
		while not date:
			date = self.ser.readline()
			date = self.parse_double(date)
			
		self.ser.write("@GET=GPS_VelocityKnot#")
		v_knot = ""
		while not v_knot:
			v_knot = self.ser.readline()
			v_knot = self.parse_double(v_knot)

		self.ser.write("@GET=GPS_VelocityKph#")
		v_kph = ""
		while not v_kph:
			v_kph = self.ser.readline()
			v_kph = self.parse_double(v_kph)			
		'''
		self.textarea.clear()
		self.textarea.append(content)
		'''
		#command = "update_GPS_table1('5')"
		#command = "update_GPS_table(" + '\'' + imu_y + '\'' + ',\'' + imu_p + '\'' + ',\'' + imu_r + '\'' + ")"
		command = "update_GPS_table(" + imu_y + "," + imu_p + "," + imu_r + "," + gps_quality + "," \
							+ gps_utc_flag + "," + gps_utc_hour + "," + gps_utc_minute + "," + gps_utc_second + "," \
							+ lat + "," + lng + "," + alt + "," + num_satellite + "," \
							+ hdop + "," + vdop + "," + pdop + "," + date + "," + v_knot + "," + v_kph + "," + NAdouble + ")"
		#command = "update_GPS_table(" + imu_y + "," + imu_p + "," + imu_r + "," + gps_quality + "," + lat + "," + lng + "," + alt + ")"
	
		#command = 'update_GPS_table(' + "\"" + imu_y + "\"," + "\"" + imu_p + "\"," + "\"" + imu_r + "\"," + "\"" + gps_quality + "\"," + "\"" + lat + "\"," + "\"" + lng + "\"," + "\"" + alt + "\""')'
		
		print command
		frame = self.browser.page().currentFrame()
		frame.evaluateJavaScript(command)
	
		#The following code will update boat's location on google map
		command = "show_GPS_pos(" + lat + "," + lng + ")"
		frame.evaluateJavaScript(command)
		
		
	#evaluateJavaScript cannot parse string variable
	def parse_double(self, str):
		global NAdouble
		if(str=="N/A\n"):
			return NAdouble
		return str
		
		
app = QtGui.QApplication(sys.argv)
Gui = Window()
sys.exit(app.exec_())
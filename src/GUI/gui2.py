from Tkinter import *
from PIL import ImageTk, Image
import os
import serial
import time

class Coordinate:
	def __init__(self, lat, lng):
		self.lat = lat
		self.lng = lng
		
class App(Frame):
	def __init__(self, master):
		Frame.__init__(self,master)
		self.grid()
		self.waypoints = []
		self.ser = serial.Serial(
			port='COM2',\
			baudrate=9600,\
			parity=serial.PARITY_NONE,\
			stopbits=serial.STOPBITS_ONE,\
			bytesize=serial.EIGHTBITS,\
			timeout=1)
		self.create_widgets()
		self.d = 1
		
	def create_widgets(self):
		################# lat and lng label #################
		self.lat_label = Label(self, text = "Latitude:")
		self.lat_label.grid(row = 0, column = 0, columnspan = 4, sticky = W)

		self.lat_entry = Entry(self)
		self.lat_entry.grid(row = 0, column = 1, sticky = W) 

		self.lng_label = Label(self, text = "Longitude:")
		self.lng_label.grid(row = 0, column = 2, sticky = W)

		self.lng_entry = Entry(self)
		self.lng_entry.grid(row = 0, column = 3, sticky = W)

		################### end of lat/lng ####################


		################# add google map image #################### 
		self.photo = PhotoImage(file="img.gif")
		self.img = Label(root, image = self.photo)
		self.img.grid()
		self.img.bind('<ButtonPress>', lambda event:self.mouse_press(event))

		################# End #################### 


		########### send lat and lng  ##################
		self.add_button = Button(self, text = "Add", command = self.add)
		self.add_button.grid(row = 0, column = 4, sticky = W) 

		self.delete_button = Button(self, text = "Delete", command = self.delete)
		self.delete_button.grid(row = 0, column = 5, sticky = W) 


		self.send_info_button = Button(self, text = "Send info to boat", command = self.send_info)
		self.send_info_button.grid(row = 2, column = 0, sticky = W) 

		self.get_boat_info_button = Button(self)
		self.get_boat_info_button.config(text = "get info")
		self.get_boat_info_button["command"] = self.update
		self.get_boat_info_button.grid(row = 2, column = 3, sticky = W)


		self.waypoints_info = Text(self, width = 35, height = 5, wrap = WORD)
		self.waypoints_info.grid(row = 3, column = 0, columnspan = 3, sticky = W) 

		self.boat_info = Text(self, width = 35, height = 5, wrap = WORD)
		self.boat_info.grid(row = 3, column = 3, sticky = W) 
	
	def send_info(self):
		counter = 1
		if self.waypoints:
			for point in self.waypoints:
				if point:
					new_str = point.lat + " " + point.lng + "\n"
				else:
					new_str = ""
				counter += 1
				self.ser.write(new_str)
				time.sleep(0.7)
	
	def update(self):
		'''
		ser = serial.Serial(
		port='COM2',\
		baudrate=9600,\
		parity=serial.PARITY_NONE,\
		stopbits=serial.STOPBITS_ONE,\
		bytesize=serial.EIGHTBITS,\
		timeout=1)
		'''
		self.ser.write("X")
		
		content = ""
		while not content:
			content = self.ser.readline()
		self.boat_info.delete(0.0, END)		
		self.boat_info.insert(0.0, content)

	def test(self):
		
		self.ser.write("X")
		
		content = ""
		while not content:
			content = self.ser.readline()
		
		self.boat_info.delete(0.0, END)	
		self.boat_info.insert(0.0, content)

		self.after(5000, self.test)
	
	def mouse_press(self, event):
		length = 700.0
		height = 600.0
		x = event.x
		y = event.y
		top_x_coord = 30.0
		top_y_coord = 30.0
		bottom_x_coord = 50.0
		bottom_y_coord = 50.0
		
		final_x = top_x_coord + 20.0 * (x/length)
		final_y = top_y_coord + 20.0 * (y/height)
		
		final_x = float("{0:.4f}".format(final_x))
		final_y = float("{0:.4f}".format(final_y))
	
		self.lat_entry.delete(0, END)
		self.lat_entry.insert(0, final_x)
		
		self.lng_entry.delete(0, END)
		self.lng_entry.insert(0, final_y)
		
	def print_waypoints(self):	
		f_str = ""
		counter = 1
		for point in self.waypoints:
			if point:
				new_str = str(counter) + ". lat: " + point.lat + " lng: " + point.lng + "\n"
			else:
				new_str = str(counter) + ".\n"
			counter += 1
			f_str += new_str
		
		self.waypoints_info.delete(0.0, END)	
		self.waypoints_info.insert(0.0, f_str)

	def add(self):
		point = Coordinate(self.lat_entry.get(), self.lng_entry.get())
		self.waypoints.append(point)
		self.print_waypoints()
		
	def delete(self):
		if self.waypoints:
			self.waypoints.pop()
		self.print_waypoints()
		
		
root = Tk()
root.title("asas")
root.geometry("800x800")

app = App(root)
app.test()

root.mainloop()
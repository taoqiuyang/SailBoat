from Tkinter import *
import os
import serial
import time

def print_waypoints():	
	f_str = ""
	counter = 1
	for point in waypoints:
		if point:
			new_str = str(counter) + ". lat: " + point.lat + " lng: " + point.lng + "\n"
		else:
			new_str = str(counter) + ".\n"
		counter += 1
		f_str += new_str
	
	waypoints_info.delete(0.0, END)	
	waypoints_info.insert(0.0, f_str)

def add():
	point = Coordinate(lat_entry.get(), lng_entry.get())
	waypoints.append(point)
	print_waypoints()
	
def delete():
	if waypoints:
		waypoints.pop()
	print_waypoints()


def send_info():
	ser = serial.Serial(
	port='COM2',\
	baudrate=9600,\
	parity=serial.PARITY_NONE,\
	stopbits=serial.STOPBITS_ONE,\
	bytesize=serial.EIGHTBITS,\
	timeout=1)
	
	counter = 1
	if waypoints:
		for point in waypoints:
			if point:
				new_str = point.lat + " " + point.lng + "\n"
			else:
				new_str = ""
			counter += 1
			ser.write(new_str)
			time.sleep(0.7)
	
	ser.close()

def mouse_press(event):
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
	
	lat_entry.delete(0, END)
	lat_entry.insert(0, final_x)
	
	lng_entry.delete(0, END)
	lng_entry.insert(0, final_y)

def update():
	ser = serial.Serial(
	port='COM2',\
	baudrate=9600,\
	parity=serial.PARITY_NONE,\
	stopbits=serial.STOPBITS_ONE,\
	bytesize=serial.EIGHTBITS,\
	timeout=1)
	
	ser.write("X")
	
	content = ""
	while not content:
		content = ser.readline()
		
	boat_info.delete(0.0, END)		
	boat_info.insert(0.0, content)
	ser.close()

class Coordinate:
	def __init__(self, lat, lng):
		self.lat = lat
		self.lng = lng
		
		
#Create the window
root = Tk()

root.title("Sailboat")
root.geometry("800x800")

app = Frame(root)
app.grid()

waypoints = []

################# lat and lng label #################
lat_label = Label(app, text = "Latitude:")
lat_label.grid(row = 0, column = 0, columnspan = 4, sticky = W)

lat_entry = Entry(app)
lat_entry.grid(row = 0, column = 1, sticky = W) 

lng_label = Label(app, text = "Longitude:")
lng_label.grid(row = 0, column = 2, sticky = W)

lng_entry = Entry(app)
lng_entry.grid(row = 0, column = 3, sticky = W)

################### end of lat/lng ####################


################# add google map image #################### 
photo = PhotoImage(file="img.gif")
img = Label(root, image = photo)
img.grid()
img.bind('<ButtonPress>', mouse_press)

################# End #################### 


########### send lat and lng  ##################
add_button = Button(app, text = "Add", command = add)
add_button.grid(row = 0, column = 4, sticky = W) 

delete_button = Button(app, text = "Delete", command = delete)
delete_button.grid(row = 0, column = 5, sticky = W) 


send_info_button = Button(app, text = "Send info to boat", command = send_info)
send_info_button.grid(row = 2, column = 0, sticky = W) 

get_boat_info_button = Button(app)
get_boat_info_button.config(text = "get info")
get_boat_info_button["command"] = update
get_boat_info_button.grid(row = 2, column = 3, sticky = W)


waypoints_info = Text(app, width = 35, height = 5, wrap = WORD)
waypoints_info.grid(row = 3, column = 0, columnspan = 3, sticky = W) 

boat_info = Text(app, width = 35, height = 5, wrap = WORD)
boat_info.grid(row = 3, column = 3, sticky = W) 



root.mainloop()
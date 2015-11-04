from Tkinter import *
from PIL import ImageTk, Image
import os
import serial

class App(Frame):
	def __init__(self, master):
		Frame.__init__(self,master)
		self.grid()
		
		self.create_widgets()
		
	def create_widgets(self):
		# lat and lng label
		self.instruction = Label(self, text = "Enter the")
		self.instruction.grid(row = 0, column = 0, columnspan = 2, sticky = W)
		
		self.password = Entry(self)
		self.password.grid(row = 0, column = 1, sticky = W) 
		
		# send lat and lng
		self.submit = Button(self, text = "submit", command = self.reveal)
		self.submit.grid(row = 2, column = 0, sticky = W) 
		
		self.text = Text(self, width = 35, height = 5, wrap = WORD)
		self.text.grid(row = 3, column = 0, columnspan = 2, sticky = W) 
		
		# add image 
		photo = PhotoImage(file="img.gif")
		self.img = Label(root, image = photo)
		self.photo = photo
		self.img.grid()
		self.img.bind('<ButtonPress>', mouse_press)
		
		self.button = Button(self, text = "text1")
		self.button.config(text = "get info")
		self.button["command"] = self.update
		self.button.grid()
	
	def reveal(self):
		content = self.password.get()
		
		ser = serial.Serial(
		port='COM2',\
		baudrate=9600,\
		parity=serial.PARITY_NONE,\
		stopbits=serial.STOPBITS_ONE,\
		bytesize=serial.EIGHTBITS,\
		timeout=1)
		ser.write(content)
		ser.close()
		'''
		if content == "password":
			message = "asdads"
		else:
			message = "aaaaa"
		
		self.text.delete(0.0, END)		
		self.text.insert(0.0, message)
		'''
	
	def update(self):
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
		self.text.delete(0.0, END)		
		self.text.insert(0.0, content)
		ser.close()

def mouse_press(event):
		print('type: {}'.format(event.type))
		print('widget: {}'.format(event.widget))
		print('num: {}'.format(event.num)) 
		print('x: {}'.format(event.x))
		print('y: {}'.format(event.y))
		print('x_root: {}'.format(event.x_root))
		print('y_root: {}\n'.format(event.y_root))
		
root = Tk()
root.title("asas")
root.geometry("800x800")

app = App(root)

root.mainloop()
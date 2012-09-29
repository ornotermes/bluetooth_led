#!/usr/bin/env python
#
#+	Copyright (c) 2011-2012 Rikard Lindstrom <ornotermes@gmail.com>
#
#	This program is free software: you can redistribute it and/or modify
#	it under the terms of the GNU General Public License as published by
#	the Free Software Foundation, either version 3 of the License, or
#	(at your option) any later version.
#
#	This program is distributed in the hope that it will be useful,
#	but WITHOUT ANY WARRANTY; without even the implied warranty of
#	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#	GNU General Public License for more details.
#
#	You should have received a copy of the GNU General Public License
#	along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

import pygtk
pygtk.require('2.0')
import gtk
import bluetooth
		
class ColorPicker:
	
	def delete_event(self, widget, event, data=None):
		return False;
   
	def destroy(self, widget, data=None):
		gtk.main_quit()
  
	def colorUpdate(self, widget):
		gdk_color = self.colorpick.get_current_color()
		red = gdk_color.red >> 8
		green = gdk_color.green >> 8
		blue = gdk_color.blue >> 8
		hex_color = "#%(red)02X%(green)02X%(blue)02X" % {"red": red, "green": green, "blue": blue}
		print (hex_color)
		bs.tx(hex_color)
		
	def selectDev(self, widget, data=None):
		bs.disconnect();
		'''Disconnect any connected devices before connecting'''
		
		dev = devs[self.devlist.get_active()]
		devtype = dev[0]
		devid = dev[1]
		if(devtype == "b"):
			print ("Connecting to bluetooth device:")
			print (devid)
			bs.connect( devid, 1 )
		
    
	def __init__(self):
		self.window = gtk.Window(gtk.WINDOW_TOPLEVEL)
		self.window.connect("delete_event", self.destroy)
		self.window.set_border_width(10)
		
		self.devlist = gtk.combo_box_new_text()
		self.devlist.padding = 8
		self.devlist.connect("changed", self.selectDev)
		self.devlist.set_title("Select device here.")
		self.devlist.show()
		
		self.colorpick = gtk.ColorSelection()
		self.colorpick.connect("color-changed", self.colorUpdate)
		self.colorpick.show()
		
		self.mainbox = gtk.VBox()
		self.mainbox.items = 2
		self.mainbox.add(self.devlist)
		self.mainbox.add(self.colorpick)
		self.mainbox.show()
	
		self.window.add(self.mainbox)
		self.window.show()
	
		self.devlist.append_text("Disconnected")
		devs.append(["none", ""])
		self.devlist.set_active(0)
		for addr, name in bs.scan():
			self.devlist.append_text("Bluetooth: %(addr)s (%(name)s)" % {"addr": addr, "name": name})
			devs.append(["b", addr])

		
	def main(self):
		gtk.main()
		
class BlueSerial:

	def __init__(self):
		self.b = bluetooth.BluetoothSocket ( bluetooth.RFCOMM )
		
	def __exit__(self, type, value, traceback):
		self.disconnect()
		
	def scan(self):
		return bluetooth.discover_devices(lookup_names=True)
		
	def connect(self, mac, port):
		self.b = bluetooth.BluetoothSocket ( bluetooth.RFCOMM )
		self.b.connect ((mac, port))
	
	def disconnect(self):
		self.b.close()

	def tx(self, data):
		self.b.send(data)
		
	def rx():
		pass
		
if __name__ == '__main__':
	devs = []
	bs = BlueSerial()
	cp = ColorPicker()
	cp.main()

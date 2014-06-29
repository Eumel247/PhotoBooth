#!/usr/bin/env python
import serial #serial communication on?
import time
ser = serial.Serial('/dev/ttyAMA0', 9600, timeout=1)
print "pi_main.py running";


#while True: #Test with PiGertduinoSerial.ino
#    inStr = raw_input("Enter an led number between 0 and 5 followed by ENTER ")
#    try:
#        led = int(inStr)
#    except ValueError:
#        led = -1
#    if led >= 0 and led <= 5:
#        ser.write(str(led))
#        print ser.readline()[:-2]

def pi_getpicture():
   	"switches USB on and fetches the latest picture"
	#tell Gertduino to close the USB+ relay
   	#automount usb
   	#transfer picture to extHDD
   	#eject /media/KINGSTON;
   	#tell Gertduino to open the relay
   	return
   
   
def pi_photomerge():
	"merges the latest picture(s) into a printable <Photostripe>"
	#
	return

   
def pi_print():
	"run after pi_photomerge! prints to Canon Selphy 800"
	print "druck...";
	#- Pushbutton: Print
	return


while True:
    data = ser.read(9999)
    if len(data) > 0:
        print "Received:", data
	if "gert2pi_shutter" in data:
    		print "execute shutter";
  	elif "gert2pi_print" in data:
    		print "execute print";
		pi_print();
    	elif "gert2pi_getPicture" in data:
    		print "getPicture";
  	else:	
		print "command not known";
  	
    #time.sleep(0.5);

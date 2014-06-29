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
   	"Fetches the latest picture and tells the Gertduino when it's done."
	print "pi_getpicture running";
	time.sleep(5);
	#automount usb
   	#transfer picture to extHDD
   	#eject /media/KINGSTON;
   	ser.write("pi2gert_gotPicture\n"); #disableUSB
	return
   
   
def pi_photomerge():
	"merges the latest picture(s) into a printable <Photostripe>"
	print "pi_photomerge running";
	return

   
def pi_print():
	"print routine"
	print "pi_print running";
	time.sleep(3);
	ser.write("pi2gert_printDone\n"); #ready to print
	return


while True:
    data = ser.read(9999)
    if len(data) > 0:
        print "Received:", data;
	if "gert2pi_print" in data:
    		pi_photomerge();
		pi_print();
    	elif "gert2pi_getPicture" in data:
    		print "got gert2pi_getPicture";
		pi_getpicture();		
	else:	
		print "command not known";
  	
    #time.sleep(0.5);

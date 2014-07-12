#!/usr/bin/env python
import serial #serial communication on?
import time
import subprocess

dslr = "/media/KINGSTON"
hdd = "/media/TREKSTOR/photobooth/raw"

# strings
com_gert2pi = None
com_pi2gert = None

# status booleans (flags)
waiting_for_echo = False
waiting_for_confirmation = False
#serial = False

# integers
send_counter = 0

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


def check_serial():
	"checks serial port for incoming communication"
	global waiting_for_echo
	global waiting_for_confirmation
	data = ser.read(9999) #max size (bit) of string
    	print "len(data):" #debug
	print len(data) #debug
	print (len(data) > 1) #debug
	if len(data) > 1:
        	print "Received:", data
		if waiting_for_echo: # pi2gert
			waiting_for_echo = False
			check_echo(data)
			return
		if waiting_for_confirmation: # gert2pi
			if "confirmed" in data:
				waiting_for_confirmation = False
				interpret_gert2pi(com_gert2pi) # global com_gert2pi: no need to define as global for reading	
				return
			#else: 
			#	print "Not confirmed! answer is:", data
			#	echo_gert2pi
			
		#Else: no flags set
		global com_gert2pi
		com_gert2pi = data
		echo_gert2pi(data)
	return

#################### communication pi2gert ####################	
def pi2gert(str):
	"serial communication between RPi and Gertduino"
	print "pi2gert: %s" % str
	waiting_for_echo = True
	ser.write("%s\n") % str
	global com_pi2gert
	com_pi2gert = str
	return

def check_echo(echo):
	"check if Gertduino got the command right"
	global send_counter
	if send_counter == 10:
		send_counter = 0
		print "check_echo timeout, aborting communication!"
		return
	if echo == com_pi2gert:
		print "Correct echo:", echo
		ser.write("confirmed\n")
		send_counter = 0
	else:
		send_counter += 1
		print "Wrong echo:", echo
		print "Resend:", com_pi2gert
		pi2gert(com_pi2gert) # send again
	time.sleep(0.5)
	return		
	
#################### communication gert2pi ####################
def echo_gert2pi(data):
	#print data #debug
	print "Echo: %s" % data
	waiting_for_confirmation = True
	print type(data)
	print type("\n")
	ser.write(data +"r\n") #% data
	return
	
def interpret_gert2pi(data):
	"interprets the command from RPi"
	if "gert2pi_print" in data:
    		pi_photomerge()
		pi_print()
	elif "gert2pi_getPicture" in data:
    		pi_getpicture()	
	else:	
		print "command not known"
	return

#################### subprocesses ####################	
def pi_getpicture():
   	"Fetches the latest picture and tells the Gertduino when it's done."
	print "pi_getpicture running"
	time.sleep(5)
	#automount usb
   	#transfer picture to extHDD
	subprocess.call(["rsync", "-a", "$dslr", "$hdd"])
   	#eject /media/KINGSTON
	subprocess.call(["sudo", "umount", "/media/KINGSTON"])
   	pi2gert("pi2gert_gotPicture\n") #disableUSB
	return
   
   
def pi_photomerge():
	"merges the latest picture(s) into a printable Photostripe"
	print "pi_photomerge running"
	return

   
def pi_print():
	"print routine"
	print "pi_print running"
	time.sleep(3)
	pi2gert("pi2gert_printDone\n") #ready to print
	return

	


#################### loop ####################
while True:
	check_serial()

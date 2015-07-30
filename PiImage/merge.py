
#!/usr/bin/env python
#import serial #serial communication on?
#import time
import subprocess
import os
#import random
from PIL import Image, ImageOps

# strings
com_gert2pi = ""
com_pi2gert = ""
output_path = ""

# status booleans (flags)
waiting_for_echo = False
waiting_for_confirmation = False
#serial = False

# integers
#send_counter = 0

#ser = serial.Serial('/dev/ttyAMA0', 9600, timeout=1)
#print "pi_main.py running";

#while True: #Test with PiGertduinoSerial.ino
#    inStr = raw_input("Enter an led number between 0 and 5 followed by ENTER ")
#    try:
#        led = int(inStr)
#    except ValueError:
#        led = -1
#    if led >= 0 and led <= 5:
#        ser.write(str(led))
#        print ser.readline()[:-2]

#def check_serial():

#   "checks serial port for incoming communication"
#   global waiting_for_echo
#   global waiting_for_confirmation
#   global com_gert2pi
#   data = ser.read(9999) #max size (bit) of string
        #print "len(data):" #debug
    #print len(data) #debug
    #print data #debug
#   if len(data) > 1:
 #          print "Received:", data
#       if waiting_for_echo == True: # pi2gert
#           #waiting_for_echo = False
#           check_echo(data)
#           return
#       if waiting_for_confirmation == True: # gert2pi
#           if "confirmed" in data:
#               waiting_for_confirmation = False
#               interpret_gert2pi(com_gert2pi)  
#               return
            #else: 
            #   print "Not confirmed! answer is:", data
            #   echo_gert2pi
#       if "debug" in data:
#           print "Debug:" + data
#           return  
        #Else: no flags set
#       global com_gert2pi
#       com_gert2pi = data
#       echo_gert2pi(data)
#   return

#################### communication pi2gert #################### 
#def pi2gert(str):
#   "serial communication between RPi and Gertduino"
#   print "pi2gert: %s" % str
#   #waiting_for_echo = True
#   ser.write(str +"\n")
#   global com_pi2gert
#   com_pi2gert = str
#   return

#def check_echo(echo):
#   "check if Gertduino got the command right"
#   global send_counter
#   if send_counter == 10:
#       send_counter = 0
#       print "check_echo timeout, aborting communication!"
#       return
#   if echo == com_pi2gert:
#       print "Correct echo:", echo
#       ser.write("confirmed\n")
#       send_counter = 0
#   else:
#       send_counter += 1
#       print "Wrong echo:", echo
#       print "Resend:", com_pi2gert
#       pi2gert(com_pi2gert) # send again
#   time.sleep(0.5)
#   return      
    
#################### communication gert2pi ####################
#def echo_gert2pi(data):
    #print data #debug
#   global waiting_for_confirmation
#   print "Echo: " + data
#   waiting_for_confirmation = True
#   ser.write(data +"\n") #% data
#   return
    
#def interpret_gert2pi(data):
#   "interprets the command from RPi"
#   print "interpret running"
#   if "gert2pi_print" in data:
 #          pi_photomerge()
#       pi_print()
#   elif "gert2pi_getPicture" in data:
 #          pi_getpicture() 
#   else:   
#       print "command not known"
#   return

#################### subprocesses ####################  
#def pi_getpicture():
 #      "Fetches the latest picture and tells the Gertduino when it's done."
#   print "pi_getpicture running"
#   time.sleep(5)
#   #mount usb
 #      #subprocess.Popen( "mount /dev/sdb1")
#   #subprocess.call(["mount", "/dev/sdb1/"])
#   #transfer picture to extHDD
#   subprocess.call(["rsync", "-a", "/media/KINGSTON_/DCIM/100OLYMP/", "/media/extHDD/photobooth/raw/"])
 #      #eject /media/KINGSTON
#   time.sleep(1)
#   subprocess.call(["sudo", "umount", "/media/KINGSTON_"])
#   print "extHDD unmount"
#   pi2gert("pi2gert_gotPicture\n") #disableUSB
#   return
   

   
def pi_photomerge():
    "merges the latest picture(s) into a printable Photostripe"
    print "pi_photomerge running"

    global output_path
    
    layout = 'layout1.png'
    print layout

    #empty_path = os.path.join(os.path.dirname(__file__), '..', 'PiImage', 'layout', 'empty.png')   
    #layout_path = os.path.join(os.path.dirname(__file__), '..', 'PiImage', 'layout', layout)
    empty_path = 'D:/empty.png'
    layout_path = 'D:/layout1.png'
    pic_raw_dir = 'D:/raw'
    output_dir = 'D:/merged'

# get path of last picture   
    files = sorted([f for f in os.listdir(pic_raw_dir) if f.startswith('20')])
    print "Last picture: %s" % (files[-1],)

    for i in files:
        print i
        recent = i  
        last_pic_path = os.path.join(pic_raw_dir, recent)
#   print last_pic_path

        output_path = os.path.join(output_dir, 'print_' + recent[:-4] + '.png')

#   try:
        empty = Image.open(empty_path)
        layout = Image.open(layout_path)
        pic_raw = Image.open(last_pic_path)
        pic = pic_raw.resize((924,693),resample=0)
        #pic.save('pic.JPG')
        empty.paste(pic, (70, 460), mask=None)
        empty.paste(pic.rotate(90, resample=0, expand=0), (1062,129), mask=None)
        empty.paste(layout, (0,0), layout)
        empty.save(output_path)
        print "Output written to: %s" % (output_path)
    
#   except IOError:
#       print('An error occured trying to read the file.')
        
#   except:
#       print('An error occured.')
    
    
    return

   
#def pi_print():
#   "print routine"
#   print "pi_print running"
#   
#   global output_path
#
#   subprocess.call(["lp", "-d", "Canon_CP800", output_path])
#
#   pi2gert("pi2gert_printDone\n") #ready to print
#   return

    
pi_photomerge()

#################### loop ####################
#while True:
#   check_serial()
#   print waiting_for_confirmation

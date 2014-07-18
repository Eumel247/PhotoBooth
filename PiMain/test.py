#!/usr/bin/env python
import serial #serial communication on?
import time
import subprocess
from PIL import Image, ImageOps



#"Fetches the latest picture and tells the Gertduino when it's done."
print "pi_getpicture running"
#time.sleep(5)
#automount usb
#transfer picture to extHDD
subprocess.call(["rsync", "-a", "/media/KINGSTON/DCIM/101OLYMP/", "/media/extHDD/photobooth/raw/"])
#eject /media/KINGSTON
time.sleep(1)
subprocess.call(["sudo", "umount", "/media/KINGSTON"])
print "extHDD unmount"
#pi2gert("pi2gert_gotPicture\n") #disableUSB

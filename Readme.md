--DSLR PhotoBooth--

The standalone PhotoBooth is developed for a wedding. It will stand on a speaker stand and will have pushbuttons, a screen, a printer for instant printing, a 7-segement display for countdown, ... and much more ;)

The System is controlled by a Raspberry in combination with a Gertduino.
The DSLR is an Olympus E-510, triggered by cable (Optocoupler...). The flash is integrated in a white box.
After a picture is taken it is transfered to the Raspberry by USB (USB has to be switched on an off, because the connected USB disables the Live-View -> Relay!).
The people should see if they're in the picture during the countdown, so the Live-View of the DSLR is converted from AV to VGA and is displayed on a TFT screen.
It is possible to print the pictures (merged with existing pictures) on a Canon Selphy CP800 and review the existing pictures on an Ipad on the backside of the Box.

The Picture-Taking-Process is initialized by a push on an customized "Easy-Button" which is connected to the Arduino and starts the main loop. 


...


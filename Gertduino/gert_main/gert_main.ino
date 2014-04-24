/*
 Input Pullup Serial
 
 This example demonstrates the use of pinMode(INPUT_PULLUP). It reads a
 digital input on pin 2 and prints the results to the serial monitor.
 
 The circuit:
 * Momentary switch attached from pin 2 to ground
 * Built-in LED on pin 13
 
 Unlike pinMode(INPUT), there is no pull-down resistor necessary. An internal
 20K-ohm resistor is pulled to 5V. This configuration causes the input to
 read HIGH when the switch is open, and LOW when it is closed.
 
 created 14 March 2012
 by Scott Fitzgerald
 
 http://www.arduino.cc/en/Tutorial/InputPullupSerial
 
 This example code is in the public domain
 
 */

void setup(){
  Serial.begin(9600); //start serial connection
  
  int shutterBUT = A2; 	//left push-button
  pinMode(shutterBUT, INPUT_PULLUP); // enable the internal pull-up resistor
  
  int printBUT = A3;	//right push-button
  pinMode(printBUT, INPUT_PULLUP);
  
  int shutterLED = 13;	//LED0
  pinMode(shutterLED, OUTPUT);
  
  int printLED = 9;		//LED1
  pinMode(printLED, OUTPUT);
  
  //int LED = 10;		//LED2
  //int LED = 4;		//LED3
  //int LED = 6;		//LED4
  //int LED = 7;		//LED5
  
}

void loop(){
  //read the pushbutton values into variables
  int shutterVal = digitalRead(shutterBUT);
  int printVal = digitalRead(printBUT);
  
  //print out the values of the pushbuttons  
  Serial.println(shutterVal);
  Serial.println(printVal);
 
  // Keep in mind the pullup means the pushbutton's
  // logic is inverted. It goes HIGH when it's open,
  // and LOW when it's pressed. Turn on shutterLED when the
  // button's pressed, and off when it's not:
  if (shutterVal == HIGH) {
    digitalWrite(shutterLED, LOW);
  }
  else {
    digitalWrite(shutterLED, HIGH);
  }
  if (printVal == HIGH) {
    digitalWrite(printLED, LOW);
  }
  else {
    digitalWrite(printLED, HIGH);
  }
}
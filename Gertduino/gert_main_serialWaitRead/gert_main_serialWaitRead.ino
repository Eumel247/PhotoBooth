// run 'avrdude -c gpio -p m328p -e -v' before upload if you get a verification error
#include <Wire.h>
#include <Adafruit_GFX.h> // download from Adafruit, unpack, move to <Sketchbook>/libraries/
#include <Adafruit_LEDBackpack.h> // download from Adafruit, unpack, move to <Sketchbook>/libraries/
#include <Metro.h> // Include the Metro library

Adafruit_BicolorMatrix matrix = Adafruit_BicolorMatrix();
// view bicolor8x8 in libraries/Adafruit_LEDBackpack

/*
 Input Pullup Serial:
 Unlike pinMode(INPUT), there is no pull-down resistor necessary. An internal
 20K-ohm resistor is pulled to 5V. This configuration causes the input to
 read HIGH when the switch is open, and LOW when it is closed.
 view http://www.arduino.cc/en/Tutorial/InputPullupSerial
 */

//Rx
//Tx
//int = 2; //D2
int statusLED = 3; //LED3/D3 blue LED
//int = 4; //D4
int warnLED = 5; //LED4/D5 red LED
int loopLED= 6; //LED5/D6
int printLED = 7; //D7

int focusOUT = 8; //D8
int shutterOUT = 9; //LED1/D9
int usbLED = 10; //LED2/D10
int usbOUT = 11; //D11
//int = 12; //D12
int debugLED = 13; //LED0
//Gnd
//
//
//

int shutterBUT = A0; //A0
//A1
int printBUT = A2; //left push-button/A2
int usbBUT = A3; //right push-button/A3 not wired -> A0
//A4
//A5

int off = 0;
int slow = 1;
int fast = 2;
int on = 3;

//					        0	  1	  2	  3	  4	  5	  6	  7	  8	  9	  10  11	12	13
//								              Status	WarnLoopPrint					          Debug
int LEDstatus[] = { 
  0,	0,	0,	off,0,	off,off,off,0,	0,	0,	0,	0,	off};


Metro serialMetro = Metro(250); // Metro event every 250 ms, triggers serial-read
Metro ledMetro = Metro(500); 
Metro resetMetro = Metro(1000);

String inputString = "";         // a string to hold incoming data
String com_gert2pi = "";     // storage for gert2pi command
String com_pi2gert = "";     // storage for pi2gert command
boolean waiting_for_print = false;    // whether the pi_print() function is running
boolean waiting_for_picture = false;    // whether the pi_print() function is running
boolean waiting_for_echo = false; // waits for echo
// boolean waiting_for_confirmation = false; // waits for confirmation off the echoed command
int send_counter = 0; //triggers an check_echo malfunction
int watchdog = 0; // resets the status after 10 sec
int tired = 0; // sets the booth in sleeping mode 

static const uint8_t PROGMEM
smile_bmp[] =
{ 
  B00111100,
  B01000010,
  B10100101,
  B10000001,
  B10100101,
  B10011001,
  B01000010,
  B00111100 }
,
neutral_bmp[] =
{ 
  B00111100,
  B01000010,
  B10100101,
  B10000001,
  B10111101,
  B10000001,
  B01000010,
  B00111100 }
,
frown_bmp[] =
{ 
  B00111100,
  B01000010,
  B10100101,
  B10000001,
  B10011001,
  B10100101,
  B01000010,
  B00111100 }
,
one_bmp[] = 
{ 
  B00001000,
  B00011000,
  B00111000,
  B00011000,
  B00011000,
  B00011000,
  B00011000,
  B00111100 }
,
two_bmp[] =
{ 
  B00111100,
  B01100110,
  B01100110,
  B00000110,
  B00001100,
  B00011000,
  B00110000,
  B01111110 }
,
three_bmp[] = 
{ 
  B00111100,
  B01100110,
  B01000110,
  B00011100,
  B00000110,
  B00000110,
  B01100110,
  B00111100 }
,
four_bmp[] =
{ 
  B00000110,
  B00001110,
  B00010110,
  B00100110,
  B01111111,
  B00000110,
  B00000110,
  B00000110 }
,
five_bmp[] =
{ 
  B01111110,
  B01100000,
  B01100000,
  B01111100,
  B00000110,
  B00000110,
  B00000110,
  B01111100 }
,
six_bmp[] =
{ 
  B00111100,
  B01100010,
  B01100000,
  B01111100,
  B01100110,
  B01100110,
  B01100110,
  B00111100 }
,
seven_bmp[] =
{ 
  B01111110,
  B00000110,
  B00001100,
  B00001100,
  B00011000,
  B00011000,
  B00110000,
  B00110000 }
,
eight_bmp[] =
{ 
  B00111100,
  B01100110,
  B01100110,
  B00111100,
  B01100110,
  B01100110,
  B01100110,
  B00111100 }
,
nine_bmp[] =
{ 
  B00111100,
  B01100110,
  B01100110,
  B00111110,
  B00000110,
  B00000110,
  B01000110,
  B00111100 };

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

void setup(){
  Serial.begin(9600); // opens serial port, sets data rate to 9600 bps
  inputString.reserve(200); // reserve 200 bytes for the inputString

  pinMode(shutterBUT, INPUT_PULLUP); // enable the internal pull-up resistor
  //pinMode(testBUT, INPUT_PULLUP); // internal, right push-button, test-purposes
  pinMode(printBUT, INPUT_PULLUP); // internal, right push-button
  pinMode(usbBUT, INPUT_PULLUP);
  pinMode(debugLED, OUTPUT); // initialize the LED pins as output
  pinMode(printLED, OUTPUT); 
  pinMode(warnLED, OUTPUT); 
  pinMode(statusLED, OUTPUT);
  pinMode(loopLED, OUTPUT);
  pinMode(usbLED, OUTPUT); // 
  pinMode(focusOUT, OUTPUT); // initialize the focus and shutter pins as output
  pinMode(shutterOUT, OUTPUT);
  pinMode(usbOUT, OUTPUT); // initialize the output pin which goes to the relay (USB on - off)

  matrix.begin(0x70);  // pass in the LED-8x8 address
  matrix.setRotation(1);  // 1: pins bottomside
} // setup

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

void loop(){  

  LEDstatus[loopLED] = slow;

  int shutterVal = digitalRead(shutterBUT); //read the push-button values into variables
  int printVal = digitalRead(printBUT);
  int usbVal = digitalRead(usbBUT);

  //print out the values of the pushbuttons  

  // Keep in mind the pullup means the pushbutton's
  // logic is inverted. It goes HIGH when it's open,
  // and LOW when it's pressed. Turn on shutterLED when the
  // button's pressed, and off when it's not:


  /////////// Metro checks ////////////

  //if (serialMetro.check() == 1) {
  check_serial();
  //}

  if (resetMetro.check() == 1) {
    if (waiting_for_print) { //120 sec
      watchdog += 1;
    }
    else if (waiting_for_picture || waiting_for_echo) { //12 sec // || waiting_for_confirmation
      watchdog += 10;
    }
    if (watchdog == 120) {
      waiting_for_picture = false;
      waiting_for_print = false;
      waiting_for_echo = false;
      //     waiting_for_confirmation = false;
      watchdog = 0;
    }

    tired += 1;
    if (tired == 300) {
      //todo

    }
  }

  if (ledMetro.check() == 1) { // check if the metro has passed its interval .
    int i = 0; // loop counter
    int delay = 0; // delay for slow mode 1
    for (i = 0; i < 14; i = i + 1) {
      //Serial.println(LEDstatus[i]);
      if (LEDstatus[i] == 0) {
        digitalWrite(i, LOW);
      }
      if (LEDstatus[i] == 1) {
        if (delay == 1) {
          digitalWrite(i, !digitalRead(i));
          delay = 0;
        }
        else {
          delay = 1;
        }
      }
      if (LEDstatus[i] == 2) {
        digitalWrite(i, !digitalRead(i));
      }
      if (LEDstatus[i] == 3) {
        digitalWrite(i, HIGH);
      }
    }	
  }

  //digitalWrite(loopLED, HIGH); //LED indicates running loop


  if (waiting_for_print) {
    LEDstatus[printLED] = off; //ready to print
  }
  else {
    LEDstatus[printLED] = on;
  }

  if (waiting_for_picture) {
    //Debug
    //    matrix.clear();
    //    matrix.drawBitmap(0, 0, four_bmp, 8, 8, LED_RED);
    //    matrix.writeDisplay();
    digitalWrite(usbOUT, HIGH); // USB on
  }  
  else {
    //    matrix.clear();
    //    matrix.drawBitmap(0, 0, five_bmp, 8, 8, LED_RED);
    //    matrix.writeDisplay();
    digitalWrite(usbOUT, LOW); // USB off
  }

  if (digitalRead(usbOUT) == LOW) { //USB status
    LEDstatus[usbLED] = off;
  }
  else {
    LEDstatus[usbLED] = fast;
  }

  if (usbVal == LOW && !waiting_for_picture) { //USB switch (right push button)
    if (digitalRead(usbOUT) == LOW) {
      digitalWrite(usbOUT, HIGH);
      LEDstatus[usbLED] = slow;
    }
    else {
      digitalWrite(usbOUT, LOW);
      LEDstatus[usbLED] = off;
    }
  }

  /////SHUTTER/////
  if (shutterVal == LOW && !waiting_for_picture) {
    LEDstatus[debugLED] = on;
    //Serial.println("gert2pi_shutter");

    takePicture(); 

    matrix.clear();
    matrix.writeDisplay();

    getPicture();

  }
  else {
    LEDstatus[debugLED] = off;

  } // else

  /////PRINT/////  
  if (printVal == LOW && !(waiting_for_picture || waiting_for_print)) {
    LEDstatus[printLED] = off;
    waiting_for_print = true;
    gert2pi("gert2pi_print");

    //Debug
    //matrix.clear();
    //matrix.drawBitmap(0, 0, three_bmp, 8, 8, LED_RED);
    //matrix.writeDisplay();
    //delay(1000);

  } // if

} // loop

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

//serial communication//////////////////////////////////////////////////////////
void check_serial() {

  if(Serial.available() > 0) {
    inputString = Serial.readStringUntil('\n');
    //x = Serial.parseInt();

    if(waiting_for_echo && (inputString.length() > 0)) {

      //Debug
      //matrix.clear();
      //matrix.drawBitmap(0, 0, two_bmp, 8, 8, LED_RED);
      //matrix.writeDisplay();
      //Serial.println("debug_echo");

      waiting_for_echo = false;
      check_echo(inputString);
      //LEDstatus[statusLED] = on; //blau, debug
      return;
    }
    else {
      //LEDstatus[statusLED] = off; //blau, debug
    }

    /* handshake pi2gert
     if(waiting_for_confirmation) {
     if(inputString == "confirmed") {
     waiting_for_confirmation = false;
     interpret_pi2gert(inputString);
     }
     return;
     }
     */
    //else
    if(waiting_for_picture) {
      matrix.clear();
      matrix.fillRect(0,0, 8,8, LED_RED);
      matrix.writeDisplay(); 
      delay(1000);    
    }
    else {
      matrix.clear();
      matrix.fillRect(0,0, 8,8, LED_GREEN);
      matrix.writeDisplay(); 
      delay(1000);
    } 


    interpret_pi2gert(inputString);


    /*handshake pi2gert    
     com_pi2gert = inputString;
     echo_pi2gert(inputString);
     */

  } //if serial avalaible

} //check_serial

//communication gert2pi//////////////////////////////////////////////////////////
void gert2pi(String str) {
  waiting_for_echo = true;
  Serial.println(str);
  com_gert2pi = str; 

  //Debug
  //matrix.clear();
  //matrix.drawBitmap(0, 0, one_bmp, 8, 8, LED_RED);
  //matrix.writeDisplay();

}

void check_echo(String echo) {
  if(send_counter == 10) {
    send_counter = 0;
    LEDstatus[warnLED] = fast;
    return;
  }

  //Serial.println("debug_check_echo running");
  //Serial.println(echo);
  //Serial.println(com_gert2pi);
  //Serial.println(com_gert2pi == echo);
  echo.trim();
  com_gert2pi.trim();
  //Serial.println(echo);
  //Serial.println(com_gert2pi);
  //Serial.println(com_gert2pi.startsWith(echo)); 
  //Serial.println(com_gert2pi == echo); 

  if(com_gert2pi == echo) {
    //Debug
    //matrix.clear();
    //matrix.drawBitmap(0, 0, seven_bmp, 8, 8, LED_RED);
    //matrix.writeDisplay();

    LEDstatus[warnLED] = off;
    Serial.println("confirmed");
    send_counter = 0;
  }
  else {

    //Debug
    //    matrix.clear();
    //    matrix.drawBitmap(0, 0, nine_bmp, 8, 8, LED_RED);
    //    matrix.writeDisplay();

    send_counter += 1;
    LEDstatus[warnLED] = slow;
    gert2pi(com_gert2pi); //send again
  }
}

//communication pi2gert//////////////////////////////////////////////////////////

/* handshake pi2gert
 void echo_pi2gert(String inputString) {
 //debug
 matrix.clear();
 matrix.drawBitmap(0, 0, six_bmp, 8, 8, LED_YELLOW);
 matrix.writeDisplay();
 delay(1000);
 
 waiting_for_confirmation = true;
 Serial.println(inputString);
 }
 */

void interpret_pi2gert(String inputString) {


  //Serial.println(inputString == "pi2gert_gotPicture");
  inputString.trim();
  //Serial.println(inputString);
  //Serial.println(inputString == "pi2gert_gotPicture");

  //  Debug
  //  matrix.clear();
  //  matrix.drawBitmap(0, 0, two_bmp, 8, 8, LED_RED);
  //  matrix.writeDisplay();


  if(inputString == "pi2gert_printDone") {
    //Serial.println("Print done!");
    waiting_for_print = false;



    inputString = "";
  }

  if(inputString == "pi2gert_gotPicture") {
    matrix.clear();
    matrix.fillRect(0,0, 8,8, LED_YELLOW);
    matrix.writeDisplay(); 
    delay(1000);
    //Serial.println("Got picture!");
    waiting_for_picture = false;
    inputString = "";
  }
}



/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////


void getPicture() {
  //LEDstatus[statusLED] = fast;
  waiting_for_picture = true;
  // 8x8 Display?
  digitalWrite(usbOUT, HIGH); // closes the relay -> USB enabled
  delay(1000);
  gert2pi("gert2pi_getPicture"); // tell RPi to get the picture
  //delay(1000); // test 
  //LEDstatus[statusLED] = off;

} // getPicture

/////////////////////////////////////////////////////////////////////////////////

void takePicture() {
  matrix.clear();
  matrix.fillRect(0,0, 8,8, LED_RED);
  //matrix.fillRect(2,2, 4,4, LED_GREEN);
  matrix.writeDisplay(); 
  delay(1000);

  matrix.setTextWrap(false);  // we dont want text to wrap so it scrolls nicely
  matrix.setTextSize(1);
  matrix.setTextColor(LED_YELLOW);
  for (int8_t x=7; x>=-36; x--) {
    matrix.clear();
    matrix.setCursor(x,0);
    matrix.print("Foto in");
    matrix.writeDisplay();
    delay(40);
  } // for

  //Countdown      
  matrix.clear();
  matrix.drawBitmap(0, 0, nine_bmp, 8, 8, LED_GREEN);
  matrix.writeDisplay();
  delay(1000); 

  digitalWrite(focusOUT, HIGH); //camera wakeup ->

  matrix.clear();
  matrix.drawBitmap(0, 0, eight_bmp, 8, 8, LED_GREEN);
  matrix.writeDisplay();
  delay(1000); 

  digitalWrite(focusOUT, LOW); //-> camera wakeup

  matrix.clear();
  matrix.drawBitmap(0, 0, seven_bmp, 8, 8, LED_GREEN);
  matrix.writeDisplay();
  delay(1000); 

  matrix.clear();
  matrix.drawBitmap(0, 0, six_bmp, 8, 8, LED_YELLOW);
  matrix.writeDisplay();
  delay(1000); 

  matrix.clear();
  matrix.drawBitmap(0, 0, five_bmp, 8, 8, LED_YELLOW);
  matrix.writeDisplay();
  delay(1000); 

  matrix.clear();
  matrix.drawBitmap(0, 0, four_bmp, 8, 8, LED_YELLOW);
  matrix.writeDisplay();
  delay(1000); 

  digitalWrite(focusOUT, HIGH);

  matrix.clear();
  matrix.drawBitmap(0, 0, three_bmp, 8, 8, LED_RED);
  matrix.writeDisplay();
  delay(1000); 

  matrix.clear();
  matrix.drawBitmap(0, 0, two_bmp, 8, 8, LED_RED);
  matrix.writeDisplay();
  delay(1000); 

  digitalWrite(shutterOUT, HIGH);  

  matrix.clear();
  matrix.drawBitmap(0, 0, one_bmp, 8, 8, LED_RED);
  matrix.writeDisplay();
  delay(1000); 

  digitalWrite(focusOUT, LOW);
  digitalWrite(shutterOUT, LOW);

  matrix.clear();
  matrix.drawBitmap(0, 0, smile_bmp, 8, 8, LED_GREEN);
  matrix.writeDisplay();
  delay(1000);
}


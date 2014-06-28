// run 'avrdude -c gpio -p m328p -e -v' before upload if you get a verification error
#include <Wire.h>
#include <Adafruit_GFX.h> // download from Adafruit, unpack, move to <Sketchbook>/libraries/
#include <Adafruit_LEDBackpack.h> // download from Adafruit, unpack, move to <Sketchbook>/libraries/

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
//int = 10; //LED2/D10
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
//int shutterBUT = A3; //right push-button/A3 not wired -> A0
//A4
//A5




void setup(){
  Serial.begin(9600); //start serial connection

  pinMode(shutterBUT, INPUT_PULLUP); // enable the internal pull-up resistor
  pinMode(printBUT, INPUT_PULLUP);
  pinMode(debugLED, OUTPUT); // initialize the LED pins as output
  pinMode(printLED, OUTPUT); 
  pinMode(warnLED, OUTPUT); 
  pinMode(statusLED, OUTPUT);
  pinMode(loopLED, OUTPUT);
  pinMode(focusOUT, OUTPUT); // initialize the focus and shutter pins as output
  pinMode(shutterOUT, OUTPUT);
  pinMode(usbOUT, OUTPUT); // initialize the output pin which goes to the relay (USB on - off)
  

  matrix.begin(0x70);  // pass in the LED-8x8 address
}

static const uint8_t PROGMEM
  smile_bmp[] =
  { B00111100,
    B01000010,
    B10100101,
    B10000001,
    B10100101,
    B10011001,
    B01000010,
    B00111100 },
  neutral_bmp[] =
  { B00111100,
    B01000010,
    B10100101,
    B10000001,
    B10111101,
    B10000001,
    B01000010,
    B00111100 },
  frown_bmp[] =
  { B00111100,
    B01000010,
    B10100101,
    B10000001,
    B10011001,
    B10100101,
    B01000010,
    B00111100 },
  one_bmp[] = 
  { B00001000,
    B00011000,
    B00111000,
    B00011000,
    B00011000,
    B00011000,
    B00011000,
    B00111100 },
  two_bmp[] =
  { B00111100,
    B01100110,
    B01100110,
    B00000110,
    B00001100,
    B00011000,
    B00110000,
    B01111110 },
  three_bmp[] = 
  { B00111100,
    B01100110,
    B01000110,
    B00011100,
    B00000110,
    B00000110,
    B01100110,
    B00111100 },
  four_bmp[] =
  { B00000110,
    B00001110,
    B00010110,
    B00100110,
    B01111111,
    B00000110,
    B00000110,
    B00000110 },
  five_bmp[] =
  { B01111110,
    B01100000,
    B01100000,
    B01111100,
    B00000110,
    B00000110,
    B00000110,
    B01111100 },
  six_bmp[] =
  { B00111100,
    B01100010,
    B01100000,
    B01111100,
    B01100110,
    B01100110,
    B01100110,
    B00111100 },
  seven_bmp[] =
  { B01111110,
    B00000110,
    B00001100,
    B00001100,
    B00011000,
    B00011000,
    B00110000,
    B00110000 },
  eight_bmp[] =
  { B00111100,
    B01100110,
    B01100110,
    B00111100,
    B01100110,
    B01100110,
    B01100110,
    B00111100 },
  nine_bmp[] =
  { B00111100,
    B01100110,
    B01100110,
    B00111110,
    B00000110,
    B00000110,
    B01000110,
    B00111100 };

void loop(){
  digitalWrite(loopLED, HIGH); // LED indicates running loop
  
  int shutterVal = digitalRead(shutterBUT); //read the pushbutton values into variables
  int printVal = digitalRead(printBUT);

  //print out the values of the pushbuttons  


  // Keep in mind the pullup means the pushbutton's
  // logic is inverted. It goes HIGH when it's open,
  // and LOW when it's pressed. Turn on shutterLED when the
  // button's pressed, and off when it's not:
  if (shutterVal == HIGH) {
    digitalWrite(shutterLED, LOW);
  }
  else {
    digitalWrite(shutterLED, HIGH);
    Serial.println("gert2pi_shutter");
    
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
    
    matrix.clear();
    matrix.drawBitmap(0, 0, eight_bmp, 8, 8, LED_GREEN);
    matrix.writeDisplay();
    delay(1000); 
    
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
    
    matrix.clear();
    matrix.drawBitmap(0, 0, three_bmp, 8, 8, LED_RED);
    matrix.writeDisplay();
    delay(1000); 
    
    matrix.clear();
    matrix.drawBitmap(0, 0, two_bmp, 8, 8, LED_RED);
    matrix.writeDisplay();
    delay(1000); 
    
    matrix.clear();
    matrix.drawBitmap(0, 0, one_bmp, 8, 8, LED_RED);
    matrix.writeDisplay();
    delay(1000); 
    
    matrix.clear();
    matrix.drawBitmap(0, 0, smile_bmp, 8, 8, LED_GREEN);
    matrix.writeDisplay();
    
    takePicture(); 
    
    delay(1000);
    matrix.clear();
    matrix.writeDisplay();
    
    getPicture();
       
  } // else
  
  if (printVal == HIGH) {
    digitalWrite(printLED, LOW);
  } // if
  else {
    digitalWrite(printLED, HIGH);
    Serial.println("gert2pi_print");
    delay(1000);
  } // else
} // loop

void takePicture() {
  digitalWrite(shutterOUT, HIGH);
  delay(2000);
  digitalWrite(shutterOUT, LOW);
} // takePicture

void getPicture() {
  digitalWrite(usbLED, HIGH); 
  // 8x8 Display?
  digitalWrite(usbOUT, HIGH); // closes the relay -> USB enabled
  Serial.println("gert2pi_getPicture"); // tell RPi to get the picture
  delay(10000); // test
  // wait for RPi to disconnect USB 
  digitalWrite(usbOUT, LOW); // USB off
  digitalWrite(usbLED, LOW); 
} // getPicture


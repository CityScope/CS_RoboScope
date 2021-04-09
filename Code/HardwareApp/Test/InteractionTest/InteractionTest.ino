/*************************************************************
  Interaction Test

  Code Test the Interaction Board
  BUTTONS
  SWITCHES
  LEDS
  MULTIPLEXERS
*************************************************************/

#include <Wire.h> // Include the I2C library (required)
#include <SparkFunSX1509.h> // Include SX1509 library
#include <Adafruit_NeoPixel.h>

struct Color {
  uint8_t red;
  uint8_t blue;
  uint8_t green;
  uint8_t white;
};




//LIMIT BUTTONS
const byte SWITCH_07_SX01 = 8; // LED connected to pin 8
const byte SWITCH_06_SX01 = 6; // LED connected to pin 9
const byte SWITCH_05_SX01 = 9; // LED connected to pin 10
const byte SWITCH_04_SX01 = 7; // LED connected to pin 11

const byte SWITCH_03_SX02 = 8; // LED connected to pin 12
const byte SWITCH_02_SX02 = 6; // LED connected to pin 13
const byte SWITCH_01_SX02 = 9; // LED connected to pin 14
const byte SWITCH_00_SX02 = 7; // LED connected to pin 15

//DOWN BUTTONS
const byte DOWN_07_SX01 = 12; // LED connected to pin 8
const byte DOWN_06_SX01 = 4; // LED connected to pin 9
const byte DOWN_05_SX01 = 15; // LED connected to pin 10
const byte DOWN_04_SX01 = 5; // LED connected to pin 11

const byte DOWN_03_SX02 = 12; // LED connected to pin 12
const byte DOWN_02_SX02 = 15; // LED connected to pin 13
const byte DOWN_01_SX02 = 4; // LED connected to pin 14
const byte DOWN_00_SX02 = 5; // LED connected to pin 15


//UP BUTTONS
const byte UP_07_SX01 = 11; // LED connected to pin 8
const byte UP_06_SX01 = 3; // LED connected to pin 9
const byte UP_05_SX01 = 14; // LED connected to pin 10
const byte UP_04_SX01 = 1; // LED connected to pin 11

const byte UP_03_SX02 = 11; // LED connected to pin 12
const byte UP_02_SX02 = 14; // LED connected to pin 13
const byte UP_01_SX02 = 3; // LED connected to pin 14
const byte UP_00_SX02 = 1; // LED connected to pin 15

//TOUCH PIN
const byte TOUCH_07_SX01 = 10; // LED connected to pin 8
const byte TOUCH_06_SX01 = 2; // LED connected to pin 9
const byte TOUCH_05_SX01 = 13; // LED connected to pin 10
const byte TOUCH_04_SX01 = 0; // LED connected to pin 11

const byte TOUCH_03_SX02 = 10; // LED connected to pin 12
const byte TOUCH_02_SX02 = 13; // LED connected to pin 13
const byte TOUCH_01_SX02 = 2; // LED connected to pin 14
const byte TOUCH_00_SX02 = 0; // LED connected to pin 15


//MUX
const byte DIP_01_SX03 = 6; // LED connected to pin 8
const byte DIP_02_SX03 = 7; // LED connected to pin 9
const byte DIP_03_SX03 = 8; // LED connected to pin 10
const byte DIP_04_SX03 = 9; // LED connected to pin 11
const byte DIP_05_SX03 = 10; // LED connected to pin 12
const byte DIP_06_SX03 = 11; // LED connected to pin 13
const byte DIP_07_SX03 = 12; // LED connected to pin 14
const byte DIP_08_SX03 = 13; // LED connected to pin 15
const byte DIP_09_SX03 = 14; // LED connected to pin 14
const byte DIP_10_SX03 = 15; // LED connected to pin 15

const byte STATUS_PIN_SX03 = 5; //3 bottom 5 top


//NEO PIXELS PINS
const byte NEO_PIN = 14;
const byte NEO_PIN_E = 4;
//Pixels
Adafruit_NeoPixel * pixels;

Color * colors[8];

//NUM PIXELS
boolean colorChange = false;

int curentLED;

int colorId = -1;

//Maximun number of pixels per board
const byte NUMPIXELS = 4;

// SX1509 I2C address (set by ADDR1 and ADDR0 (00 by default):
const byte SX1509_ADDRESS_00 = 0x3E;  // SX1509 I2C address
SX1509 sx00;

const byte SX1509_ADDRESS_01 = 0x3F;
SX1509 sx01;

const byte SX1509_ADDRESS_10 = 0x71;
SX1509 sx02;

const byte SX1509_ADDRESS_11 = 0x70;
SX1509 sx03;

//sx0
const int selectLED[3] = {11, 13, 15};
const int LEDEnable = 4;

int INT_PIN_01 = 32;
int INT_PIN_02 = 33;
boolean buttonsx01 = false;
boolean buttonsx02 = false;

//INIT
void setup()
{

  Serial.begin(9600);
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  delay(500);

  Serial.println("Mux SX1509: ");

  int muxCounter = 0;

  while (muxCounter != 3) {
    if (!sx00.begin(SX1509_ADDRESS_00) ) {
      Serial.println("Failed 0");
      Serial.println(SX1509_ADDRESS_00);
      muxCounter++;
      delay(100);
    } else {
      Serial.println("Connected 0");
      break;
    }
  }

  delay(250);
  muxCounter = 0;

  while (muxCounter != 3) {
    if (!sx01.begin(SX1509_ADDRESS_01) ) {
      Serial.println("Failed 01");
      Serial.println(SX1509_ADDRESS_01);
      muxCounter++;
      delay(100);
    } else {
      Serial.println("Connected 1");
      break;
    }
  }
  delay(250);

  delay(250);
  muxCounter = 0;
  while (muxCounter != 3) {
    if (!sx02.begin(SX1509_ADDRESS_11)) {
      Serial.println("Failed sx 2");
      Serial.println(SX1509_ADDRESS_11);
      muxCounter++;
      delay(100);
    } else {
      Serial.println("Connected 2");
      break;
    }
  }
  delay(250);

  delay(250);
  muxCounter = 0;
  while (muxCounter != 3) {
    if (!sx03.begin(SX1509_ADDRESS_10)) {
      Serial.println("Failed sx 3");
      Serial.println(SX1509_ADDRESS_10);
      muxCounter++;
      delay(100);
    } else {
      Serial.println("Connected 3");
      break;
    }
  }

  delay(250);
  digitalWrite(13, LOW);

  sx01.debounceTime(2);
  sx02.debounceTime(2);

  //sx 00
  sx01.pinMode(SWITCH_04_SX01, INPUT);
  sx01.pinMode(SWITCH_05_SX01, INPUT);
  sx01.pinMode(SWITCH_06_SX01, INPUT);
  sx01.pinMode(SWITCH_07_SX01, INPUT);

  sx01.pinMode(DOWN_04_SX01, INPUT_PULLUP);
  sx01.pinMode(DOWN_05_SX01, INPUT_PULLUP);
  sx01.pinMode(DOWN_06_SX01, INPUT_PULLUP);
  sx01.pinMode(DOWN_07_SX01, INPUT_PULLUP);

  sx01.enableInterrupt(DOWN_04_SX01, CHANGE);
  sx01.enableInterrupt(DOWN_05_SX01, CHANGE);
  sx01.enableInterrupt(DOWN_06_SX01, CHANGE);
  sx01.enableInterrupt(DOWN_07_SX01, CHANGE);

  sx01.debouncePin(DOWN_04_SX01);
  sx01.debouncePin(DOWN_05_SX01);
  sx01.debouncePin(DOWN_06_SX01);
  sx01.debouncePin(DOWN_07_SX01);

  sx01.pinMode(UP_04_SX01, INPUT);
  sx01.pinMode(UP_05_SX01, INPUT);
  sx01.pinMode(UP_06_SX01, INPUT);
  sx01.pinMode(UP_07_SX01, INPUT);

  sx01.pinMode(TOUCH_04_SX01, INPUT);
  sx01.pinMode(TOUCH_05_SX01, INPUT);
  sx01.pinMode(TOUCH_06_SX01, INPUT);
  sx01.pinMode(TOUCH_07_SX01, INPUT);

  pinMode(INT_PIN_01, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(INT_PIN_01),
                  buttonSX01, CHANGE);

  //sx 01
  sx02.pinMode(SWITCH_00_SX02, INPUT);
  sx02.pinMode(SWITCH_01_SX02, INPUT);
  sx02.pinMode(SWITCH_02_SX02, INPUT);
  sx02.pinMode(SWITCH_03_SX02, INPUT);

  sx02.pinMode(DOWN_00_SX02, INPUT_PULLUP);
  sx02.pinMode(DOWN_01_SX02, INPUT_PULLUP);
  sx02.pinMode(DOWN_02_SX02, INPUT_PULLUP);
  sx02.pinMode(DOWN_03_SX02, INPUT_PULLUP);

  sx02.enableInterrupt(DOWN_00_SX02, CHANGE);
  sx02.enableInterrupt(DOWN_01_SX02, CHANGE);
  sx02.enableInterrupt(DOWN_02_SX02, CHANGE);
  sx02.enableInterrupt(DOWN_03_SX02, CHANGE);

  //sx02.debouncePin(DOWN_00_SX02);
  //sx02.debouncePin(DOWN_01_SX02);
  //sx02.debouncePin(DOWN_02_SX02);
  //sx02.debouncePin(DOWN_03_SX02);

  sx02.pinMode(UP_00_SX02, INPUT);
  sx02.pinMode(UP_01_SX02, INPUT);
  sx02.pinMode(UP_02_SX02, INPUT);
  sx02.pinMode(UP_03_SX02, INPUT);

  sx02.pinMode(TOUCH_00_SX02, INPUT);
  sx02.pinMode(TOUCH_01_SX02, INPUT);
  sx02.pinMode(TOUCH_02_SX02, INPUT);
  sx02.pinMode(TOUCH_03_SX02, INPUT);

  pinMode(INT_PIN_02, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(INT_PIN_02),
                  buttonSX02, CHANGE);

  //sx 03

  sx03.pinMode(DIP_01_SX03, INPUT);
  sx03.pinMode(DIP_02_SX03, INPUT);
  sx03.pinMode(DIP_03_SX03, INPUT);
  sx03.pinMode(DIP_04_SX03, INPUT);
  sx03.pinMode(DIP_05_SX03, INPUT);
  sx03.pinMode(DIP_06_SX03, INPUT);
  sx03.pinMode(DIP_07_SX03, INPUT);
  sx03.pinMode(DIP_08_SX03, INPUT);
  sx03.pinMode(DIP_09_SX03, INPUT);
  sx03.pinMode(DIP_10_SX03, INPUT);




  sx03.pinMode(STATUS_PIN_SX03, OUTPUT);

  Serial.println("Done Mux Pis");

  //LED
  sx00.pinMode(selectLED[0], OUTPUT);
  sx00.pinMode(selectLED[1], OUTPUT);
  sx00.pinMode(selectLED[2], OUTPUT);

  //MUX
  for (int i = 0; i < 3; i++) {
    sx00.digitalWrite(selectLED[i], LOW);
  }

  //pinMode(NEO_PIN_E, OUTPUT);

  pinMode(NEO_PIN_E, LOW);


  //reset and init the neopixels
  //pixels = new Adafruit_NeoPixel(NUMPIXELS, NEO_PIN, NEO_GRBW + NEO_KHZ800);
  //pixels->begin();
//  pixels->clear();


  for (int i = 0; i < 8; i++) {
    colors[i] = new Color;
    colors[i]->red = 0;
    colors[i]->blue = 0;
    colors[i]->green = 0;
    colors[i]->white = 0;
  }


  for (int  i = 0; i < 8; i++) {
   // changeColorLED(i, colors[i]->red, colors[i]->green, colors[i]->blue, colors[i]->white);
  }

  Serial.println("Done LEDs");

  //status pinTest
  sx03.digitalWrite(STATUS_PIN_SX03, HIGH);
  delay(500);
  sx03.digitalWrite(STATUS_PIN_SX03, LOW);

  Serial.println("Done Init");
}


void buttonSX01() {
  buttonsx01 = true;
}
void buttonSX02() {
  buttonsx02 = true;

}

void checkInterruptSX01() {
  if (buttonsx01) // If the button() ISR was executed
  {
    // read io.interruptSource() find out which pin generated
    // an interrupt and clear the SX1509's interrupt output.
    unsigned int intStatus = sx01.interruptSource();

    // For debugging handiness, print the intStatus variable.
    // Each bit in intStatus represents a single SX1509 IO.
    Serial.println("intStatus = " + String(intStatus, BIN));

    // If the bit corresponding to our button IO generated
    // the input:

    if (intStatus & (1 << DOWN_04_SX01))
    {
      Serial.println("Button 4 Down!"); // Print a message.
      colorChange = true;
      colorId = 4;
    }

    if (intStatus & (1 << DOWN_05_SX01))
    {
      Serial.println("Button 5 Down!"); // Print a message.
      colorChange = true;
      colorId = 5;
    }

    if (intStatus & (1 << DOWN_06_SX01))
    {
      Serial.println("Button 6 Down!"); // Print a message.
      colorChange = true;
      colorId = 6;
    }

    if (intStatus & (1 << DOWN_07_SX01))
    {
      Serial.println("Button 7 Down!"); // Print a message.
      colorChange = true;
      colorId = 7;
    }

    buttonsx01 = false; // Clear the buttonPressed flag
  }
}

void checkInterruptSX02() {

}


//main interaction test loop
void loop() {

 // checkInterruptSX01();

  //SX 01
  if (sx02.digitalRead(SWITCH_00_SX02) == HIGH) {
    Serial.println("PIN 00");
  }
  if (sx02.digitalRead(SWITCH_01_SX02) == HIGH) {
    Serial.println("PIN 01");
  }
  if (sx02.digitalRead(SWITCH_02_SX02) == HIGH) {
    Serial.println("PIN 02");
  }
  if (sx02.digitalRead(SWITCH_03_SX02) == HIGH) {
    Serial.println("PIN 03");
  }

  if (sx01.digitalRead(SWITCH_04_SX01) == HIGH) {
    Serial.println("PIN 04");
  }
  if (sx01.digitalRead(SWITCH_05_SX01) == HIGH) {
    Serial.println("PIN 05");
  }
  if (sx01.digitalRead(SWITCH_06_SX01) == HIGH) {
    Serial.println("PIN 06");
  }
  if (sx01.digitalRead(SWITCH_07_SX01) == HIGH) {
    Serial.println("PIN 07");
  }


  //SX 02 down
  if (sx02.digitalRead(DOWN_00_SX02) == HIGH) {
    Serial.println("PIN 00 DOWN");
    colorChange = true;
    colorId = 0;
  }
  if (sx02.digitalRead(DOWN_01_SX02) == HIGH) {
    Serial.println("PIN 01 DOWN");
    colorChange = true;
    colorId = 1;
  }
  if (sx02.digitalRead(DOWN_02_SX02) == HIGH) {
    Serial.println("PIN 02 DOWN");
    colorChange = true;
    colorId = 2;
  }
  if (sx02.digitalRead(DOWN_03_SX02) == HIGH) {
    Serial.println("PIN 03 DOWN");
    colorChange = true;
    colorId = 3;
  }
  /*
    if (sx01.digitalRead(DOWN_04_SX01) == HIGH) {
      Serial.println("PIN 04 DOWN");
      colorChange = true;
      colorId = 4;
    }
    if (sx01.digitalRead(DOWN_05_SX01) == HIGH) {
      Serial.println("PIN 05 DOWN");
      colorChange = true;
      colorId = 5;
    }
    if (sx01.digitalRead(DOWN_06_SX01) == HIGH) {
      Serial.println("PIN 06 DOWN");
      colorChange = true;
      colorId = 6;
    }
    if (sx01.digitalRead(DOWN_07_SX01) == HIGH) {
      Serial.println("PIN 07 DOWN");
      colorChange = true;
      colorId = 7;
    }
  */

  //sx up
  if (sx02.digitalRead(UP_00_SX02) == HIGH) {
    Serial.println("PIN 00 UP");
  }
  if (sx02.digitalRead(UP_01_SX02) == HIGH) {
    Serial.println("PIN 01 UP");
  }
  if (sx02.digitalRead(UP_02_SX02) == HIGH) {
    Serial.println("PIN 02 UP");
  }
  if (sx02.digitalRead(UP_03_SX02) == HIGH) {
    Serial.println("PIN 03 UP");
  }

  if (sx01.digitalRead(UP_04_SX01) == HIGH) {
    Serial.println("PIN 04 UP");
  }
  if (sx01.digitalRead(UP_05_SX01) == HIGH) {
    Serial.println("PIN 05 UP");
  }
  if (sx01.digitalRead(UP_06_SX01) == LOW) {
    Serial.println("PIN 06 UP");
  }
  if (sx01.digitalRead(UP_07_SX01) == LOW) {
    Serial.println("PIN 07 UP");
  }

  Serial.println(sx01.digitalRead(UP_07_SX01) );

  //sx03
  if (sx03.digitalRead(DIP_01_SX03) == HIGH) {
    Serial.println("DIP 01 UP");
  }
  if (sx03.digitalRead(DIP_02_SX03) == HIGH) {
    Serial.println("DIP 02 UP");
  }
  if (sx03.digitalRead(DIP_03_SX03) == HIGH) {
    Serial.println("DIP 03 UP");
  }
  if (sx03.digitalRead(DIP_04_SX03) == HIGH) {
    Serial.println("DIP 04 UP");
  }
  if (sx03.digitalRead(DIP_05_SX03) == HIGH) {
    Serial.println("DIP 05 UP");
  }
  if (sx03.digitalRead(DIP_06_SX03) == HIGH) {
    Serial.println("DIP 06 UP");
  }
  if (sx03.digitalRead(DIP_07_SX03) == HIGH) {
    Serial.println("DIP 07 UP");
  }
  if (sx03.digitalRead(DIP_08_SX03) == HIGH) {
    Serial.println("DIP 08 UP");
  }
  if (sx03.digitalRead(DIP_09_SX03) == HIGH) {
    Serial.println("DIP 09 UP");
  }
  if (sx03.digitalRead(DIP_10_SX03) == HIGH) {
    Serial.println("DIP 10 UP");
  }

/*
  if (colorChange && colorId != -1) {

    updateColor(colorId, 0, 0, 255, 0);
    chnageColor();
    chnageColor();

    colorChange = false;
    colorId = -1;
  }

*/
  if (Serial.available() > 0) {
    char key = Serial.read();

    if (key == 'p') {

      for (int i = 0; i < 8; i++) {
        updateColor(i, 0, 0, 255,0);
      }
      chnageColor();


      Serial.println("Done Clean LEDs");
    }

    if (key == '1') {
      //clearLEDs(sx00);
      //changeColorLED(sx00, 0, 255, 0, 0, 0);
      int red = random(0, 255);
      int blue = random(0, 255);
      int green = random(0, 255);
      updateColor(0, red, blue, green, 0);
      chnageColor(0);
      //chnageColor();
    }

    if (key == '2') {
      //clearLEDs(sx00);
      //changeColorLED(sx00, 1, 255, 0, 0, 0);

      int red = random(0, 255);
      int blue = random(0, 255);
      int green = random(0, 255);
      updateColor(1, red, blue, green, 0);
      chnageColor(1);
      //chnageColor();
    }

    if (key == '3') {
      //clearLEDs(sx00);
      //changeColorLED(sx00, 2, 255, 0, 0, 0);

      int red = random(0, 255);
      int blue = random(0, 255);
      int green = random(0, 255);
      updateColor(2, red, blue, green, 0);
      chnageColor();
      //chnageColor();
    }

    if (key == '4') {
      // clearLEDs(sx00);
      //changeColorLED(sx00, 3, 255, 0, 0, 0);

      int red = random(0, 255);
      int blue = random(0, 255);
      int green = random(0, 255);
      updateColor(3, red, blue, green, 0);
      chnageColor();
      // chnageColor();
    }

    if (key == '5') {
      // clearLEDs(sx00);
      //changeColorLED(sx00, 4, 255, 0, 0, 0);

      int red = random(0, 255);
      int blue = random(0, 255);
      int green = random(0, 255);
      updateColor(4, red, blue, green, 0);
      chnageColor();
      chnageColor();
      //chnageColor();
    }
    if (key == '6') {
      //  clearLEDs(sx00);
      //changeColorLED(sx00, 5, 255, 0, 0, 0);

      int red = random(0, 255);
      int blue = random(0, 255);
      int green = random(0, 255);
      updateColor(5, red, blue, green, 0);
      chnageColor();
      // chnageColor();
    }
    if (key == '7') {
      //clearLEDs(sx00);
      //changeColorLED(sx00, 6, 255, 0, 0, 0);

      int red = random(0, 255);
      int blue = random(0, 255);
      int green = random(0, 255);
      updateColor(6, red, blue, green, 0);
      chnageColor();
      //chnageColor();
    }
    if (key == '8') {
      // clearLEDs(sx00);
      //changeColorLED(sx00, 7, 255, 0, 0, 0);
      int red = random(0, 255);
      int blue = random(0, 255);
      int green = random(0, 255);
      updateColor(7, red, blue, green, 0);
      chnageColor();
      // chnageColor();
    }


  }


}

void updateColor(int id,  uint8_t red, uint8_t blue, uint8_t green, uint8_t white) {
  colors[id]->red = red;
  colors[id]->blue = blue;
  colors[id]->green = green;
  colors[id]->white = white;
}

void chnageColor(int id) {
  enableLED(id);
  for (int j = 0; j < NUMPIXELS; j++) {
    pixels->setPixelColor(j, pixels->Color(colors[id]->red, colors[id]->green, colors[id]->blue, colors[id]->white)); // Moderately bright green color.
   pixels->show();
  }
 
}

void chnageColor() {
  for (int i = 0; i < 8; i++) {
    //digitalWrite(NEO_PIN_E, HIGH);
    enableLED(i);
    for (int j = 0; j < NUMPIXELS; j++) {
      pixels->setPixelColor(j, pixels->Color(colors[i]->red, colors[i]->green, colors[i]->blue, colors[i]->white)); // Moderately bright green color.
      pixels->show();
    }
    
    //delay(100);
    // digitalWrite(NEO_PIN_E, LOW);

    //delay(50);
  }

}

void clearLEDs() {
  for (int i = 0; i < 8; i++) {
    changeColorLED(i, 0, 0, 0, 0);
  }
}

void changeColorLED(int id, uint8_t red, uint8_t blue, uint8_t green, uint8_t w) {
  enableLED(id);
  for (int j = 0; j < NUMPIXELS; j++) {
    pixels->setPixelColor(j, pixels->Color(red, blue, green)); // Moderately bright green color.
  }
  pixels->show();
  //delay(50);
}

void enableLED(int id) {
  if (id == 0) {
    sx00.digitalWrite(selectLED[0], HIGH);
    sx00.digitalWrite(selectLED[1], HIGH);
    sx00.digitalWrite(selectLED[2], HIGH);
    Serial.print("c ");
    Serial.println(0);
  }

  if ( id == 1) {
    sx00.digitalWrite(selectLED[0], HIGH);
    sx00.digitalWrite(selectLED[1], LOW);
    sx00.digitalWrite(selectLED[2], HIGH);
    Serial.print("c ");
    Serial.println(1);
  }

  if ( id == 2) {
    sx00.digitalWrite(selectLED[0], LOW);
    sx00.digitalWrite(selectLED[1], HIGH);
    sx00.digitalWrite(selectLED[2], HIGH);
    Serial.print("c ");
    Serial.println(2);
  }

  if ( id == 3) {
    sx00.digitalWrite(selectLED[0], LOW);
    sx00.digitalWrite(selectLED[1], LOW);
    sx00.digitalWrite(selectLED[2], HIGH);
    Serial.print("c ");
    Serial.println(3);
  }
  if ( id == 4) {
    sx00.digitalWrite(selectLED[0], HIGH);
    sx00.digitalWrite(selectLED[1], HIGH);
    sx00.digitalWrite(selectLED[2], LOW);
    Serial.print("c ");
    Serial.println(4);
  }
  if ( id == 5) {
    sx00.digitalWrite(selectLED[0], LOW);
    sx00.digitalWrite(selectLED[1], HIGH);
    sx00.digitalWrite(selectLED[2], LOW);
    Serial.print("c ");
    Serial.println(5);
  }
  if ( id == 6) {
    sx00.digitalWrite(selectLED[0], HIGH);
    sx00.digitalWrite(selectLED[1], LOW);
    sx00.digitalWrite(selectLED[2], LOW);
    Serial.print("c ");
    Serial.println(6);
  }
  if ( id == 7) {
    sx00.digitalWrite(selectLED[0], LOW);
    sx00.digitalWrite(selectLED[1], LOW);
    sx00.digitalWrite(selectLED[2], LOW);//7
    Serial.print("c ");
    Serial.println(7);
  }
}

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


//LIMIT BUTTONS
const byte SWITCH_01_SX01 = 8; // LED connected to pin 8
const byte SWITCH_02_SX01 = 6; // LED connected to pin 9
const byte SWITCH_03_SX01 = 9; // LED connected to pin 10
const byte SWITCH_04_SX01 = 7; // LED connected to pin 11

const byte SWITCH_05_SX02 = 8; // LED connected to pin 12
const byte SWITCH_06_SX02 = 6; // LED connected to pin 13
const byte SWITCH_07_SX02 = 9; // LED connected to pin 14
const byte SWITCH_08_SX02 = 7; // LED connected to pin 15

//DOWN BUTTONS
const byte DOWN_01_SX01 = 12; // LED connected to pin 8
const byte DOWN_02_SX01 = 4; // LED connected to pin 9
const byte DOWN_03_SX01 = 15; // LED connected to pin 10
const byte DOWN_04_SX01 = 5; // LED connected to pin 11

const byte DOWN_05_SX02 = 12; // LED connected to pin 12
const byte DOWN_06_SX02 = 4; // LED connected to pin 13
const byte DOWN_07_SX02 = 15; // LED connected to pin 14
const byte DOWN_08_SX02 = 5; // LED connected to pin 15


//UP BUTTONS
const byte UP_01_SX01 = 11; // LED connected to pin 8
const byte UP_02_SX01 = 3; // LED connected to pin 9
const byte UP_03_SX01 = 14; // LED connected to pin 10
const byte UP_04_SX01 = 1; // LED connected to pin 11

const byte UP_05_SX02 = 11; // LED connected to pin 12
const byte UP_06_SX02 = 3; // LED connected to pin 13
const byte UP_07_SX02 = 14; // LED connected to pin 14
const byte UP_08_SX02 = 1; // LED connected to pin 15

//TOUCH PIN
const byte TOUCH_01_SX01 = 10; // LED connected to pin 8
const byte TOUCH_02_SX01 = 2; // LED connected to pin 9
const byte TOUCH_03_SX01 = 13; // LED connected to pin 10
const byte TOUCH_04_SX01 = 0; // LED connected to pin 11

const byte TOUCH_05_SX02 = 10; // LED connected to pin 12
const byte TOUCH_06_SX02 = 2; // LED connected to pin 13
const byte TOUCH_07_SX02 = 13; // LED connected to pin 14
const byte TOUCH_08_SX02 = 0; // LED connected to pin 15


//MUX
const byte DIP_01_SX03 = 5; // LED connected to pin 8
const byte DIP_02_SX03 = 6; // LED connected to pin 9
const byte DIP_03_SX03 = 7; // LED connected to pin 10
const byte DIP_04_SX03 = 8; // LED connected to pin 11
const byte DIP_05_SX03 = 9; // LED connected to pin 12
const byte DIP_06_SX03 = 10; // LED connected to pin 13
const byte DIP_07_SX03 = 11; // LED connected to pin 14
const byte DIP_08_SX03 = 12; // LED connected to pin 15
const byte DIP_09_SX03 = 13; // LED connected to pin 14
const byte DIP_10_SX03 = 14; // LED connected to pin 15

const byte STATUS_PIN_SX03 = 3;

//NEO PIXELS PINS
const  byte NEO_PIN_01 = 16;
const  byte NEO_PIN_02 = 17;
const  byte NEO_PIN_03 = 2;
const  byte NEO_PIN_04 = 3;
const  byte NEO_PIN_05 = 4;
const  byte NEO_PIN_06 = 24;
const  byte NEO_PIN_07 = 25;
const  byte NEO_PIN_08 = 13;

//NEO PIXELS PINS
const byte NEO_PIN[8] = {NEO_PIN_01, NEO_PIN_02, NEO_PIN_03, NEO_PIN_04,
                         NEO_PIN_05, NEO_PIN_06, NEO_PIN_07, NEO_PIN_08
                        };

//NUM PIXELS
boolean colorChange = false;

int colorId = -1;

//Maximun number of pixels per board
const byte NUMPIXELS = 4;

//Pixels
Adafruit_NeoPixel * pixels[8];

// SX1509 I2C address (set by ADDR1 and ADDR0 (00 by default):
const byte SX1509_ADDRESS_00 = 0x3E;  // SX1509 I2C address
SX1509 sx00;

const byte SX1509_ADDRESS_01 = 0x3F;
SX1509 sx01;

const byte SX1509_ADDRESS_10 = 0x71;
SX1509 sx02;

const byte SX1509_ADDRESS_11 = 0x70;
SX1509 sx03;

//INIT
void setup()
{

  Serial.begin(9600);
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  delay(3000);

  Serial.println("SX1509: ");

  if (!sx00.begin(SX1509_ADDRESS_00) ) {
    Serial.println("Failed 0");
    Serial.println(SX1509_ADDRESS_00);
    //while (1) ; // If we fail to communicate, loop forever.
  } else {
    Serial.println("Connected 0");
  }

  if (!sx01.begin(SX1509_ADDRESS_01) ) {
    Serial.println("Failed 01");
    Serial.println(SX1509_ADDRESS_01);
  } else {
    Serial.println("Connected 1");
  }

  int count = 0;
  if (!sx02.begin(SX1509_ADDRESS_11)) {
    Serial.println("Failed sx 2");
    Serial.println(SX1509_ADDRESS_11);
  } else {
    Serial.println("Connected 2");
  }

  if (!sx03.begin(SX1509_ADDRESS_10)) {
    Serial.println("Failed sx 3");
    Serial.println(SX1509_ADDRESS_10);
  } else {
    Serial.println("Connected 3");
  }


  delay(2000);
  digitalWrite(13, LOW);

  //sx 00
  sx01.pinMode(SWITCH_01_SX01, INPUT);
  sx01.pinMode(SWITCH_02_SX01, INPUT);
  sx01.pinMode(SWITCH_03_SX01, INPUT);
  sx01.pinMode(SWITCH_04_SX01, INPUT);

  sx01.pinMode(DOWN_01_SX01, INPUT);
  sx01.pinMode(DOWN_02_SX01, INPUT);
  sx01.pinMode(DOWN_03_SX01, INPUT);
  sx01.pinMode(DOWN_04_SX01, INPUT);

  sx01.pinMode(UP_01_SX01, INPUT);
  sx01.pinMode(UP_02_SX01, INPUT);
  sx01.pinMode(UP_03_SX01, INPUT);
  sx01.pinMode(UP_04_SX01, INPUT);

  sx01.pinMode(TOUCH_01_SX01, INPUT);
  sx01.pinMode(TOUCH_02_SX01, INPUT);
  sx01.pinMode(TOUCH_03_SX01, INPUT);
  sx01.pinMode(TOUCH_04_SX01, INPUT);

  //sx 01
  sx02.pinMode(SWITCH_05_SX02, INPUT);
  sx02.pinMode(SWITCH_06_SX02, INPUT);
  sx02.pinMode(SWITCH_07_SX02, INPUT);
  sx02.pinMode(SWITCH_08_SX02, INPUT);

  sx02.pinMode(DOWN_05_SX02, INPUT);
  sx02.pinMode(DOWN_06_SX02, INPUT);
  sx02.pinMode(DOWN_07_SX02, INPUT);
  sx02.pinMode(DOWN_08_SX02, INPUT);

  sx02.pinMode(UP_05_SX02, INPUT);
  sx02.pinMode(UP_06_SX02, INPUT);
  sx02.pinMode(UP_07_SX02, INPUT);
  sx02.pinMode(UP_08_SX02, INPUT);

  sx02.pinMode(TOUCH_05_SX02, INPUT);
  sx02.pinMode(TOUCH_06_SX02, INPUT);
  sx02.pinMode(TOUCH_07_SX02, INPUT);
  sx02.pinMode(TOUCH_08_SX02, INPUT);

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


  //reset and init the neopixels
  for (int i = 0; i < 8; i++) {
    pixels[i] = new Adafruit_NeoPixel(NUMPIXELS, NEO_PIN[i], NEO_GRBW + NEO_KHZ800);
    pixels[i]->begin();
    pixels[i]->clear();
  }

  //set pixels to different colors
  for (int j = 0; j < NUMPIXELS; j++) {
    pixels[0]->setPixelColor(j, pixels[0]->Color(255, 0, 0)); // Moderately bright green color.
  }
  pixels[0]->show();

  for (int j = 0; j < NUMPIXELS; j++) {
    pixels[1]->setPixelColor(j, pixels[1]->Color(255, 255, 0)); // Moderately bright green color.
  }
  pixels[1]->show();

  for (int j = 0; j < NUMPIXELS; j++) {
    pixels[2]->setPixelColor(j, pixels[2]->Color(255, 0, 255)); // Moderately bright green color.
  }
  pixels[2]->show();

  for (int j = 0; j < NUMPIXELS; j++) {
    pixels[3]->setPixelColor(j, pixels[3]->Color(0, 255, 255)); // Moderately bright green color.
  }
  pixels[3]->show();

  for (int j = 0; j < NUMPIXELS; j++) {
    pixels[4]->setPixelColor(j, pixels[4]->Color(0, 255, 0)); // Moderately bright green color.
  }
  pixels[4]->show();

  for (int j = 0; j < NUMPIXELS; j++) {
    pixels[5]->setPixelColor(j, pixels[5]->Color(50, 155, 255)); // Moderately bright green color.
  }
  pixels[5]->show();

  for (int j = 0; j < NUMPIXELS; j++) {
    pixels[6]->setPixelColor(j, pixels[6]->Color(0, 0, 255)); // Moderately bright green color.
  }
  pixels[6]->show();

  for (int j = 0; j < NUMPIXELS; j++) {
    pixels[7]->setPixelColor(j, pixels[7]->Color(255, 255, 255)); // Moderately bright green color.
  }
  pixels[7]->show();

  //status pinTest
  sx03.digitalWrite(STATUS_PIN_SX03, HIGH);
  delay(2000);
  sx03.digitalWrite(STATUS_PIN_SX03, LOW);
}


//main interaction test loop
void loop() {


  //SX 01
  if (sx01.digitalRead(SWITCH_01_SX01) == HIGH) {
    Serial.println("PIN 01");
  }
  if (sx01.digitalRead(SWITCH_02_SX01) == HIGH) {
    Serial.println("PIN 02");
  }
  if (sx01.digitalRead(SWITCH_03_SX01) == HIGH) {
    Serial.println("PIN 03");
  }
  if (sx01.digitalRead(SWITCH_04_SX01) == HIGH) {
    Serial.println("PIN 04");
  }

  if (sx02.digitalRead(SWITCH_05_SX02) == HIGH) {
    Serial.println("PIN 05");
  }
  if (sx02.digitalRead(SWITCH_06_SX02) == HIGH) {
    Serial.println("PIN 06");
  }
  if (sx02.digitalRead(SWITCH_07_SX02) == HIGH) {
    Serial.println("PIN 07");
  }
  if (sx02.digitalRead(SWITCH_08_SX02) == HIGH) {
    Serial.println("PIN 08");
  }


  //SX 02 down
  if (sx01.digitalRead(DOWN_01_SX01) == HIGH) {
    Serial.println("PIN 01 DOWN");
    colorChange = true;
    colorId = 0;
  }
  if (sx01.digitalRead(DOWN_02_SX01) == HIGH) {
    Serial.println("PIN 02 DOWN");
    colorChange = true;
    colorId = 1;
  }
  if (sx01.digitalRead(DOWN_03_SX01) == HIGH) {
    Serial.println("PIN 03 DOWN");
    colorChange = true;
    colorId = 2;
  }
  if (sx01.digitalRead(DOWN_04_SX01) == HIGH) {
    Serial.println("PIN 04 DOWN");
    colorChange = true;
    colorId = 3;
  }

  if (sx02.digitalRead(DOWN_05_SX02) == HIGH) {
    Serial.println("PIN 05 DOWN");
    colorChange = true;
    colorId = 4;
  }
  if (sx02.digitalRead(DOWN_06_SX02) == HIGH) {
    Serial.println("PIN 06 DOWN");
    colorChange = true;
    colorId = 5;
  }
  if (sx02.digitalRead(DOWN_07_SX02) == HIGH) {
    Serial.println("PIN 07 DOWN");
    colorChange = true;
    colorId = 6;
  }
  if (sx02.digitalRead(DOWN_08_SX02) == HIGH) {
    Serial.println("PIN 08 DOWN");
    colorChange = true;
    colorId = 7;
  }
  //sx up
  if (sx01.digitalRead(UP_01_SX01) == HIGH) {
    Serial.println("PIN 01 UP");
  }
  if (sx01.digitalRead(UP_02_SX01) == HIGH) {
    Serial.println("PIN 02 UP");
  }
  if (sx01.digitalRead(UP_03_SX01) == HIGH) {
    Serial.println("PIN 03 UP");
  }
  if (sx01.digitalRead(UP_04_SX01) == HIGH) {
    Serial.println("PIN 04 UP");
  }

  if (sx02.digitalRead(UP_05_SX02) == HIGH) {
    Serial.println("PIN 05 UP");
  }
  if (sx02.digitalRead(UP_06_SX02) == HIGH) {
    Serial.println("PIN 06 UP");
  }
  if (sx02.digitalRead(UP_07_SX02) == HIGH) {
    Serial.println("PIN 07 UP");
  }
  if (sx02.digitalRead(UP_08_SX02) == HIGH) {
    Serial.println("PIN 08 UP");
  }

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
    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < NUMPIXELS; j++) {
        pixels[i]->setPixelColor(j, pixels[i]->Color(255, 0, 255)); // Moderately bright green color.
      }
       pixels[i]->show();
    }
  */

  if (colorChange) {
    int randRed    = random(0, 255);
    int randBlue   = random(0, 255);
    int randGreen  = random(0, 255);
    for (int j = 0; j < NUMPIXELS; j++) {
      pixels[colorId]->setPixelColor(j, pixels[colorId]->Color(randRed, randBlue, randGreen)); // Moderately bright green color.
    }
    pixels[colorId]->show();
    colorChange = false;
  }


}

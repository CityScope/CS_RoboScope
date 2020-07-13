/*************************************************************
  digitalWrite.ino
  SparkFun SX1509 I/O Expander Example: digital out (digitalWrite)
  Jim Lindblom @ SparkFun Electronics
  Original Creation Date: September 21, 2015
  https://github.com/sparkfun/SparkFun_SX1509_Arduino_Library

  This simple example demonstrates the SX1509's digital output
  functionality. Attach an LED to SX1509 IO 15, or just look at
  it with a multimeter. We're gonna blink it!

  Hardware Hookup:
  SX1509 Breakout ------ Arduino -------- Breadboard
        GND -------------- GND
        3V3 -------------- 3.3V
      SDA ------------ SDA (A4)
      SCL ------------ SCL (A5)
      15 -------------------------------- LED+
                                     LED- -/\/\/\- GND
                                                330

  Development environment specifics:
  IDE: Arduino 1.6.5
  Hardware Platform: Arduino Uno
  SX1509 Breakout Version: v2.0

  This code is beerware; if you see me (or any other SparkFun
  employee) at the local, and you've found our code helpful,
  please buy us a round!

  Distributed as-is; no warranty is given.
*************************************************************/

#include <Wire.h> // Include the I2C library (required)
#include <SparkFunSX1509.h> // Include SX1509 library

const byte SX1509_01_SWITCH = 15; // LED connected to pin 8
const byte SX1509_02_SWITCH = 14; // LED connected to pin 9
const byte SX1509_03_SWITCH = 13; // LED connected to pin 10
const byte SX1509_04_SWITCH = 12; // LED connected to pin 11
const byte SX1509_05_SWITCH = 11; // LED connected to pin 12
const byte SX1509_06_SWITCH = 10; // LED connected to pin 13
const byte SX1509_07_SWITCH = 9; // LED connected to pin 14
const byte SX1509_08_SWITCH = 8; // LED connected to pin 15

const byte SX1509_01_SWITCH_LED = 7;
const byte SX1509_02_SWITCH_LED = 6;
const byte SX1509_03_SWITCH_LED = 5;
const byte SX1509_04_SWITCH_LED = 4;
const byte SX1509_05_SWITCH_LED = 3;
const byte SX1509_06_SWITCH_LED = 2;
const byte SX1509_07_SWITCH_LED = 1;
const byte SX1509_08_SWITCH_LED = 0;

// SX1509 I2C address (set by ADDR1 and ADDR0 (00 by default):
const byte SX1509_ADDRESS_00 = 0x3E;  // SX1509 I2C address
SX1509 sx00; // Create an SX1509 object to be used throughout



void setup()
{
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  delay(3000);

  Serial.begin(9600);
  //digitalWrite(13, LOW);

  Serial.println("SX1509:");

  if (!sx00.begin(SX1509_ADDRESS_00) )
  {
    Serial.println(SX1509_ADDRESS_00);
    Serial.println("Failed 00");
    while (1) ; // If we fail to communicate, loop forever.
  } else {
    Serial.println("Connected 00");
  }

  delay(2000);
  digitalWrite(13, LOW);

  sx00.pinMode(SX1509_01_SWITCH, OUTPUT);
  sx00.pinMode(SX1509_02_SWITCH, OUTPUT);
  sx00.pinMode(SX1509_03_SWITCH, OUTPUT);
  sx00.pinMode(SX1509_04_SWITCH, OUTPUT);

  sx00.pinMode(SX1509_05_SWITCH, OUTPUT);
  sx00.pinMode(SX1509_06_SWITCH, OUTPUT);
  sx00.pinMode(SX1509_07_SWITCH, OUTPUT);
  sx00.pinMode(SX1509_08_SWITCH, OUTPUT);

  sx00.pinMode(SX1509_01_SWITCH_LED, OUTPUT);
  sx00.pinMode(SX1509_02_SWITCH_LED, OUTPUT);
  sx00.pinMode(SX1509_03_SWITCH_LED, OUTPUT);
  sx00.pinMode(SX1509_04_SWITCH_LED, OUTPUT);
  sx00.pinMode(SX1509_05_SWITCH_LED, OUTPUT);
  sx00.pinMode(SX1509_06_SWITCH_LED, OUTPUT);
  sx00.pinMode(SX1509_07_SWITCH_LED, OUTPUT);
  sx00.pinMode(SX1509_08_SWITCH_LED, OUTPUT);


}

void loop()
{


}

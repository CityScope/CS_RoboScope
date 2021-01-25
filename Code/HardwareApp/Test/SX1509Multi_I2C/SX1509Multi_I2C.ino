/*************************************************************

   Multi

*************************************************************/

#include <Wire.h> // Include the I2C library (required)
#include <SparkFunSX1509.h> // Include SX1509 library

const byte SX1509_00_W0_00 = 0; // SX pin 1 Wire 0 to address 00
const byte SX1509_01_W0_00 = 1; // SX pin 2 Wire 0 to address 00
const byte SX1509_02_W0_00 = 2; // SX pin 3 Wire 0 to address 00
const byte SX1509_03_W0_00 = 3; // SX pin 4 Wire 0 to address 00
const byte SX1509_04_W0_00 = 4; // SX pin 5 Wire 0 to address 00
const byte SX1509_05_W0_00 = 5; // SX pin 6 Wire 0 to address 00
const byte SX1509_06_W0_00 = 6; // SX pin 7 Wire 0 to address 00
const byte SX1509_07_W0_00 = 7; // SX pin 8 Wire 0 to address 00
const byte SX1509_08_W0_00 = 8; // SX pin 9 Wire 0 to address 00
const byte SX1509_09_W0_00 = 9; // SX pin 10 Wire 0 to address 00
const byte SX1509_10_W0_00 = 10; // SX pin 11 Wire 0 to address 00
const byte SX1509_11_W0_00 = 11; // SX pin 12 Wire 0 to address 00
const byte SX1509_12_W0_00 = 12; // SX pin 13 Wire 0 to address 00
const byte SX1509_13_W0_00 = 13; // SX pin 14 Wire 0 to address 00
const byte SX1509_14_W0_00 = 14; // SX pin 15 Wire 0 to address 00
const byte SX1509_15_W0_00 = 15; // SX pin 16 Wire 0 to address 00

//
const byte SX1509_00_W1_00 = 0; // SX pin 0 Wire 1 to address 00
const byte SX1509_01_W1_00 = 1; // SX pin 1 Wire 1 to address 00
const byte SX1509_02_W1_00 = 2; // SX pin 2 Wire 1 to address 00
const byte SX1509_03_W1_00 = 3; // SX pin 3 Wire 1 to address 00
const byte SX1509_04_W1_00 = 4; // SX pin 4 Wire 1 to address 00
const byte SX1509_05_W1_00 = 5; // SX pin 5 Wire 1 to address 00
const byte SX1509_06_W1_00 = 6; // SX pin 6 Wire 1 to address 00
const byte SX1509_07_W1_00 = 7; // SX pin 7 Wire 1 to address 00
const byte SX1509_08_W1_00 = 8; // SX pin 8 Wire 1 to address 00
const byte SX1509_09_W1_00 = 9; // SX pin 9 Wire 1 to address 00
const byte SX1509_10_W1_00 = 10; // SX pin 10 Wire 1 to address 00
const byte SX1509_11_W1_00 = 11; // SX pin 11 Wire 1 to address 00
const byte SX1509_12_W1_00 = 12; // SX pin 12 Wire 1 to address 00
const byte SX1509_13_W1_00 = 13; // SX pin 13 Wire 1 to address 00
const byte SX1509_14_W1_00 = 14; // SX pin 14 Wire 1 to address 00
const byte SX1509_15_W1_00 = 15; // SX pin 15 Wire 1 to address 00

//
const byte SX1509_00_W1_01 = 0; // SX pin 0 Wire 1 to address 00
const byte SX1509_01_W1_01 = 1; // SX pin 1 Wire 1 to address 01
const byte SX1509_02_W1_01 = 2; // SX pin 2 Wire 1 to address 01
const byte SX1509_03_W1_01 = 3; // SX pin 3 Wire 1 to address 01
const byte SX1509_04_W1_01 = 4; // SX pin 4 Wire 1 to address 01
const byte SX1509_05_W1_01 = 5; // SX pin 5 Wire 1 to address 01
const byte SX1509_06_W1_01 = 6; // SX pin 6 Wire 1 to address 01
const byte SX1509_07_W1_01 = 7; // SX pin 7 Wire 1 to address 01
const byte SX1509_08_W1_01 = 8; // SX pin 8 Wire 1 to address 01
const byte SX1509_09_W1_01 = 9; // SX pin 9 Wire 1 to address 01
const byte SX1509_10_W1_01 = 10; // SX pin 10 Wire 1 to address 01
const byte SX1509_11_W1_01 = 11; // SX pin 11 Wire 1 to address 01
const byte SX1509_12_W1_01 = 12; // SX pin 12 Wire 1 to address 01
const byte SX1509_13_W1_01 = 13; // SX pin 13 Wire 1 to address 01
const byte SX1509_14_W1_01 = 14; // SX pin 14 Wire 1 to address 01
const byte SX1509_15_W1_01 = 15; // SX pin 15 Wire 1 to address 01

//
const byte SX1509_00_W2_00 = 0; // SX pin 0 Wire 2 to address 00
const byte SX1509_01_W2_00 = 1; // SX pin 1 Wire 2 to address 00
const byte SX1509_02_W2_00 = 2; // SX pin 2 Wire 2 to address 00
const byte SX1509_03_W2_00 = 3; // SX pin 3 Wire 2 to address 00
const byte SX1509_04_W2_00 = 4; // SX pin 4 Wire 2 to address 00
const byte SX1509_05_W2_00 = 5; // SX pin 5 Wire 2 to address 00
const byte SX1509_06_W2_00 = 6; // SX pin 6 Wire 2 to address 00
const byte SX1509_07_W2_00 = 7; // SX pin 7 Wire 2 to address 00
const byte SX1509_08_W2_00 = 8; // SX pin 8 Wire 2 to address 00
const byte SX1509_09_W2_00 = 9; // SX pin 9 Wire 2 to address 00
const byte SX1509_10_W2_00 = 10; // SX pin 10 Wire 2 to address 00
const byte SX1509_11_W2_00 = 11; // SX pin 11 Wire 2 to address 00
const byte SX1509_12_W2_00 = 12; // SX pin 12 Wire 2 to address 00
const byte SX1509_13_W2_00 = 13; // SX pin 13 Wire 2 to address 00
const byte SX1509_14_W2_00 = 14; // SX pin 14 Wire 2 to address 00
const byte SX1509_15_W2_00 = 15; // SX pin 15 Wire 2 to address 00


int SX1509_PINS_W0_00 [] = {SX1509_00_W0_00, SX1509_01_W0_00, SX1509_02_W0_00, SX1509_03_W0_00,
                            SX1509_04_W0_00, SX1509_05_W0_00, SX1509_06_W0_00, SX1509_07_W0_00,
                            SX1509_08_W0_00, SX1509_09_W0_00, SX1509_10_W0_00, SX1509_11_W0_00,
                            SX1509_12_W0_00, SX1509_13_W0_00, SX1509_14_W0_00, SX1509_15_W0_00
                           };

int SX1509_PINS_W1_00 [] = {SX1509_00_W1_00, SX1509_01_W1_00, SX1509_02_W1_00, SX1509_03_W1_00,
                            SX1509_04_W1_00, SX1509_05_W1_00, SX1509_06_W1_00, SX1509_07_W1_00,
                            SX1509_08_W1_00, SX1509_09_W1_00, SX1509_10_W1_00, SX1509_11_W1_00,
                            SX1509_12_W1_00, SX1509_13_W1_00, SX1509_14_W1_00, SX1509_15_W1_00
                           };

int SX1509_PINS_W1_01 [] = {SX1509_00_W1_01, SX1509_01_W1_01, SX1509_02_W1_01, SX1509_03_W1_01,
                            SX1509_04_W1_01, SX1509_05_W1_01, SX1509_06_W1_01, SX1509_07_W1_01,
                            SX1509_08_W1_01, SX1509_09_W1_01, SX1509_10_W1_01, SX1509_11_W1_01,
                            SX1509_12_W1_01, SX1509_13_W1_01, SX1509_14_W1_01, SX1509_15_W1_01
                           };

int SX1509_PINS_W2_00 [] = {SX1509_00_W2_00, SX1509_01_W2_00, SX1509_02_W2_00, SX1509_03_W2_00,
                            SX1509_04_W2_00, SX1509_05_W2_00, SX1509_06_W2_00, SX1509_07_W2_00,
                            SX1509_08_W2_00, SX1509_09_W2_00, SX1509_10_W2_00, SX1509_11_W2_00,
                            SX1509_12_W2_00, SX1509_13_W2_00, SX1509_14_W2_00, SX1509_15_W2_00
                           };

// SX1509 I2C address (set by ADDR1 and ADDR0 (00 by default):
const byte SX1509_ADDRESS_00 = 0x3E;  // SX1509 I2C address

// SX1509 I2C address (set by ADDR1 and ADDR0 (00 by default):
const byte SX1509_ADDRESS_01 = 0x3F;

SX1509 sx00; // Create an SX1509 object to be used throughout
SX1509 sx01;
SX1509 sx02;
SX1509 sx03;

void setup() {

  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  delay(3000);

  Serial.begin(9600);


  //
  digitalWrite(13, LOW);
  Serial.println("SX1509 00 Wire 0:");

  if (!sx00.begin(Wire, SX1509_ADDRESS_00) )
  {
    Serial.println(SX1509_ADDRESS_00);
    Serial.println("Failed 00");
    while (1) ; // If we fail to communicate, loop forever.
  } else {
    Serial.println("Connected 00 - ADD 00 - Wire 0");
  }
  digitalWrite(13, HIGH);
  delay(1000);

  //
  digitalWrite(13, LOW);
  Serial.println("SX1509 00 Wire 0:");

  if (!sx01.begin(Wire1, SX1509_ADDRESS_00) )
  {
    Serial.println(SX1509_ADDRESS_00);
    Serial.println("Failed 00");
    while (1) ; // If we fail to communicate, loop forever.
  } else {
    Serial.println("Connected 01 - ADD 00 - Wire 1");
  }
  digitalWrite(13, HIGH);
  delay(1000);
  //

  //
  digitalWrite(13, LOW);
  Serial.println("SX1509 01 Wire 2:");

  if (!sx02.begin(Wire2, SX1509_ADDRESS_01) )
  {
    Serial.println(SX1509_ADDRESS_01);
    Serial.println("Failed 00");
    //while (1) ; // If we fail to communicate, loop forever.
  } else {
    Serial.println("Connected 02 - ADD 01 - Wire 1");
  }
  digitalWrite(13, HIGH);
  delay(1000);

  //
  digitalWrite(13, LOW);
  Serial.println("SX1509 00 Wire 0:");

  if (!sx03.begin(Wire2, SX1509_ADDRESS_00) )
  {
    Serial.println(SX1509_ADDRESS_00);
    Serial.println("Failed 00");
    while (1) ; // If we fail to communicate, loop forever.
  } else {
    Serial.println("Connected 03 - ADD 00 - Wire 2");
  }
  digitalWrite(13, HIGH);
  delay(1000);


  digitalWrite(13, LOW);


  for (int i = 0; i < 16; i++) {
    sx00.pinMode(SX1509_PINS_W0_00[i], INPUT);
    sx01.pinMode(SX1509_PINS_W1_00[i], INPUT);
    sx02.pinMode(SX1509_PINS_W1_01[i], INPUT);
    sx03.pinMode(SX1509_PINS_W2_00[i], INPUT);
  }



}

void loop()
{

  for (int i = 0; i < 15; i++) {
    if (sx00.digitalRead(SX1509_PINS_W0_00[i]) == HIGH) {
      Serial.print("HIGH SX - W0 ADD - 00 ");
      Serial.println(i);
    }

    if (sx01.digitalRead(SX1509_PINS_W1_00[i]) == HIGH) {
      Serial.print("HIGH SX - W1 ADD - 00 ");
      Serial.println(i);
    }

    if (sx02.digitalRead(SX1509_PINS_W1_01[i]) == HIGH) {
      Serial.print("HIGH SX - W1 ADD - 01 ");
      Serial.println(i);
    }

    if (sx03.digitalRead(SX1509_PINS_W2_00[i]) == HIGH) {
      Serial.print("HIGH SX - W2 ADD - 00 ");
      Serial.println(i);
    }

  }


}

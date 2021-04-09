#include <Wire.h> // Include the I2C library (required)
#include <SparkFunSX1509.h> // Include SX1509 library

const byte UP_07_SX01 = 11; // LED connected to pin 8
const byte DOWN_07_SX01 = 12;

const byte DOWN_06_SX01 = 4;
const byte UP_06_SX01 = 3;

const byte SX1509_ADDRESS_01 = 0x3F;
SX1509 sx01;

int hallEffectPin = 11;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  int muxCounter = 0;

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

  sx01.pinMode(DOWN_07_SX01, INPUT);
  sx01.pinMode(UP_07_SX01, INPUT);

  sx01.pinMode(DOWN_06_SX01, INPUT);
  sx01.pinMode(UP_06_SX01, INPUT);

  delay(250);

  pinMode(13, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  int valHall7  = sx01.digitalRead(UP_07_SX01);
  int valButton7 = sx01.digitalRead(DOWN_07_SX01);

  int valHall6  = sx01.digitalRead(UP_06_SX01);
  int valButton6 = sx01.digitalRead(DOWN_06_SX01);


  Serial.print("7: ");
  Serial.print(valHall7);
  Serial.print(" ");
  Serial.println(valButton7);
  //
  Serial.print("6: ");
  Serial.print(valHall6);
  Serial.print(" ");
  Serial.println(valButton6);
}

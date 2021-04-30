#include <Arduino.h>
#include <SparkFunSX1509.h>
#include <Wire.h>

const byte SX1509_ADDRESS_00 = 0x3F;
const byte
SX1509 sx;

void setup() {
  Serial.begin(9600);
  pinMode()

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

  delay(500);
  sx00.debounceTime(2);

  sx00.pinMode(1, INPUT);
  sx00.pinMode(2, INPUT);
  sx00.pinMode(3, INPUT);
  sx00.pinMode(4, INPUT);
}

void loop() {
  Serial.println(sx00.digitalRead(1))
  Serial.println(sx00.digitalRead(1))
  Serial.println(sx00.digitalRead(1))
  Serial.println(sx00.digitalRead(1))

  Serial.println()

}

#include <SparkFunSX1509.h>
#include <Wire.h>

const byte SX1509_ADDRESS_00 = 0x3E;  // SX1509 I2C address
SX1509 sx00;

const byte SX1509_ADDRESS_01 = 0x3F;
SX1509 sx01;

const byte SX1509_ADDRESS_10 = 0x71;
SX1509 sx02;

const byte SX1509_ADDRESS_11 = 0x70;
SX1509 sx03;

void setup() {
  Serial.begin(9600);

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



  delay(500);
  sx01.debounceTime(2);
  sx02.debounceTime(2);

  sx03.pinMode(1, INPUT);

  sx00.pinMode(1, INPUT);
  sx00.pinMode(3, INPUT);
  sx00.pinMode(13, INPUT);
  sx00.pinMode(15, INPUT);


}

void loop() {
  delay(500);
  Serial.println(sx03.digitalRead(1));

  Serial.println(sx00.digitalRead(1));
  Serial.println(sx00.digitalRead(3));
  Serial.println(sx00.digitalRead(13));
  Serial.println(sx00.digitalRead(15));

  Serial.println();

}


/*
Mux 0
sxoo_1,3,13,15

mux 3
pin 1, if high then connected
*/

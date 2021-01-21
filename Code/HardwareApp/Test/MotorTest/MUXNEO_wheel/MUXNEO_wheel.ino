#include <Adafruit_NeoPixel.h>
#include <Wire.h> // Include the I2C library (required)
#include <SparkFunSX1509.h>
#include "Pins.h"

#define PIN 14 //

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(2, PIN, NEO_RGBW + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

//mux
const int selectPins[3] = {15, 16, 17};
const int muxEnable = 20;


const byte SX1509_ADDRESS_01 = 0x3F;
SX1509 sx01;

const byte SX1509_ADDRESS_10 = 0x71;
SX1509 sx02;

const byte SX1509_ADDRESS_11 = 0x70;
SX1509 sx03;

void setup() {
  Serial.begin(9600);


  //MUX
  pinMode(muxEnable, OUTPUT);
  for (int i = 0; i < 3; i++) {
    pinMode(selectPins[i], OUTPUT);
    digitalWrite(selectPins[i], LOW);
  }

  pinMode(14, OUTPUT);



  strip.begin();
  strip.setBrightness(50);
  strip.show(); // Initialize all pixels to 'off'


  Serial.println("Mux SX1509: ");
  delay(500);

  if (!sx01.begin(Wire, SX1509_ADDRESS_01) ) {
    Serial.println("Failed 01");
    Serial.println(SX1509_ADDRESS_01);
  } else {
    Serial.println("Connected 1");
  }
  delay(500);

  int count = 0;
  if (!sx02.begin(Wire, SX1509_ADDRESS_11)) {
    Serial.println("Failed sx 2");
    Serial.println(SX1509_ADDRESS_11);
  } else {
    Serial.println("Connected 2");
  }
  delay(500);

  if (!sx03.begin(Wire, SX1509_ADDRESS_10)) {
    Serial.println("Failed sx 3");
    Serial.println(SX1509_ADDRESS_10);
  } else {
    Serial.println("Connected 3");
  }

  setupSXPins();
}

void loop() {

  /* digitalWrite(14, HIGH);
    delay(5000);

    digitalWrite(14, LOW);
    delay(5000);
  */

  // Some example procedures showing how to display to the pixels:
  //colorWipe(strip.Color(255, 0, 0), 50); // Red
  //colorWipe(strip.Color(0, 255, 0), 50); // Green
  // colorWipe(strip.Color(0, 0, 255), 50); // Blue
  //colorWipe(strip.Color(0, 0, 0, 255), 1); // White RGBW
  // Send a theater pixel chase in...
  // theaterChase(strip.Color(127, 127, 127), 50); // White
  // theaterChase(strip.Color(127, 0, 0), 50); // Red
  // theaterChase(strip.Color(0, 0, 127), 50); // Blue

  //rainbow(20);
  // rainbowCycle(20);
  // theaterChaseRainbow(50);


  digitalWrite(muxEnable, LOW);
  color(strip.Color(0, 0, 255));

  digitalWrite(muxEnable, HIGH);
  rainbowCycle(20);


  checkButtons();
}


// Fill the dots one after the other with a color
void color(uint32_t c) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
  }
  strip.show();
  delay(3000);

}


// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  //0

  for (int j = 0; j < 8; j++) {
    setMUX(j);
    for (j = 0; j < 256; j++) {
      for (i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, Wheel((i + j) & 255));
      }
      strip.show();
      delay(wait);
    }

  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256 * 1; j++) { // 5 cycles of all colors on wheel
    for (int k = 0; k < 8; k++) {
      setMUX(k);
      for (i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
      }
      strip.show();
    }
    //delay(10);

  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j = 0; j < 10; j++) { //do 10 cycles of chasing
    for (int q = 0; q < 3; q++) {
      for (uint16_t i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, c);  //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, 0);      //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j = 0; j < 256; j++) {   // cycle all 256 colors in the wheel
    for (int q = 0; q < 3; q++) {
      for (uint16_t i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, Wheel( (i + j) % 255)); //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, 0);      //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}


void setMUX(int id) {
  //0
  if (id == 0) {
    digitalWrite(selectPins[0], LOW);
    digitalWrite(selectPins[1], LOW);
    digitalWrite(selectPins[2], LOW);
  } else if (id == 1) {  //1
    digitalWrite(selectPins[0], HIGH);
    digitalWrite(selectPins[1], LOW);
    digitalWrite(selectPins[2], LOW);
  } else if (id == 2) {  //2
    digitalWrite(selectPins[0], LOW);
    digitalWrite(selectPins[1], HIGH);
    digitalWrite(selectPins[2], LOW);
  } else if (id == 3) {  //3
    digitalWrite(selectPins[0], HIGH);
    digitalWrite(selectPins[1], HIGH);
    digitalWrite(selectPins[2], LOW);
  } else if (id == 4) {  //4
    digitalWrite(selectPins[0], LOW);
    digitalWrite(selectPins[1], LOW);
    digitalWrite(selectPins[2], HIGH);
  } else if (id == 5) {  //5
    digitalWrite(selectPins[0], HIGH);
    digitalWrite(selectPins[1], LOW);
    digitalWrite(selectPins[2], HIGH);
  } else if (id == 6) {  //6
    digitalWrite(selectPins[0], LOW);
    digitalWrite(selectPins[1], HIGH);
    digitalWrite(selectPins[2], HIGH);
  }  else if (id == 7) {  //6
    digitalWrite(selectPins[0], HIGH);
    digitalWrite(selectPins[1], HIGH);
    digitalWrite(selectPins[2], HIGH);
  }

}

void setupSXPins() {
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
}

void checkButtons() {
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
  }

  if (sx01.digitalRead(DOWN_02_SX01) == HIGH) {
    Serial.println("PIN 02 DOWN");
  }

  if (sx01.digitalRead(DOWN_03_SX01) == HIGH) {
    Serial.println("PIN 03 DOWN");
  }
  if (sx01.digitalRead(DOWN_04_SX01) == HIGH) {
    Serial.println("PIN 04 DOWN");
  }

  if (sx02.digitalRead(DOWN_05_SX02) == HIGH) {
    Serial.println("PIN 05 DOWN");
  }
  if (sx02.digitalRead(DOWN_06_SX02) == HIGH) {
    Serial.println("PIN 06 DOWN");
  }
  if (sx02.digitalRead(DOWN_07_SX02) == HIGH) {
    Serial.println("PIN 07 DOWN");
  }
  if (sx02.digitalRead(DOWN_08_SX02) == HIGH) {
    Serial.println("PIN 08 DOWN");
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
}

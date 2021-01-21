/* WS2812Serial BasicTest_RGBW Example - Works with SK6812 RGBW LEDs

   Test LEDs by turning then 7 different colors.

   This example code is in the public domain. */

#include <WS2812Serial.h>

const int numled = 4;
const int pin = 14;

// Usable pins:
//   Teensy LC:   1, 4, 5, 24
//   Teensy 3.2:  1, 5, 8, 10, 31   (overclock to 120 MHz for pin 8)
//   Teensy 3.5:  1, 5, 8, 10, 26, 32, 33, 48
//   Teensy 3.6:  1, 5, 8, 10, 26, 32, 33
//   Teensy 4.0:  1, 8, 14, 17, 20, 24, 29, 39
//   Teensy 4.1:  1, 8, 14, 17, 20, 24, 29, 35, 47, 53

byte drawingMemory[numled * 4];       //  4 bytes per LED for RGBW
DMAMEM byte displayMemory[numled * 16]; // 16 bytes per LED for RGBW

WS2812Serial leds(numled, displayMemory, drawingMemory, pin, WS2812_RGBW);

#define RED    0x00FF0000
#define GREEN  0x0000FF00
#define BLUE   0x000000FF
#define YELLOW 0x00FFD000
#define PINK   0x44F00080
#define ORANGE 0x00FF4200
#define WHITE  0xAA000000


//mux
const int selectPins[3] = {15, 16, 17};


void setup() {
  Serial.begin(9600);

  for (int i = 0; i < 3; i++) {
    pinMode(selectPins[i], OUTPUT);
    digitalWrite(selectPins[i], LOW);
  }

  digitalWrite(selectPins[0], LOW);
  digitalWrite(selectPins[1], LOW);
  digitalWrite(selectPins[2], LOW);

  leds.begin();
  leds.setBrightness(200); // 0=off, 255=brightest



  Serial.println("done");


}

void loop() {
  // change all the LEDs in 1.5 seconds
  int microsec = 1500000 / leds.numPixels();

  colorWipe(RED, microsec);
  colorWipe(GREEN, microsec);
  colorWipe(BLUE, microsec);
  colorWipe(YELLOW, microsec);
  colorWipe(PINK, microsec);
  colorWipe(ORANGE, microsec);
  colorWipe(WHITE, microsec);
}

void colorWipe(int color, int wait_us) {

  //000
  digitalWrite(selectPins[0], LOW);
  digitalWrite(selectPins[1], LOW);
  digitalWrite(selectPins[2], LOW);
  for (int i = 0; i < leds.numPixels(); i++) {
    leds.setPixel(i, color);
    leds.show();
  }

  // 100
  for (int i = 0; i < leds.numPixels(); i++) {
    leds.setPixel(i, color);
    leds.show();
  }
  digitalWrite(selectPins[0], HIGH);
  digitalWrite(selectPins[1], LOW);
  digitalWrite(selectPins[2], LOW);

  // 010
  for (int i = 0; i < leds.numPixels(); i++) {
    leds.setPixel(i, color);
    leds.show();
  }
  digitalWrite(selectPins[0], LOW);
  digitalWrite(selectPins[1], HIGH);
  digitalWrite(selectPins[2], LOW);

  // 110
  for (int i = 0; i < leds.numPixels(); i++) {
    leds.setPixel(i, color);
    leds.show();
  }
  digitalWrite(selectPins[0], HIGH);
  digitalWrite(selectPins[1], HIGH);
  digitalWrite(selectPins[2], LOW);

  // 001
  for (int i = 0; i < leds.numPixels(); i++) {
    leds.setPixel(i, color);
    leds.show();
  }
  digitalWrite(selectPins[0], LOW);
  digitalWrite(selectPins[1], LOW);
  digitalWrite(selectPins[2], HIGH);

  // 011
  for (int i = 0; i < leds.numPixels(); i++) {
    leds.setPixel(i, color);
    leds.show();
  }
  digitalWrite(selectPins[0], LOW);
  digitalWrite(selectPins[1], HIGH);
  digitalWrite(selectPins[2], HIGH);

  // 101
  for (int i = 0; i < leds.numPixels(); i++) {
    leds.setPixel(i, color);
    leds.show();
  }
  digitalWrite(selectPins[0], HIGH);
  digitalWrite(selectPins[1], LOW);
  digitalWrite(selectPins[2], HIGH);

  // 111
  for (int i = 0; i < leds.numPixels(); i++) {
    leds.setPixel(i, color);
    leds.show();
  }
  digitalWrite(selectPins[0], HIGH);
  digitalWrite(selectPins[1], HIGH);
  digitalWrite(selectPins[2], HIGH);


  delayMicroseconds(300000);
  Serial.println("done");
}

#include <Wire.h> // Include the I2C library (required)
#include <Adafruit_NeoPixel.h>


// SX1509 pin definitions:
const byte NEO_PIN  = 2; // LED connected to pin 15

// How many Neopixels are attached to the Arduino?
#define NUMpixels      16

//pixels
Adafruit_NeoPixel * pixels;


void setup()
{
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);


  delay(5000);
  digitalWrite(13, LOW);


  pixels = new Adafruit_NeoPixel(NUMpixels, NEO_PIN, NEO_GRBW + NEO_KHZ800);
  pixels->begin();
}

void loop()
{
  
  rainbow(50);

}


// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<pixels->numPixels(); i++) {
    pixels->setPixelColor(i, c);
    pixels->show();
    delay(wait);
  }
}

void whiteOverRainbow(uint8_t wait, uint8_t whiteSpeed, uint8_t whiteLength ) {

  if (whiteLength >= pixels->numPixels()) {
    whiteLength = pixels->numPixels() - 1;
  }
  int head = whiteLength - 1;
  int tail = 0;

  int loops = 3;
  int loopNum = 0;

  static unsigned long lastTime = 0;


  while (true) {
    for (int j = 0; j < 256; j++) {
      for (uint16_t i = 0; i < pixels->numPixels(); i++) {
        if ((i >= tail && i <= head) || (tail > head && i >= tail) || (tail > head && i <= head) ) {
          pixels->setPixelColor(i, pixels->Color(0, 0, 0, 255 ) );
        }
        else {
          pixels->setPixelColor(i, Wheel(((i * 256 / pixels->numPixels()) + j) & 255));
        }

      }

      if (millis() - lastTime > whiteSpeed) {
        head++;
        tail++;
        if (head == pixels->numPixels()) {
          loopNum++;
        }
        lastTime = millis();
      }

      if (loopNum == loops) return;

      head %= pixels->numPixels();
      tail %= pixels->numPixels();
      pixels->show();
      delay(wait);
    }
  }

}
void fullWhite() {

  for (uint16_t i = 0; i < pixels->numPixels(); i++) {
    pixels->setPixelColor(i, pixels->Color(0, 0, 0, 255 ) );
  }
  pixels->show();
}


// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on wheel
    for (i = 0; i < pixels->numPixels(); i++) {
      pixels->setPixelColor(i, Wheel(((i * 256 / pixels->numPixels()) + j) & 255));
    }
    pixels->show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return  pixels->Color(255 - WheelPos * 3, 0, WheelPos * 3,0);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return  pixels->Color(0, WheelPos * 3, 255 - WheelPos * 3,0);
  }
  WheelPos -= 170;
  return pixels->Color(WheelPos * 3, 255 - WheelPos * 3, 0,0);
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256; j++) {
    for (i = 0; i < pixels->numPixels(); i++) {
      pixels->setPixelColor(i, Wheel((i + j) & 255));
    }
    pixels->show();
    delay(wait);
  }
}

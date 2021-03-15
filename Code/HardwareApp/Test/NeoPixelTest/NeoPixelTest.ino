#include <Wire.h> // Include the I2C library (required)
#include <Adafruit_NeoPixel.h>
#include <SparkFunSX1509.h>

// SX1509 pin definitions:
const byte NEO_PIN  = 14; // LED connected to pin 15

// How many Neopixels are attached to the Arduino?
#define NUMpixels      4

//pixels
Adafruit_NeoPixel * pixels;

const int selectPins[3] = {11, 13, 15};

const byte SX1509_ADDRESS_00 = 0x3E;  // SX1509 I2C address
SX1509 sx00;

void setup()
{
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  delay(500);

  Serial.println("Mux SX1509: ");

  if (!sx00.begin(SX1509_ADDRESS_00) ) {
    Serial.println("Failed 0");
    Serial.println(SX1509_ADDRESS_00);
    //while (1) ; // If we fail to communicate, loop forever.
  } else {
    Serial.println("Connected 0");
  }
  delay(250);


  for (int i = 0; i < 3; i++) {
    sx00.pinMode(selectPins[i], OUTPUT);
    sx00.digitalWrite(selectPins[i], LOW);
  }

  delay(500);
  digitalWrite(13, LOW);

  //pinMode(NEO_PIN, OUTPUT);


  pixels = new Adafruit_NeoPixel(NUMpixels, NEO_PIN, NEO_RGBW + NEO_KHZ800);
  pixels->begin();

  for (int j = 0; j < 8; j++) {
    enableLED(sx00, j);
    for (int i = 0; i < NUMpixels; i++) {
      pixels->setPixelColor(i, pixels->Color(0, 0, 0, 0));
    }
  }

  enableLED(sx00, 1);

}

void loop()
{

  rainbow(25);

}


// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < pixels->numPixels(); i++) {
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
  if (WheelPos < 85) {
    return  pixels->Color(255 - WheelPos * 3, 0, WheelPos * 3, 0);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return  pixels->Color(0, WheelPos * 3, 255 - WheelPos * 3, 0);
  }
  WheelPos -= 170;
  return pixels->Color(WheelPos * 3, 255 - WheelPos * 3, 0, 0);
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

void enableLED(SX1509 sx, int id) {
  if (id == 0) {
    sx.digitalWrite(selectPins[0], HIGH);
    sx.digitalWrite(selectPins[1], HIGH);
    sx.digitalWrite(selectPins[2], HIGH);
  } else if ( id == 1) {
    sx.digitalWrite(selectPins[0], HIGH);
    sx.digitalWrite(selectPins[1], LOW);
    sx.digitalWrite(selectPins[2], HIGH);
  } else if ( id == 2) {
    sx.digitalWrite(selectPins[0], LOW);
    sx.digitalWrite(selectPins[1], HIGH);
    sx.digitalWrite(selectPins[2], HIGH);
  } else if ( id == 3) {
    sx.digitalWrite(selectPins[0], LOW);
    sx.digitalWrite(selectPins[1], LOW);
    sx.digitalWrite(selectPins[2], HIGH);
  } else if ( id == 4) {
    sx.digitalWrite(selectPins[0], HIGH);
    sx.digitalWrite(selectPins[1], HIGH);
    sx.digitalWrite(selectPins[2], LOW);
  } else if ( id == 5) {
    sx.digitalWrite(selectPins[0], LOW);
    sx.digitalWrite(selectPins[1], HIGH);
    sx.digitalWrite(selectPins[2], LOW);
  } else if ( id == 6) {
    sx.digitalWrite(selectPins[0], HIGH);
    sx.digitalWrite(selectPins[1], LOW);
    sx.digitalWrite(selectPins[2], LOW);
  } else if ( id == 7) {
    sx.digitalWrite(selectPins[0], LOW);
    sx.digitalWrite(selectPins[1], LOW);
    sx.digitalWrite(selectPins[2], LOW);//7
  }
}

/* WS2812Serial BasicTest_RGBW Example - Works with SK6812 RGBW LEDs

   Test LEDs by turning then 7 different colors.

   This example code is in the public domain. */

#include <WS2812Serial.h>
#include <SparkFunSX1509.h>

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
const byte SX1509_ADDRESS_00 = 0x3E;  // SX1509 I2C address
SX1509 sx00;

const int selectPins[3] = {11, 13, 15};


void setup() {
  //Serial.begin(9600);
  delay(500);

  Serial.println("Mux SX1509: ");

  if (!sx00.begin(SX1509_ADDRESS_00) ) {
   // Serial.println("Failed 0");
   // Serial.println(SX1509_ADDRESS_00);
    //while (1) ; // If we fail to communicate, loop forever.
  } else {
   // Serial.println("Connected 0");
  }
  delay(250);


  for (int i = 0; i < 3; i++) {
    sx00.pinMode(selectPins[i], OUTPUT);
    sx00.digitalWrite(selectPins[i], LOW);
  }

  delay(500);

  leds.begin();
  leds.setBrightness(255); // 0=off, 255=brightest
 // Serial.println("done");
}

void loop() {
  // change all the LEDs in 1.5 seconds
  int microsec = 1500000 / leds.numPixels();

  colorWipe(RED, 0);
  colorWipe(GREEN, 1);
  colorWipe(BLUE, 2);
  colorWipe(YELLOW, 3);
  colorWipe(PINK, 4);
  colorWipe(WHITE, 5);
  colorWipe(WHITE, 6);
  colorWipe(WHITE, 7);

  delay(2000);

  colorWipe(BLUE, 0);
  colorWipe(RED, 6);
  //colorWipe(GREEN, 5);
  //colorWipe(BLUE, 4);
  //colorWipe(YELLOW, 3);
  //colorWipe(RED, 2);
  //colorWipe(RED, 1);
  //colorWipe(WHITE, 0);


  delay(2000);

/*
  if (Serial.available() > 0) {
    char key = Serial.read();
    if (key == '1') {
      // clearLEDs();
      enableLED(sx00, 0);
      changeColorLED(sx00, 0x00FF0000);
    }

    if (key == '2') {
      // clearLEDs();
      enableLED(sx00, 1);
      changeColorLED(sx00, 0x00FF0000);
    }

    if (key == '3') {
      //clearLEDs();
      enableLED(sx00, 2);
      changeColorLED(sx00, 0x00FF0000);
    }

    if (key == '4') {
      //clearLEDs();
      enableLED(sx00, 3);
      changeColorLED(sx00, 0x00FF0000);
    }

    if (key == '5') {
      //clearLEDs();
      enableLED(sx00, 4);
      changeColorLED(sx00, 0x00FF0000);
    }
    if (key == '6') {
      //clearLEDs();
      enableLED(sx00, 5);
      changeColorLED(sx00, 0x00FF0000);
    }
    if (key == '7') {
      //clearLEDs();
      enableLED(sx00, 6);
      changeColorLED(sx00, 0x00FF0000);
    }
    if (key == '8') {
      // clearLEDs();
      enableLED(sx00, 7);
      changeColorLED(sx00, 0x00FF0000);
    }
  }
  */
}

void changeColorLED(SX1509 sx, int color) {
  for (int i = 0; i < leds.numPixels(); i++) {
    leds.setPixel(i, color);
    leds.show();
  }
}

void colorWipe(int color, int id) {
  enableLED(sx00, id);
  for (int i = 0; i < leds.numPixels(); i++) {
    leds.setPixel(i, color);
    leds.show();
  }
}
///
void colorWipe(int color) {

  //000
  for (int j = 0; j < 8; j++) {
    enableLED(sx00, j);
    for (int i = 0; i < leds.numPixels(); i++) {
      leds.setPixel(i, color);
      leds.show();
    }
  }
  delayMicroseconds(1000000);
  Serial.println("done");
}

void clearLEDs() {
  for (int j = 0; j < 8; j++) {
    enableLED(sx00, j);
    for (int i = 0; i < leds.numPixels(); i++) {
      leds.setPixel(i, 0x00000000);
      leds.show();
    }
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

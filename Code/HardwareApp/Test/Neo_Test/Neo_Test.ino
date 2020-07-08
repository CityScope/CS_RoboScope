#include <Adafruit_NeoPixel.h>

#define NEO_PIN         20
#define NUMPIXELS       2

#define SWITCH_PIN_01   19
#define SWITCH_PIN_02   21

Adafruit_NeoPixel * pixels;

void setup() {

  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  delay(3000);
  digitalWrite(13, LOW);

 // pinMode(SWITCH_PIN_01, INPUT);
 // pinMode(SWITCH_PIN_02, INPUT);


  pixels = new Adafruit_NeoPixel(NUMPIXELS, NEO_PIN, NEO_GRBW + NEO_KHZ800);
  pixels->begin();
  pixels->clear();
}

void loop() {

  if (digitalRead(SWITCH_PIN_01) == HIGH) {
    Serial.println("pressed PIN 1");
  }
  if (digitalRead(SWITCH_PIN_02) == HIGH) {
   Serial.println("pressed PIN 2");
  }

  
    for (int i = 0; i < NUMPIXELS; i++) {
      pixels->setPixelColor(i, pixels->Color(255, 0, 0, 0)); // Moderately bright green color.
    }
    // This sends the updated pixel color to the hardware.
    pixels->show();
  
}

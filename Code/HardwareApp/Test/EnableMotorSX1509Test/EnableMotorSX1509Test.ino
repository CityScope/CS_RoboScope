#include <Arduino.h>
#include "Stepper.h"
#include "MotorPins.h"
//#include "MotorPinsOld.h"
#include <SparkFunSX1509.h>
#include <Adafruit_NeoPixel.h>


SX1509 sx;


//Motor 1
Stepper * motors[8];

//false -> backwards
//true  -> forwards
boolean dirMotor = false;

//current motor
int currentMotor = 1;

int numMotors = 8;

boolean enableMotor = true;
boolean moveMotor = true;

//--------------------- 1  2  3  4  5  6  7  8
int activateMotors[] = {1, 1, 1, 1, 1, 1, 1, 1};

//index
int indexMotor = 0;

//
int motorDirPins[]    = {DIR_PIN_01, DIR_PIN_02 , DIR_PIN_03, DIR_PIN_04, DIR_PIN_05, DIR_PIN_06, DIR_PIN_07, DIR_PIN_08};
int motorStepPins[]   = {STEP_PIN_01, STEP_PIN_02, STEP_PIN_03, STEP_PIN_04, STEP_PIN_05, STEP_PIN_06, STEP_PIN_07, STEP_PIN_08};

int motorEnablePins[] = {SX1509_01_MENABLE, SX1509_02_MENABLE, SX1509_03_MENABLE, SX1509_04_MENABLE, SX1509_05_MENABLE, SX1509_06_MENABLE, SX1509_07_MENABLE, SX1509_08_MENABLE};
int motorSleepPins[]  = {SX1509_01_MSLEEP, SX1509_02_MSLEEP, SX1509_03_MSLEEP, SX1509_04_MSLEEP, SX1509_05_MSLEEP, SX1509_06_MSLEEP, SX1509_07_MSLEEP, SX1509_08_MSLEEP};


int trq1Mode = 1;
int trq2Mode = 0;

//Maximun number of pixels per board
Adafruit_NeoPixel * pixels[8];

const byte NUMPIXELS = 4;

//NEO PIXELS PINS
const byte NEO_PIN[8] = {NEO_PIN_01, NEO_PIN_02, NEO_PIN_03, NEO_PIN_04,
                         NEO_PIN_05, NEO_PIN_06, NEO_PIN_07, NEO_PIN_08
                        };

//------------------------
void setup() {
  Serial.begin(9600);
  Serial.println("Starting");
  delay(500);

  //
  //Starting mux
  Serial.println("Setting up Enable Motor Mux:");

  //
  int muxCounter = 0;
  while (muxCounter != 3) {
    if (!sx.begin(SX1509_ADDRESS_00) ) {
      Serial.print("Failed 00 ");
      Serial.print(SX1509_ADDRESS_00);
      Serial.print(" ");
      Serial.println(muxCounter);
      delay(100);
      muxCounter++;
    } else {
      Serial.println("Connected 00");
      Serial.print(SX1509_ADDRESS_00);
      Serial.print(" ");
      Serial.println(muxCounter);
      break;
    }
  }

  delay(1000);

  //motor01 = new Stepper(0, GMOTOR_STEPS, DIR_PIN_07, STEP_PIN_07, SX1509_07_MENABLE, SX1509_07_MSLEEP, GM0_PIN, GM1_PIN);

  //------------------
  for (int i = 0; i < numMotors; i++) {
    if (activateMotors[i] ==  1) {
      Serial.print("Setting up: ");
      Serial.println(i);
      motors[i] =  new Stepper(0, GMOTOR_STEPS, motorDirPins[i], motorStepPins[i], motorEnablePins[i], motorSleepPins[i], GM0_PIN, GM1_PIN);

      Serial.println("Starting: ");
      motors[i]->setRPM(GRPM);
      motors[i]->init();
      motors[i]->initMux(sx);
      motors[i]->printMotorInfo();
      Serial.println("-----");
    }
  }

  delay(2000);

  pinMode(TRQ1_PIN, OUTPUT);
  pinMode(TRQ2_PIN, OUTPUT);

  if (trq1Mode == 1) {
    digitalWrite(TRQ1_PIN, HIGH);
  }

  if (trq2Mode == 1) {
    digitalWrite(TRQ2_PIN, HIGH);
  }

  //LED
  for (int i = 0; i < 8; i++) {
    pixels[i] = new Adafruit_NeoPixel(NUMPIXELS, NEO_PIN[i], NEO_GRBW + NEO_KHZ800);
    pixels[i]->begin();
    pixels[i]->clear();
  }
  //set pixels to different colors
  for (int j = 0; j < 8; j++) {
    for (int i = 0; i < NUMPIXELS; i++) {
      pixels[j]->setPixelColor(i, pixels[j]->Color(0, 0, 0, 255)); // Moderately bright green color.
    }
    pixels[j]->show();
  }
  Serial.println("Done LEDs");

}


void loop() {

  //if (motor01->isEnable()) {

  if (moveMotor) {
    //  motor01->moveForward();
  }
  // }

  for (int i = 0; i < numMotors; i++) {
    if (activateMotors[i] ==  1) {
      motors[i]->getNextAction();
    }
  }

  if (Serial.available() > 0) {
    char key = Serial.read();

    if (key == '1') {
      indexMotor = 0;

      if ( motors[indexMotor]->isEnable()) {
        motors[indexMotor]->stop(sx);
      } else {
        motors[indexMotor]->startMoveForward(20);
        motors[indexMotor]->start(sx);
      }
      Serial.println("1");
    }

    if (key == 'Q') {
      indexMotor = 0;
      if ( motors[indexMotor]->isEnable()) {
        motors[indexMotor]->stop(sx);
      } else {
        motors[indexMotor]->startMoveBackward(10);
        motors[indexMotor]->start(sx);
      }
      Serial.println("1");
    }
    ////
    if (key == '2') {
      indexMotor = 1;
      if ( motors[indexMotor]->isEnable()) {
        motors[indexMotor]->stop(sx);
      } else {
        motors[indexMotor]->startMoveForward(20);
        motors[indexMotor]->start(sx);
      }
      Serial.println("2");
    }
    if (key == 'W') {
      indexMotor = 1;
      if ( motors[indexMotor]->isEnable()) {
        motors[indexMotor]->stop(sx);
      } else {
        motors[indexMotor]->startMoveBackward(20);
        motors[indexMotor]->start(sx);
      }
      Serial.println("2");
    }

    ////
    if (key == '3') {
      indexMotor = 2;
      if ( motors[indexMotor]->isEnable()) {
        motors[indexMotor]->stop(sx);
      } else {
        motors[indexMotor]->startMoveForward(20);
        motors[indexMotor]->start(sx);
      }
      Serial.println("3");
    }
    if (key == 'E') {
      indexMotor = 2;
      if ( motors[indexMotor]->isEnable()) {
        motors[indexMotor]->stop(sx);
      } else {
        motors[indexMotor]->startMoveBackward(20);
        motors[indexMotor]->start(sx);
      }
      Serial.println("4");
    }

    ///
    if (key == '4') {
      indexMotor = 3;
      if ( motors[indexMotor]->isEnable()) {
        motors[indexMotor]->stop(sx);
      } else {
        motors[indexMotor]->startMoveForward(20);
        motors[indexMotor]->start(sx);
      }
      Serial.println("5");
    }
    if (key == 'R') {
      indexMotor = 3;
      if ( motors[indexMotor]->isEnable()) {
        motors[indexMotor]->stop(sx);
      } else {
        motors[indexMotor]->startMoveBackward(20);
        motors[indexMotor]->start(sx);
      }
      Serial.println("4");
    }
    ///
    if (key == '5') {
      indexMotor = 4;
      if ( motors[indexMotor]->isEnable()) {
        motors[indexMotor]->stop(sx);
      } else {
        motors[indexMotor]->startMoveForward(20);
        motors[indexMotor]->start(sx);
      }
      Serial.println("6");
    }
    if (key == 'T') {
      indexMotor = 4;
      if ( motors[indexMotor]->isEnable()) {
        motors[indexMotor]->stop(sx);
      } else {
        motors[indexMotor]->startMoveBackward(20);
        motors[indexMotor]->start(sx);
      }
      Serial.println("6");
    }

    //
    if (key == '6') {
      indexMotor = 5;
      if ( motors[indexMotor]->isEnable()) {
        motors[indexMotor]->stop(sx);
      } else {
        motors[indexMotor]->startMoveForward(20);
        motors[indexMotor]->start(sx);
      }
      Serial.println("7");
    }
    if (key == 'Y') {
      indexMotor = 5;
      if ( motors[indexMotor]->isEnable()) {
        motors[indexMotor]->stop(sx);
      } else {
        motors[indexMotor]->startMoveBackward(20);
        motors[indexMotor]->start(sx);
      }
      Serial.println("7");
    }

    //
    if (key == '7') {
      indexMotor = 6;
      if ( motors[indexMotor]->isEnable()) {
        motors[indexMotor]->stop(sx);
      } else {
        motors[indexMotor]->startMoveForward(20);
        motors[indexMotor]->start(sx);
      }
      Serial.println("8");
    }
    if (key == 'U') {
      indexMotor = 6;
      if ( motors[indexMotor]->isEnable()) {
        motors[indexMotor]->stop(sx);
      } else {
        motors[indexMotor]->startMoveBackward(20);
        motors[indexMotor]->start(sx);
      }
      Serial.println("8");
    }

    //
    if (key == '8') {
      indexMotor = 7;
      if ( motors[indexMotor]->isEnable()) {
        motors[indexMotor]->stop(sx);
      } else {
        motors[indexMotor]->startMoveForward(20);
        motors[indexMotor]->start(sx);
      }
      Serial.println("8");
    }
    if (key == 'I') {
      indexMotor = 7;
      if ( motors[indexMotor]->isEnable()) {
        motors[indexMotor]->stop(sx);
      } else {
        motors[indexMotor]->startMoveBackward(20);
        motors[indexMotor]->start(sx);
      }
      Serial.println("8");
    }

    //
    if (key == 'b') {
      for (int i = 0; i < numMotors; i++) {
        if (activateMotors[i] ==  1) {
          motors[i]->startMoveBackward(50);
          motors[i]->start(sx);
          motors[i]->sleepOff(sx);
        }
      }
      Serial.println("ON ALL");
    }


    if (key == 'n') {
      for (int i = 0; i < numMotors; i++) {
        if (activateMotors[i] ==  1) {
          motors[i]->startMoveForward(50);
          motors[i]->start(sx);
          motors[i]->sleepOff(sx);
        }
      }
      Serial.println("ON ALL");
    }

    if (key == 'm') {
      for (int i = 0; i < numMotors; i++) {
        if (activateMotors[i] ==  1) {
          motors[i]->stop(sx);
          motors[i]->sleepOn(sx);
        }
      }
      Serial.println("OFF ALL");
    }

    if (key == 'q') {
      motors[indexMotor]->stop(sx);
      motors[indexMotor]->sleepOn(sx);
      Serial.println("OFF");

    }
    if (key == 'w') {
      motors[indexMotor]->start(sx);
      motors[indexMotor]->sleepOff(sx);
      Serial.println("ON");
    }

    if (key == 'a') {
      motors[indexMotor]->stop(sx);
      Serial.println("stop");
    }

    if (key == 's') {
      for (int j = 0; j < 8; j++) {
        for (int i = 0; i < NUMPIXELS; i++) {
          pixels[j]->setPixelColor(i, pixels[j]->Color(0, 0, 0, 10)); // Moderately bright green color.
        }
        pixels[j]->show();
      }
    }

    if (key == 'z') {
      //motor up and change color

      //indexMotor = 2;
      //motors[indexMotor]->startMoveBackward(1);
      //motors[indexMotor]->start(sx);

      int r = int(random(255));
      int g = int(random(255));
      int b = int(random(255));
      int w = 0;

      if (r >= 254 && g >= 254 && b >= 254) {
        w = 255;
      }
      for (int i = 0; i < NUMPIXELS; i++) {
        pixels[indexMotor]->setPixelColor(i, pixels[indexMotor]->Color(0, 0, 0, w));// White
        pixels[indexMotor]->show();
      }


    }
    if (key == 'x') {
      indexMotor = 2;
      //motors[indexMotor]->startMoveBackward(1);
      //motors[indexMotor]->start(sx);

      int r = int(random(255));
      int g = int(random(255));
      int b = int(random(255));
      int w = 0;

      if (r >= 254 && g >= 254 && b >= 254) {
        w = 255;
      }
      for (int i = 0; i < NUMPIXELS; i++) {
        pixels[indexMotor]->setPixelColor(i, pixels[indexMotor]->Color(r, g, b, w));// White
        pixels[indexMotor]->show();
      }

    }

    if (key == 'c') {
      //motor up and change color

      indexMotor = 0;
      motors[indexMotor]->startMoveForward(1);
      motors[indexMotor]->start(sx);

      for (int i = 0; i < NUMPIXELS; i++) {
        pixels[indexMotor]->setPixelColor(i, pixels[indexMotor]->Color(0, 0, 0, 255));// White
        pixels[indexMotor]->show();
      }


    }
    if (key == 'v') {
      indexMotor = 2;
      motors[indexMotor]->startMoveForward(1);
      motors[indexMotor]->start(sx);

      for (int i = 0; i < NUMPIXELS; i++) {
        pixels[indexMotor]->setPixelColor(i, pixels[indexMotor]->Color(0, 0, 0, 255));// White
        pixels[indexMotor]->show();
      }

    }


    if (key == 'r') {
      trq1Mode = 0;
      digitalWrite(TRQ1_PIN, LOW);
      Serial.println("Amp Mode");
      Serial.print(trq1Mode);
      Serial.print(" ");
      Serial.println(trq2Mode);
    }
    if (key == 't') {
      trq1Mode = 1;
      digitalWrite(TRQ1_PIN, HIGH);
      Serial.println("Amp Mode");
      Serial.print(trq1Mode);
      Serial.print(" ");
      Serial.println(trq2Mode);
    }
    if (key == 'f') {
      trq2Mode = 0;
      digitalWrite(TRQ2_PIN, LOW);
      Serial.println("Amp Mode");
      Serial.print(trq1Mode);
      Serial.print(" ");
      Serial.println(trq2Mode);
    }
    if (key == 'g') {
      trq2Mode = 1;
      digitalWrite(TRQ2_PIN, HIGH);
      Serial.println("Amp Mode");
      Serial.print(trq1Mode);
      Serial.print(" ");
      Serial.println(trq2Mode);
    }


  }
}

#include <Arduino.h>
#include <Wire.h> /

#include "Stepper.h"
#include "Pins.h"
//#include "MotorPinsOld.h"

#include <SparkFunSX1509.h>
#include <Adafruit_NeoPixel.h>
#include <Wire.h> /
#include <FlexCAN_T4.h>

FlexCAN_T4FD<CAN3, RX_SIZE_512, TX_SIZE_64> canFD;
FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16>   can2;

//
//NEO PIXELS PINS
const byte NEO_PIN[8] = {NEO_PIN_01, NEO_PIN_02, NEO_PIN_03, NEO_PIN_04,
                         NEO_PIN_05, NEO_PIN_06, NEO_PIN_07, NEO_PIN_08
                        };

//Maximun number of pixels per board
const byte NUMPIXELS = 4;
//Pixels

Adafruit_NeoPixel * pixels[8];
boolean colorChange = false;
int colorId = -1;

//Motor commands, step calibration
SX1509 sx00;

//limit and push down buttons
SX1509 sx01;
bool interruptActivate01 = false;

//push push interaction and touch enable
SX1509 sx02;
bool interruptActivate02 = false;

//dip switch  and notifications
SX1509 sx03;

//change if the buttons are using the interrupt funtion
bool interrupts_pins = false;

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
int motorDirPins[]     =  {DIR_PIN_01, DIR_PIN_02 , DIR_PIN_03, DIR_PIN_04, DIR_PIN_05, DIR_PIN_06, DIR_PIN_07, DIR_PIN_08};
int motorStepPins[]    =  {STEP_PIN_01, STEP_PIN_02, STEP_PIN_03, STEP_PIN_04, STEP_PIN_05, STEP_PIN_06, STEP_PIN_07, STEP_PIN_08};
int motorEnablePins[]  =  {ENABLE_01_SX00, ENABLE_02_SX00, ENABLE_03_SX00, ENABLE_04_SX00, ENABLE_05_SX00, ENABLE_06_SX00, ENABLE_07_SX00, ENABLE_08_SX00};

int trq1Mode = 1;
int trq2Mode = 0;

int dir = 1;
//------------------------
void setup() {
  Serial.begin(9600);
  Serial.println("Starting");
  delay(500);

  //
  //Starting mux
  Serial.println("Setting up Enable Motor Mux:");

  int muxCounter = 0;
  while (muxCounter != 3) {
    if (!sx00.begin(SX1509_ADDRESS_00) ) {
      Serial.print("Failed 00 ");
      Serial.print(" " + SX1509_ADDRESS_00);
      Serial.print(" ");
      Serial.println(muxCounter);
      delay(100);
      muxCounter++;
    } else {
      Serial.println("Connected 00");
      Serial.print(" " + SX1509_ADDRESS_00);
      Serial.print(" ");
      Serial.println(muxCounter);
      break;
    }
  }
  muxCounter = 0;
  delay(500);
  ///--------------------------------------------

  while (muxCounter != 3) {
    if (!sx01.begin(SX1509_ADDRESS_01) ) {
      Serial.print("Failed 01");
      Serial.print(" " + SX1509_ADDRESS_01);
      Serial.print(" ");
      Serial.println(muxCounter);
      delay(100);
      muxCounter++;
    } else {
      Serial.println("Connected 01 ");
      Serial.print("" + SX1509_ADDRESS_01);
      Serial.print(" ");
      Serial.println(muxCounter);
      break;
    }
  }
  muxCounter = 0;
  delay(500);
  ///--------------------------------------------

  while (muxCounter != 3) {
    if (!sx02.begin(SX1509_ADDRESS_10) ) {
      Serial.print("Failed 02");
      Serial.print(" " + SX1509_ADDRESS_10);
      Serial.print(" ");
      Serial.println(muxCounter);
      delay(100);
      muxCounter++;
    } else {
      Serial.println("Connected 02 ");
      Serial.print("" + SX1509_ADDRESS_10);
      Serial.print(" ");
      Serial.println(muxCounter);
      break;
    }
  }
  muxCounter = 0;
  delay(500);
  //---------------------------------------

  while (muxCounter != 3) {
    if (!sx03.begin(SX1509_ADDRESS_11) ) {
      Serial.print("Failed 03");
      Serial.print(" " + SX1509_ADDRESS_11);
      Serial.print(" ");
      Serial.println(muxCounter);
      delay(100);
      muxCounter++;
    } else {
      Serial.println("Connected 03");
      Serial.print("" + SX1509_ADDRESS_11);
      Serial.print(" ");
      Serial.println(muxCounter);
      break;
    }
  }
  muxCounter = 0;
  delay(500);

  //-----------------------
  initSX02();
  delay(200);
  Serial.println("done init pins SX 02");

  initSX01();
  delay(200);
  Serial.println("done init pins SX 01");

  initSX03();
  delay(100);
  Serial.println("done init pins SX 03");
  //----------------------

  //Init motors
  for (int i = 0; i < numMotors; i++) {
    if (activateMotors[i] ==  1) {
      Serial.print("Setting up: ");
      Serial.println(i);
      motors[i] =  new Stepper(0, GMOTOR_STEPS, motorDirPins[i], motorStepPins[i], motorEnablePins[i]);

      Serial.println("Starting: ");
      motors[i]->setRPM(GRPM);
      motors[i]->init();
      motors[i]->initMux(sx00);
      motors[i]->printMotorInfo();
      Serial.println("-----");
    }
  }
  delay(500);
  ///

  //NEO PIXELS
  //reset and init the neopixels
  for (int i = 0; i < 8; i++) {
    pixels[i] = new Adafruit_NeoPixel(NUMPIXELS, NEO_PIN[i], NEO_GRBW + NEO_KHZ800);
    pixels[i]->begin();
    pixels[i]->clear();
  }
  //set pixels to different colors
  for (int j = 0; j < 8; j++) {
    pixels[j]->setPixelColor(0, pixels[j]->Color(0, 0, 0, 255)); // Moderately bright green color.
    pixels[j]->setPixelColor(1, pixels[j]->Color(0, 0, 0, 255)); // Moderately bright green color.
    pixels[j]->show();
  }
  Serial.println("Done LEDs");

  //////////
  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN_SWITCH_01), checkSXO1_interrupts, CHANGE);
  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN_SWITCH_02), checkSXO2_interrupts, CHANGE);



  //
  //setup CAN Bus FD
  canFD.begin();
  CANFD_timings_t config;
  config.clock = CLK_24MHz;
  config.baudrate = 1000000;
  config.baudrateFD = 2000000;
  config.propdelay = 190;
  config.bus_length = 1;
  config.sample = 87.5;
  canFD.setRegions(64);
  canFD.setBaudRate(config);
  canFD.mailboxStatus();
  //

  //setup CAN BUS 2.0
  can2.begin();
  can2.setBaudRate(500000);     // 500kbps data rate
  can2.enableFIFO();
  can2.enableFIFOInterrupt();
  can2.onReceive(FIFO, canBusSniff);
  can2.mailboxStatus();

  Serial.println("Done Can Bus");
  delay(200);

  Serial.println("done");
}


void loop() {

  //if (motor01->isEnable()) {

  /*if (moveMotor) {
    //  motor01->moveForward();
    }
    // }

    for (int i = 0; i < numMotors; i++) {
    if (activateMotors[i] ==  1) {
      motors[i]->getNextAction();
    }
    }
  */
  ///

  checkLimit();
  checkPushDown();
  //checkPushUp();
  checkDip();


  ///Limit and push buttons
  //checkLimit();
  //checkPushDown();

  //CAN BUS
  /*
    CANFD_message_t msg;
    if (canFD.read(msg)) {
    Serial.println(" ");
    reading(msg);
    }

  */
  //keyCommands
  keyCommands();

}



///---------------------------------------------------------
//SX 01
void checkLimit() {
  if (sx01.digitalRead(SWITCH_01_SX01) == HIGH) {
    Serial.println("LIMIT PIN 01");
  }
  if (sx01.digitalRead(SWITCH_02_SX01) == HIGH) {
    Serial.println("LIMIT PIN 02");
  }
  if (sx01.digitalRead(SWITCH_03_SX01) == HIGH) {
    Serial.println("LIMIT PIN 03");
  }
  if (sx01.digitalRead(SWITCH_04_SX01) == HIGH) {
    Serial.println("LIMIT PIN 04");
  }
  if (sx02.digitalRead(SWITCH_05_SX02) == HIGH) {
    Serial.println("LIMIT PIN 05");
  }
  if (sx02.digitalRead(SWITCH_06_SX02) == HIGH) {
    Serial.println("LIMIT PIN 06");
  }
  if (sx02.digitalRead(SWITCH_07_SX02) == HIGH) {
    Serial.println("LIMIT PIN 07");
  }
  if (sx02.digitalRead(SWITCH_08_SX02) == HIGH) {
    Serial.println("LIMIT PIN 08");
  }
}

void checkDip() {
  if (sx03.digitalRead(DIP_01_SX03) == HIGH) {
    Serial.println("DIP PIN 01");
  }
  if (sx03.digitalRead(DIP_02_SX03) == HIGH) {
    Serial.println("DIP PIN 02");
  }
  if (sx03.digitalRead(DIP_03_SX03) == HIGH) {
    Serial.println("DIP PIN 03");
  }
  if (sx03.digitalRead(DIP_04_SX03) == HIGH) {
    Serial.println("DIP PIN 04");
  }
  if (sx03.digitalRead(DIP_05_SX03) == HIGH) {
    Serial.println("DIP PIN 05");
  }
  if (sx03.digitalRead(DIP_06_SX03) == HIGH) {
    Serial.println("DIP PIN 06");
  }
  if (sx03.digitalRead(DIP_07_SX03) == HIGH) {
    Serial.println("DIP PIN 07");
  }
  if (sx03.digitalRead(DIP_08_SX03) == HIGH) {
    Serial.println("DIP PIN 08");
  }
  if (sx03.digitalRead(DIP_09_SX03) == HIGH) {
    Serial.println("DIP PIN 09");
  }
  if (sx03.digitalRead(DIP_10_SX03) == HIGH) {
    Serial.println("DIP PIN 10");
  }

}

void checkPushDown() {
  //SX 02 down
  if (sx01.digitalRead(DOWN_01_SX01) == HIGH) {
    Serial.println("PIN 01 DOWN");
    colorChange = true;
    colorId = 0;
  }
  if (sx01.digitalRead(DOWN_02_SX01) == HIGH) {
    Serial.println("PIN 02 DOWN");
    colorChange = true;
    colorId = 1;
  }
  if (sx01.digitalRead(DOWN_03_SX01) == HIGH) {
    Serial.println("PIN 03 DOWN");
    colorChange = true;
    colorId = 2;
  }
  if (sx01.digitalRead(DOWN_04_SX01) == HIGH) {
    Serial.println("PIN 04 DOWN");
    colorChange = true;
    colorId = 3;
  }
  if (sx02.digitalRead(DOWN_05_SX02) == HIGH) {
    Serial.println("PIN 05 DOWN");
    colorChange = true;
    colorId = 4;
  }
  if (sx02.digitalRead(DOWN_06_SX02) == HIGH) {
    Serial.println("PIN 06 DOWN");
    colorChange = true;
    colorId = 5;
  }
  if (sx02.digitalRead(DOWN_07_SX02) == HIGH) {
    Serial.println("PIN 07 DOWN");
    colorChange = true;
    colorId = 6;
  }
  if (sx02.digitalRead(DOWN_08_SX02) == HIGH) {
    Serial.println("PIN 08 DOWN");
    colorChange = true;
    colorId = 7;
  }

  if (colorChange) {
    int randRed    = random(0, 255);
    int randBlue   = random(0, 255);
    int randGreen  = random(0, 255);
    for (int j = 0; j < NUMPIXELS; j++) {
      pixels[colorId]->setPixelColor(j, pixels[colorId]->Color(randRed, randBlue, randGreen)); // Moderately bright green color.
    }
    pixels[colorId]->show();
    colorChange = false;
  }
}


//-------------------------------------------------------------------
void checkPushUp() {
  if (sx01.digitalRead(UP_01_SX01) == LOW) {
    Serial.println("LIMIT UP PIN 01");
  }
  if (sx01.digitalRead(UP_02_SX01) == LOW) {
    Serial.println("LIMIT UP PIN 02");
  }
  if (sx01.digitalRead(UP_03_SX01) == LOW) {
    Serial.println("LIMIT UP PIN 03");
  }
  if (sx01.digitalRead(UP_04_SX01) == LOW) {
    Serial.println("LIMIT UP PIN 04");
  }
  if (sx02.digitalRead(UP_05_SX02) == LOW) {
    Serial.println("LIMIT UP PIN 05");
  }
  if (sx02.digitalRead(UP_06_SX02) == LOW) {
    Serial.println("LIMIT UP PIN 06");
  }
  if (sx02.digitalRead(UP_07_SX02) == LOW) {
    Serial.println("LIMIT UP PIN 07");
  }
  if (sx02.digitalRead(UP_08_SX02) == LOW) {
    Serial.println("LIMIT UP PIN 08");
  }

}

//------------------------------------------------------------------
void canBusSniff(const CAN_message_t &msg) { // global callback
  digitalWrite(LED_STATUS, HIGH);

  Serial.print("T4: ");
  Serial.print("MB "); Serial.print(msg.mb);
  Serial.print(" OVERRUN: "); Serial.print(msg.flags.overrun);
  Serial.print(" BUS "); Serial.print(msg.bus);
  Serial.print(" LEN: "); Serial.print(msg.len);
  Serial.print(" EXT: "); Serial.print(msg.flags.extended);
  Serial.print(" REMOTE: "); Serial.print(msg.flags.remote);
  Serial.print(" TS: "); Serial.print(msg.timestamp);
  Serial.print(" ID: "); Serial.print(msg.id, HEX);
  Serial.print(" IDHIT: "); Serial.print(msg.idhit);

  Serial.print(" Buffer: ");
  for ( uint8_t i = 0; i < msg.len; i++ ) {
    Serial.print(msg.buf[i], HEX); Serial.print(" ");
  } Serial.println();

  digitalWrite(LED_STATUS, LOW); //CAN Bus FD

}

//----------------------------------------------------------------
void reading(CANFD_message_t msg) {
  //turn on LED
  digitalWrite(LED_STATUS, HIGH);
  int node = msg.id & 0xF;
  int local = (msg.id >> 4) & 0xF;
  //print CAN BUS data
  Serial.print("  ID: ");
  Serial.print("  Node: ");
  Serial.print(node);
  Serial.print("  Local: ");
  Serial.print(local);
  Serial.print("  LEN: ");
  Serial.print(msg.len);
  Serial.print(" DATA: ");

  for ( uint8_t i = 0; i < msg.len; i++ ) {
    Serial.print(msg.buf[i]);
    Serial.print(" ");
  }

  //turn off LED
  digitalWrite(LED_STATUS, LOW);
}

void keyCommands() {
  if (Serial.available() > 0) {
    char key = Serial.read();

    if (key == '1') {
      indexMotor = 0;

      if ( motors[indexMotor]->isEnable()) {
        motors[indexMotor]->stop(sx00);
      } else {
        motors[indexMotor]->startMoveForward(10);
        motors[indexMotor]->start(sx00);
      }
      Serial.println("1");
    }
    if (key == '2') {
      indexMotor = 1;
      if ( motors[indexMotor]->isEnable()) {
        motors[indexMotor]->stop(sx00);
      } else {
        motors[indexMotor]->startMoveForward(10);
        motors[indexMotor]->start(sx00);
      }
      Serial.println("2");
    }
    if (key == '3') {
      indexMotor = 2;
      if ( motors[indexMotor]->isEnable()) {
        motors[indexMotor]->stop(sx00);
      } else {
        motors[indexMotor]->startMoveForward(10);
        motors[indexMotor]->start(sx00);
      }
      Serial.println("3");
    }
    if (key == '4') {
      indexMotor = 3;
      if ( motors[indexMotor]->isEnable()) {
        motors[indexMotor]->stop(sx00);
      } else {
        motors[indexMotor]->startMoveForward(10);
        motors[indexMotor]->start(sx00);
      }
      Serial.println("4");
    }
    if (key == '5') {
      indexMotor = 4;
      if ( motors[indexMotor]->isEnable()) {
        motors[indexMotor]->stop(sx00);
      } else {
        motors[indexMotor]->startMoveForward(10);
        motors[indexMotor]->start(sx00);
      }
      Serial.println("5");
    }
    if (key == '6') {
      indexMotor = 5;
      if ( motors[indexMotor]->isEnable()) {
        motors[indexMotor]->stop(sx00);
      } else {
        motors[indexMotor]->startMoveForward(10);
        motors[indexMotor]->start(sx00);
      }
      Serial.println("6");
    }
    if (key == '7') {
      indexMotor = 6;
      if ( motors[indexMotor]->isEnable()) {
        motors[indexMotor]->stop(sx00);
      } else {
        motors[indexMotor]->startMoveForward(10);
        motors[indexMotor]->start(sx00);
      }
      Serial.println("7");
    }
    if (key == '8') {
      indexMotor = 7;
      if ( motors[indexMotor]->isEnable()) {
        motors[indexMotor]->stop(sx00);
      } else {
        motors[indexMotor]->startMoveForward(10);
        motors[indexMotor]->start(sx00);
      }
      Serial.println("8");
    }

    //
    if (key == 'q') {
      indexMotor = 0;

      if ( motors[indexMotor]->isEnable()) {
        motors[indexMotor]->stop(sx00);
      } else {
        motors[indexMotor]->startMoveBackward(10);
        motors[indexMotor]->start(sx00);
      }
      Serial.println("1");
    }
    if (key == 'w') {
      indexMotor = 1;
      if ( motors[indexMotor]->isEnable()) {
        motors[indexMotor]->stop(sx00);
      } else {
        motors[indexMotor]->startMoveBackward(10);
        motors[indexMotor]->start(sx00);
      }
      Serial.println("2");
    }
    if (key == 'e') {
      indexMotor = 2;
      if ( motors[indexMotor]->isEnable()) {
        motors[indexMotor]->stop(sx00);
      } else {
        motors[indexMotor]->startMoveBackward(10);
        motors[indexMotor]->start(sx00);
      }
      Serial.println("3");
    }
    if (key == 'r') {
      indexMotor = 3;
      if ( motors[indexMotor]->isEnable()) {
        motors[indexMotor]->stop(sx00);
      } else {
        motors[indexMotor]->startMoveBackward(10);
        motors[indexMotor]->start(sx00);
      }
      Serial.println("4");
    }
    if (key == 't') {
      indexMotor = 4;
      if ( motors[indexMotor]->isEnable()) {
        motors[indexMotor]->stop(sx00);
      } else {
        motors[indexMotor]->startMoveBackward(10);
        motors[indexMotor]->start(sx00);
      }
      Serial.println("5");
    }
    if (key == 'y') {
      indexMotor = 5;
      if ( motors[indexMotor]->isEnable()) {
        motors[indexMotor]->stop(sx00);
      } else {
        motors[indexMotor]->startMoveBackward(20);
        motors[indexMotor]->start(sx00);
      }
      Serial.println("6");
    }
    if (key == 'u') {
      indexMotor = 6;
      if ( motors[indexMotor]->isEnable()) {
        motors[indexMotor]->stop(sx00);
      } else {
        motors[indexMotor]->startMoveBackward(20);
        motors[indexMotor]->start(sx00);
      }
      Serial.println("7");
    }
    if (key == 'i') {
      indexMotor = 7;
      if ( motors[indexMotor]->isEnable()) {
        motors[indexMotor]->stop(sx00);
      } else {
        motors[indexMotor]->startMoveBackward(10);
        motors[indexMotor]->start(sx00);
      }
      Serial.println("8");
    }
    //----------------


    if (key == 'b') {
      for (int i = 0; i < numMotors; i++) {
        if (activateMotors[i] ==  1) {
          motors[i]->startMoveBackward(10);
          motors[i]->start(sx00);
          motors[i]->sleepOff(sx00);
        }
      }
      Serial.println("ON ALL");
    }


    if (key == 'n') {
      for (int i = 0; i < numMotors; i++) {
        if (activateMotors[i] ==  1) {
          motors[i]->startMoveForward(10);
          motors[i]->start(sx00);
          motors[i]->sleepOff(sx00);
        }
      }
      Serial.println("ON ALL");
    }

    if (key == 'm') {
      for (int i = 0; i < numMotors; i++) {
        if (activateMotors[i] ==  1) {
          motors[i]->stop(sx00);
          motors[i]->sleepOn(sx00);
        }
      }
      Serial.println("OFF ALL");
    }


    //single
    if (key == ' ') {
      motors[indexMotor]->stop(sx00);
      motors[indexMotor]->sleepOn(sx00);
      Serial.println("OFF");
    }
    if (key == 'a') {
      motors[indexMotor]->stop(sx00);
      Serial.println("stop");
    }

    if (key == 's') {
      motors[indexMotor]->start(sx00);
      Serial.println("start");
    }
    if (key == 'd') {
      motors[indexMotor]->startMoveForward(5);
      Serial.println("fowards");
    }
    if (key == 'z') {
      motors[indexMotor]->sleepOn(sx00);
      Serial.println("sleep ON");
    }
    if (key == 'x') {
      motors[indexMotor]->sleepOff(sx00);
      Serial.println("sleep OFF");
    }
    //

    //modes
    if (key == 'f') {

    }
    if (key == 'g') {

    }
    if (key == 'h') {

    }
    if (key == 'j') {

    }

    //steps
    if (key == 'o') {

    }

    if (key == 'p') {

    }

    //LEDS
    if (key == 'k') {
      //reset and init the neopixels
      for (int i = 0; i < 8; i++) {
        pixels[i] = new Adafruit_NeoPixel(NUMPIXELS, NEO_PIN[i], NEO_GRBW + NEO_KHZ800);
        pixels[i]->begin();
        pixels[i]->clear();
      }
      //set pixels to different colors
      for (int j = 0; j < 8; j++) {
        pixels[j]->setPixelColor(0, pixels[j]->Color(0, 0, 0, 255));// White
        pixels[j]->setPixelColor(1, pixels[j]->Color(0, 0, 0, 255));// White
        pixels[j]->show();
      }

      Serial.println("Done LEDs");
    }

    if (key == 'l') {
      //set pixels to different colors
      int r = int(random(255));
      int g = int(random(255));
      int b = int(random(255));
      int w = 0;

      if (r >= 254 && g >= 254 && b >= 254) {
        w = 255;
      }

      for (int j = 0; j < 8; j++) {
        pixels[j]->setPixelColor(0, pixels[j]->Color(r, g, b, w));
        pixels[j]->setPixelColor(1, pixels[j]->Color(r, g, b, w));
        pixels[j]->show();
      }
    }
  }
}

void initSX01() {
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
}

void initSX02() {

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
  sx02.pinMode(UP_08_SX02,  INPUT);

  sx02.pinMode(TOUCH_05_SX02, INPUT);
  sx02.pinMode(TOUCH_06_SX02, INPUT);
  sx02.pinMode(TOUCH_07_SX02, INPUT);
  sx02.pinMode(TOUCH_08_SX02, INPUT);
}

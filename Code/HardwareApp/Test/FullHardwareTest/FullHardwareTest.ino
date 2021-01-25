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
const byte NUMPIXELS = 2;
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
    if (!sx00.begin(Wire, SX1509_ADDRESS_00) ) {
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
    if (!sx01.begin(Wire, SX1509_ADDRESS_01) ) {
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
    if (!sx02.begin(Wire, SX1509_ADDRESS_10) ) {
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
    if (!sx03.begin(Wire, SX1509_ADDRESS_11) ) {
      Serial.print("Failed 03 ");
      Serial.print(" " + SX1509_ADDRESS_11);
      Serial.print(" ");
      Serial.println(muxCounter);
      delay(100);
      muxCounter++;
    } else {
      Serial.println("Connected 03 ");
      Serial.print("" + SX1509_ADDRESS_11);
      Serial.print(" ");
      Serial.println(muxCounter);
      break;
    }
  }
  muxCounter = 0;
  delay(200);

  //-----------------------
  initSX01();
  delay(100);
  Serial.println("done init pins SX 01");

  initSX03();
  delay(100);
  Serial.println("done init pins SX 02");

  initSX03();
  delay(100);
  Serial.println("done init pins SX 03");
  //----------------------

  //Init motors
  for (int i = 0; i < numMotors; i++) {
    if (activateMotors[i] ==  1) {
      Serial.print("Setting up: ");
      Serial.println(i);
      motors[i] =  new Stepper(0, GMOTOR_STEPS, motorDirPins[i], motorStepPins[i], motorEnablePins[i], GM0_PIN, GM1_PIN);

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
  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN_SWITCH_01), checkSXO1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN_SWITCH_02), checkSXO2, CHANGE);



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

  //Pin Modes
  pinMode(TRQ1_PIN, OUTPUT);
  pinMode(TRQ2_PIN, OUTPUT);

  if (trq1Mode == 1) {
    digitalWrite(TRQ1_PIN, HIGH);
  }

  if (trq2Mode == 1) {
    digitalWrite(TRQ2_PIN, HIGH);
  }

  sx00.pinMode(GM1_PIN, OUTPUT);
  sx00.digitalWrite(GM1_PIN, HIGH);

  Serial.println("done");
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

  ///
  updateSX01();
  updateSX02();

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

//sx 01
void initSX01() {
  sx01.pinMode(SWITCH_01_SX01, INPUT_PULLUP);
  sx01.pinMode(SWITCH_02_SX01, INPUT_PULLUP);
  sx01.pinMode(SWITCH_03_SX01, INPUT_PULLUP);
  sx01.pinMode(SWITCH_04_SX01, INPUT_PULLUP);

  sx01.pinMode(DOWN_01_SX01, INPUT_PULLUP);
  sx01.pinMode(DOWN_02_SX01, INPUT_PULLUP);
  sx01.pinMode(DOWN_03_SX01, INPUT_PULLUP);
  sx01.pinMode(DOWN_04_SX01, INPUT_PULLUP);

  sx01.pinMode(UP_01_SX01, INPUT_PULLUP);
  sx01.pinMode(UP_02_SX01, INPUT_PULLUP);
  sx01.pinMode(UP_03_SX01, INPUT_PULLUP);
  sx01.pinMode(UP_04_SX01, INPUT_PULLUP);

  sx01.pinMode(TOUCH_01_SX01, INPUT_PULLUP);
  sx01.pinMode(TOUCH_02_SX01, INPUT_PULLUP);
  sx01.pinMode(TOUCH_03_SX01, INPUT_PULLUP);
  sx01.pinMode(TOUCH_04_SX01, INPUT_PULLUP);

  //interrupt
  sx01.enableInterrupt(SWITCH_01_SX01, CHANGE);
  sx01.enableInterrupt(SWITCH_02_SX01, CHANGE);
  sx01.enableInterrupt(SWITCH_03_SX01, CHANGE);
  sx01.enableInterrupt(SWITCH_04_SX01, CHANGE);

  sx01.enableInterrupt(DOWN_01_SX01, CHANGE);
  sx01.enableInterrupt(DOWN_02_SX01, CHANGE);
  sx01.enableInterrupt(DOWN_03_SX01, CHANGE);
  sx01.enableInterrupt(DOWN_04_SX01, CHANGE);

  sx01.enableInterrupt(UP_01_SX01, CHANGE);
  sx01.enableInterrupt(UP_02_SX01, CHANGE);
  sx01.enableInterrupt(UP_03_SX01, CHANGE);
  sx01.enableInterrupt(UP_04_SX01, CHANGE);

  sx01.enableInterrupt(TOUCH_01_SX01, CHANGE);
  sx01.enableInterrupt(TOUCH_02_SX01, CHANGE);
  sx01.enableInterrupt(TOUCH_03_SX01, CHANGE);
  sx01.enableInterrupt(TOUCH_04_SX01, CHANGE);

  //debouncePin
  sx01.debounceTime(4);

  sx01.debouncePin(SWITCH_01_SX01);
  sx01.debouncePin(SWITCH_02_SX01);
  sx01.debouncePin(SWITCH_03_SX01);
  sx01.debouncePin(SWITCH_04_SX01);

  sx01.debouncePin(DOWN_01_SX01);
  sx01.debouncePin(DOWN_02_SX01);
  sx01.debouncePin(DOWN_03_SX01);
  sx01.debouncePin(DOWN_04_SX01);

  sx01.debouncePin(UP_01_SX01);
  sx01.debouncePin(UP_02_SX01);
  sx01.debouncePin(UP_03_SX01);
  sx01.debouncePin(UP_04_SX01);

  sx01.debouncePin(TOUCH_01_SX01);
  sx01.debouncePin(TOUCH_02_SX01);
  sx01.debouncePin(TOUCH_03_SX01);
  sx01.debouncePin(TOUCH_04_SX01);

  pinMode(INTERRUPT_PIN_SWITCH_01, INPUT_PULLUP);
}

//sx 02
void initSX02() {

  sx02.pinMode(SWITCH_05_SX02, INPUT_PULLUP);
  sx02.pinMode(SWITCH_06_SX02, INPUT_PULLUP);
  sx02.pinMode(SWITCH_07_SX02, INPUT_PULLUP);
  sx02.pinMode(SWITCH_08_SX02, INPUT_PULLUP);

  sx02.pinMode(DOWN_05_SX02, INPUT_PULLUP);
  sx02.pinMode(DOWN_06_SX02, INPUT_PULLUP);
  sx02.pinMode(DOWN_07_SX02, INPUT_PULLUP);
  sx02.pinMode(DOWN_08_SX02, INPUT_PULLUP);

  sx02.pinMode(UP_05_SX02, INPUT_PULLUP);
  sx02.pinMode(UP_06_SX02, INPUT_PULLUP);
  sx02.pinMode(UP_07_SX02, INPUT_PULLUP);
  sx02.pinMode(UP_08_SX02,  INPUT_PULLUP);

  sx02.pinMode(TOUCH_05_SX02, INPUT_PULLUP);
  sx02.pinMode(TOUCH_06_SX02, INPUT_PULLUP);
  sx02.pinMode(TOUCH_07_SX02, INPUT_PULLUP);
  sx02.pinMode(TOUCH_08_SX02, INPUT_PULLUP);

  //interrupt
  sx02.enableInterrupt(SWITCH_05_SX02, CHANGE);
  sx02.enableInterrupt(SWITCH_06_SX02, CHANGE);
  sx02.enableInterrupt(SWITCH_07_SX02, CHANGE);
  sx02.enableInterrupt(SWITCH_08_SX02, CHANGE);

  sx02.enableInterrupt(DOWN_05_SX02, CHANGE);
  sx02.enableInterrupt(DOWN_06_SX02, CHANGE);
  sx02.enableInterrupt(DOWN_07_SX02, CHANGE);
  sx02.enableInterrupt(DOWN_08_SX02, CHANGE);

  sx02.enableInterrupt(UP_05_SX02, CHANGE);
  sx02.enableInterrupt(UP_06_SX02, CHANGE);
  sx02.enableInterrupt(UP_07_SX02, CHANGE);
  sx02.enableInterrupt(UP_08_SX02, CHANGE);

  sx02.enableInterrupt(TOUCH_05_SX02, CHANGE);
  sx02.enableInterrupt(TOUCH_06_SX02, CHANGE);
  sx02.enableInterrupt(TOUCH_07_SX02, CHANGE);
  sx02.enableInterrupt(TOUCH_08_SX02, CHANGE);

  //debouncePin
  sx02.debounceTime(4);

  sx02.debouncePin(SWITCH_05_SX02);
  sx02.debouncePin(SWITCH_06_SX02);
  sx02.debouncePin(SWITCH_07_SX02);
  sx02.debouncePin(SWITCH_08_SX02);

  sx02.debouncePin(DOWN_05_SX02);
  sx02.debouncePin(DOWN_06_SX02);
  sx02.debouncePin(DOWN_07_SX02);
  sx02.debouncePin(DOWN_08_SX02);

  sx02.debouncePin(UP_05_SX02);
  sx02.debouncePin(UP_06_SX02);
  sx02.debouncePin(UP_07_SX02);
  sx02.debouncePin(UP_08_SX02);

  sx02.debouncePin(TOUCH_05_SX02);
  sx02.debouncePin(TOUCH_06_SX02);
  sx02.debouncePin(TOUCH_07_SX02);
  sx02.debouncePin(TOUCH_08_SX02);

  pinMode(INTERRUPT_PIN_SWITCH_02, INPUT_PULLUP);
}

//SX03
void initSX03() {
  sx03.pinMode(DIP_01_SX03, INPUT);
  sx03.pinMode(DIP_02_SX03, INPUT);
  sx03.pinMode(DIP_03_SX03, INPUT);
  sx03.pinMode(DIP_04_SX03, INPUT);
  sx03.pinMode(DIP_05_SX03, INPUT);
  sx03.pinMode(DIP_06_SX03, INPUT);
  sx03.pinMode(DIP_07_SX03, INPUT);
  sx03.pinMode(DIP_08_SX03, INPUT);
  sx03.pinMode(DIP_09_SX03, INPUT);
  sx03.pinMode(DIP_10_SX03, INPUT);
}

//---------------------------------------------
void checkSXO1() {
  interruptActivate01 = true;
}
void checkSXO2() {
  interruptActivate02 = true;
}
//---------------------------------------------
void updateSX01() {
  if (interruptActivate01) {
    Serial.println("interrupt sx01");

    unsigned int intStatus = sx01.interruptSource();
    Serial.println("intStatus = " + String(intStatus, BIN));

    if (bool(intStatus & (1 << SWITCH_01_SX01)) == HIGH) {
      Serial.println("LIMIT PIN 01");
    }
    if (bool(intStatus & (1 << SWITCH_02_SX01)) == HIGH) {
      Serial.println("LIMIT PIN 02");
    }
    if (bool(intStatus & (1 << SWITCH_03_SX01)) == HIGH) {
      Serial.println("LIMIT PIN 03");
    }
    if (bool(intStatus & (1 << SWITCH_04_SX01)) == HIGH) {
      Serial.println("LIMIT PIN 04");
    }

    //SX 02 down
    if (bool(intStatus & (1 << DOWN_01_SX01)) == HIGH) {
      Serial.println("PIN 01 DOWN");
      colorChange = true;
      colorId = 0;
    }
    if (bool(intStatus & (1 << DOWN_02_SX01)) == HIGH) {
      Serial.println("PIN 02 DOWN");
      colorChange = true;
      colorId = 1;
    }
    if (bool(intStatus & (1 << DOWN_03_SX01)) == HIGH) {
      Serial.println("PIN 03 DOWN");
      colorChange = true;
      colorId = 2;
    }
    if (bool(intStatus & (1 << DOWN_04_SX01)) == HIGH) {
      Serial.println("PIN 04 DOWN");
      colorChange = true;
      colorId = 3;
    }
    interruptActivate01 = false;
  }
}
//----------------------------------------------------
void updateSX02() {
  if (interruptActivate02) {
    Serial.println("interrupt sx02");

    unsigned int intStatus = sx02.interruptSource();
    Serial.println("intStatus = " + String(intStatus, BIN));

    //
    if (intStatus & (1 << SWITCH_05_SX02) == HIGH) {
      Serial.println("LIMIT PIN 05");
    }
    if (intStatus & (1 << SWITCH_06_SX02) == HIGH) {
      Serial.println("LIMIT PIN 06");
    }
    if (intStatus & (1 << SWITCH_07_SX02) == HIGH) {
      Serial.println("LIMIT PIN 07");
    }
    if (intStatus & (1 << SWITCH_08_SX02) == HIGH) {
      Serial.println("LIMIT PIN 08");
    }

    //SX 02 down
    if (intStatus & (1 << DOWN_05_SX02) == HIGH) {
      Serial.println("PIN 05 DOWN");
      colorChange = true;
      colorId = 4;
    }
    if (intStatus & (1 << DOWN_06_SX02) == HIGH) {
      Serial.println("PIN 06 DOWN");
      colorChange = true;
      colorId = 5;
    }
    if (intStatus & (1 << DOWN_07_SX02) == HIGH) {
      Serial.println("PIN 07 DOWN");
      colorChange = true;
      colorId = 6;
    }
    if (intStatus & (1 << DOWN_08_SX02) == HIGH) {
      Serial.println("PIN 08 DOWN");
      colorChange = true;
      colorId = 7;
    }
    interruptActivate02 = false;
  }

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
      trq1Mode = 0;
      digitalWrite(TRQ1_PIN, LOW);
      Serial.println("Amp Mode");
      Serial.print(trq1Mode);
      Serial.print(" ");
      Serial.println(trq2Mode);
    }
    if (key == 'g') {
      trq1Mode = 1;
      digitalWrite(TRQ1_PIN, HIGH);
      Serial.println("Amp Mode");
      Serial.print(trq1Mode);
      Serial.print(" ");
      Serial.println(trq2Mode);
    }
    if (key == 'h') {
      trq2Mode = 0;
      digitalWrite(TRQ2_PIN, LOW);
      Serial.println("Amp Mode");
      Serial.print(trq1Mode);
      Serial.print(" ");
      Serial.println(trq2Mode);
    }
    if (key == 'j') {
      trq2Mode = 1;
      digitalWrite(TRQ2_PIN, HIGH);
      Serial.println("Amp Mode");
      Serial.print(trq1Mode);
      Serial.print(" ");
      Serial.println(trq2Mode);
    }

    //steps
    if (key == 'o') {
      sx00.digitalWrite(GM1_PIN, HIGH);
      Serial.print("GM1 HIGH 16");
    }

    if (key == 'p') {
      sx00.digitalWrite(GM1_PIN, LOW);
      Serial.print("GM1 HIGH 8");
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

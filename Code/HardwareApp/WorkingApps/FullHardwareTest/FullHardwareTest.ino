#include <Arduino.h>
#include <Wire.h>

#include "StepperMotor.h"
#include "Pins.h"

//SX
#include <SparkFunSX1509.h>
#include <Adafruit_NeoPixel.h>
#include <Wire.h>

#include "message.h"

//CAN BUS
#include <FlexCAN_T4.h>

//EEPROM
#include <EEPROM.h>

//CAN BUS
FlexCAN_T4FD<CAN3, RX_SIZE_512, TX_SIZE_64> canFD;
FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16> can2;


//NEO PIXELS PINS
const byte NEO_PIN_UP[8] = { NEO_PIN_01_UP, NEO_PIN_02_UP, NEO_PIN_03_UP, NEO_PIN_04_UP,
                             NEO_PIN_05_UP, NEO_PIN_06_UP, NEO_PIN_07_UP, NEO_PIN_08_UP };

//NEO PIXELS PINS
const byte NEO_PIN_DOWN[8] = { NEO_PIN_01_DOWN, NEO_PIN_02_DOWN, NEO_PIN_03_DOWN, NEO_PIN_04_DOWN,
                               NEO_PIN_05_DOWN, NEO_PIN_06_DOWN, NEO_PIN_07_DOWN, NEO_PIN_08_DOWN };


//Pixels
struct rgb {
  uint8_t r;
  uint8_t g;
  uint8_t b;
};


Adafruit_NeoPixel* pixels[8];

boolean colorChange = false;
int colorId = -1;
rgb tempRGB[8];  //store temp current color

//MUX
//Motor commands, step calibration
SX1509* sx00;
bool activeSX00 = false;

//limit and push down buttons
SX1509* sx01;
bool activeSX01 = false;

//push push interaction and touch enable
SX1509* sx02;
bool activeSX02 = false;

//dip switch  and notifications
SX1509* sx03;
bool activeSX03 = false;

//Motors
StepperMotor* motors[8];
bool activeCalibration = false;
//DRV8880 class
TMC2209Stepper* driver;

//
int motorDirPins[] = { DIR_PIN_01, DIR_PIN_02, DIR_PIN_03, DIR_PIN_04, DIR_PIN_05, DIR_PIN_06, DIR_PIN_07, DIR_PIN_08 };
int motorStepPins[] = { STEP_PIN_01, STEP_PIN_02, STEP_PIN_03, STEP_PIN_04, STEP_PIN_05, STEP_PIN_06, STEP_PIN_07, STEP_PIN_08 };
int motorEnablePins[] = { ENABLE_01_SX00, ENABLE_02_SX00, ENABLE_03_SX00, ENABLE_04_SX00, ENABLE_05_SX00, ENABLE_06_SX00, ENABLE_07_SX00, ENABLE_08_SX00 };
//------------------------
//Motor Mux
const int selectMotor[3] = { 5, 7, 9 };
const int motorEnable = 15;
//

//number form the Dip
int dipNumber = 0;

//Node Id
int nodeId = 0;  //in case dip number is different from node id
//

bool sendPos = false;

void setup() {
  Serial.print("begin");
  //Start Serial
  Serial.begin(250000);
  delay(500);

  Serial.println("Starting: ");

  //Starting mux
  initMUX();

  //-----------------------
  Serial.println("...");
  initSX00();

  Serial.println("...");
  initSX01();

  Serial.println("...");
  initSX02();

  Serial.println("...");
  initSX03();
  //----------------------

  //Init motors
  Serial.println("....");
  initMotors();
  ///

  //init Calibration
  Serial.println(".....");
  initCalibration();
  //

  //get Node Number Dip
  Serial.println("......");
  checkDip();

  //NEO PIXELS
  Serial.println("......");
  initPixels();

  //
  //setup CAN Bus FD
  Serial.println("......");
  initCanBus();

  Serial.println("done setup");
  Serial.println(".............");


  // Serial.println("Initialize positions");
  // Serial.println("......");
  // initPositions();

}


void loop() {

  // motorSequence();
  motorUpdate();
  // motorCalibration();

  //swithces
  // checkLimit();

  checkPushDown();
  // checkPushUp();
  // checkDip();

  //calibration Module
  // updateCalibration();

  //CAN BUS
  canFD.events();
  // if (sendPos){
  //   sendLastPos();
  //   sendPos = false;
  // }

  //keyCommands
  keyCommands();

}

void initPositions() { // activated with key 0
  for (int i = 0; i < NUM_3D_PIXELS; i++) {
    // Serial.println(i);
    motors[i]->enableSequence = false;
    motors[i]->updatePos = true;
    enableMotor(i);
    motors[i]->motorOn();
    motors[i]->shaftOn(driver);
    motors[i]->start(sx01, driver);
  }
  // motors[5]->enableSequence = false;
  // motors[5]->updatePos = true;
  // enableMotor(5);
  // motors[5]->motorOn();
  // motors[5]->shaftOn(driver);
  // motors[5]->start(sx01, driver);

  // motors[3]->enableSequence = false;
  // motors[3]->updatePos = true;
  // enableMotor(3);
  // motors[3]->motorOn();
  // motors[3]->shaftOn(driver);
  // motors[3]->start(sx01, driver);
  bool a = false;
  bool b = false;
  // bool b = true;
  bool c = false;
  bool d = false;
  bool e = false;
  bool f = false;
  bool g = false;
  bool h = false;
  while (!a || !b || !c || !d || !e || !f || !g || !h){
    if (!a){ // added because motor 7 always ended up stopping when all were driven together
      motors[1]->shaftOn(driver);
    }
    if (!b){ // added because motor 7 always ended up stopping when all were driven together
      motors[0]->shaftOn(driver);
    }
    if (!c){ // added because motor 7 always ended up stopping when all were driven together
      motors[3]->shaftOn(driver);
    }
    if (!d){ // added because motor 7 always ended up stopping when all were driven together
      motors[2]->shaftOn(driver);
    }
    if (!e){ // added because motor 7 always ended up stopping when all were driven together
      motors[5]->shaftOn(driver);
    }
    if (!f){ // added because motor 7 always ended up stopping when all were driven together
      motors[4]->shaftOn(driver);
    }
    if (!g){ // added because motor 7 always ended up stopping when all were driven together
      motors[7]->shaftOn(driver);
    }
    if (!h){ // added because motor 7 always ended up stopping when all were driven together
      motors[6]->shaftOn(driver);
    }
    if (Serial.available() > 0) {
      char key = Serial.read();

      //------- toggle direction
      if (key == '1') {
        Serial.println(a);
        Serial.println(b);
        Serial.println(c);
        Serial.println(d);
        Serial.println(e);
        Serial.println(f);
        Serial.println(g);
        Serial.println(h);
      }
      if (key == '2') {
        Serial.println("Motor positions");
        Serial.println(motors[1]->currPos);
        Serial.println(motors[0]->currPos);
        Serial.println(motors[3]->currPos);
        Serial.println(motors[2]->currPos);
        Serial.println(motors[5]->currPos);
        Serial.println(motors[4]->currPos);
        Serial.println(motors[7]->currPos);
        Serial.println(motors[6]->currPos);
      }
    }
    motorUpdate();
    if (!a && activeSX01 && sx01->digitalRead(SWITCH_01_SX01) == HIGH) {
      Serial.println("LIMIT PIN 01");
      motors[1]->shaftOff(driver);
      // motors[1]->stop(sx01, driver);
      motors[1]->motorOff();
      motors[1]->currPos = BUTTON_POS;
      // motors[1]->startSequence(driver, 0);
      a = true;
    }
    if (!b && activeSX01 && sx01->digitalRead(SWITCH_02_SX01) == HIGH) {
      Serial.println("LIMIT PIN 02");
      motors[0]->shaftOff(driver);
      // motors[0]->stop(sx01, driver);
      motors[0]->motorOff();
      motors[0]->currPos = BUTTON_POS;
      // motors[0]->startSequence(driver, 0);
      b = true;
    }
    if (!c && activeSX01 && sx01->digitalRead(SWITCH_03_SX01) == HIGH) {
      Serial.println("LIMIT PIN 03");
      motors[3]->shaftOff(driver);
      // motors[3]->stop(sx01, driver);
      motors[3]->motorOff();
      motors[3]->currPos = BUTTON_POS;
      // motors[3]->startSequence(driver, 0);
      c = true;
    }
    if (!d && activeSX01 && sx01->digitalRead(SWITCH_04_SX01) == HIGH) {
      Serial.println("LIMIT PIN 04");
      motors[2]->shaftOff(driver);
      // motors[2]->stop(sx01, driver);
      motors[2]->motorOff();
      motors[2]->currPos = BUTTON_POS;
      // motors[2]->startSequence(driver, 0);
      d = true;
    }
    if (!e && activeSX02 && sx02->digitalRead(SWITCH_05_SX02) == HIGH) {
      Serial.println("LIMIT PIN 05");
      motors[5]->shaftOff(driver);
      // motors[5]->stop(sx01, driver);
      motors[5]->motorOff();
      motors[5]->currPos = BUTTON_POS;
      // motors[5]->startSequence(driver, 0);
      e = true;
    }
    if (!f && activeSX02 && sx02->digitalRead(SWITCH_06_SX02) == HIGH) {
      Serial.println("LIMIT PIN 06");
      motors[4]->shaftOff(driver);
      // motors[4]->stop(sx01, driver);
      motors[4]->motorOff();
      motors[4]->currPos = BUTTON_POS;
      // motors[4]->startSequence(driver, 0);
      f = true;
    }
    if (!g && activeSX02 && sx02->digitalRead(SWITCH_07_SX02) == HIGH) {
      Serial.println("LIMIT PIN 07");
      motors[7]->shaftOff(driver);
      // motors[7]->stop(sx01, driver);
      motors[7]->motorOff();
      motors[7]->currPos = BUTTON_POS;
      // motors[7]->startSequence(driver, 0);
      g = true;
    }
    if (!h && activeSX02 && sx02->digitalRead(SWITCH_08_SX02) == HIGH) {
      Serial.println("LIMIT PIN 08");
      motors[6]->shaftOff(driver);
      // motors[6]->stop(sx01, driver);
      motors[6]->motorOff();
      motors[6]->currPos = BUTTON_POS;
      // motors[6]->startSequence(driver, 0);
      h = true;
    }
  }
  for (int i = 0; i < NUM_3D_PIXELS; i++) {
    enableMotor(i);
    motors[i]->startSequence(driver, 0);
    motors[i]->motorOn();
    motors[i]->start(sx01, driver);
  }
  // enableMotor(5);
  // motors[5]->startSequence(driver, 0);
  // motors[5]->motorOn();
  // motors[5]->start(sx01, driver);

  // enableMotor(3);
  // motors[3]->startSequence(driver, 0);
  // motors[3]->motorOn();
  // motors[3]->start(sx01, driver);
}

//--PIXELS
//-------------------------------------------------------------------
void initPixels() {
  Serial.println("Init LEDs");
  if (nodeId >= 6) {
    for (int i = 0; i < NUM_3D_PIXELS; i++) {
      pixels[i] = new Adafruit_NeoPixel(NUM_LEDS, NEO_PIN_DOWN[i], NEO_GRBW + NEO_KHZ800);
      pixels[i]->begin();
      pixels[i]->clear();
    }
  } else {
    for (int i = 0; i < NUM_3D_PIXELS; i++) {
      pixels[i] = new Adafruit_NeoPixel(NUM_LEDS, NEO_PIN_UP[i], NEO_GRBW + NEO_KHZ800);
      pixels[i]->begin();
      pixels[i]->clear();
    }
  }
  //set pixels to different colors
  for (uint8_t j = 0; j < NUM_3D_PIXELS; j++) {
    for (uint8_t i = 0; i < NUM_LEDS; i++) {
      pixels[j]->setPixelColor(i, pixels[j]->Color(0, 0, 0, 255));  // Moderately bright green color.
    }
    pixels[j]->show();
  }
  delay(200);
  Serial.println("Done LEDs");
}

//-------------------------------------------------------------------
void initMotors() {
  Serial.println("Setting up Motor Control");

  driver = new TMC2209Stepper(&SERIAL_PORT, R_SENSE, DRIVER_ADDRESS);
  SERIAL_PORT.begin(250000);
  driver->begin();

  for (uint8_t i = 0; i < NUM_3D_PIXELS; i++) {
    Serial.print(" Motor init: ");
    Serial.println(i);

    enableMotor(i);
    motors[i] = new StepperMotor(i, MICRO_STEPS, motorDirPins[i], motorStepPins[i], motorEnablePins[i]);
    motors[i]->initPins(sx00, activeSX00);
    motors[i]->initMotor(driver);
    motors[i]->shaftOff(driver);
    motors[i]->motorOff();
    motors[i]->stop(sx01, driver);
    Serial.println("-----");
    delay(10);
  }

  //stop all motors
  for (int i = 0; i < NUM_3D_PIXELS; i++) {
    enableMotor(i);
    motors[i]->stop(sx01, driver);
  }
  Serial.println("Deactive Motor");

  Serial.println("Done Motors");
}
//-------------------------------------------------------------------
//MOTOR UPDATE BLOCKING WITH DELAY
void motorUpdate() {
  if (!motors[1]->isCalibrating()) {
    for (uint16_t i = 1000; i > 0; i--) {
      sendPos = true;
      for (int j = 0; j < NUM_3D_PIXELS; j++) {
        if (motors[j]->currPos == motors[j]->targetPos){
          motors[j]->targetReached = true;
        } else {
          sendPos = false;
        }
        if (!motors[j]->isSequence() || !motors[j]->targetReached){
          motors[j]->motorBegin();
        }
      }
      delayMicroseconds(4);

      for (int j = 0; j < NUM_3D_PIXELS; j++) {
        motors[j]->motorEnd();
      }
      delayMicroseconds(4);
    }
    for (int j = 0; j < NUM_3D_PIXELS; j++) {
      // if (motors[j]->enableMotor){
      //   Serial.print("Motor ");
      //   Serial.print(j);
      //   Serial.print(": ");
      //   Serial.println(motors[j]->enableSequence);
      //   Serial.println(motors[j]->dirMotor);
      // }
      motors[j]->incPos(driver, sx00, sx01);
        // if (motors[j]->dirMotor){
        //   motors[j]->currPos--; // goes down
        // } else {
        //   motors[j]->currPos++; // goes up
        // }
      // Serial.print("Motor ");
      // Serial.print(j);
      // Serial.print(": ");
      // Serial.println(motors[j]->getCurrPos());
    }

    if (motors[1]->isSequence()) {
      //int currPos = motors[1]->getCurrPos();
      //int targetPos = motors[1]->getTargetPos();
      //Serial.print("seq ");
      //Serial.print(1);
      //Serial.print(" ");
      //Serial.println(currPos);
      // Serial.print(" ");
      //Serial.println(targetPos);
    }
  }
}

void sendLastPos(){
  // Serial.println("Sending last position");
  CANMotorMessage msg = CANMotorMessage(12); // 12 will be id of server
  for (int i=0;i<NUM_3D_PIXELS;i++){
    if (!motors[i]->enableMotor){
      // Serial.print("Sending last position of motor ");
      // Serial.println(i);
      uint8_t cp = map(motors[i]->currPos, motors[i]->minPos, motors[i]->maxPos, 0, 255);
      // Serial.println(cp);
      msg.addMessage(i, cp, 10, 11, 12); // destination multiplied by 4 as messages are 4 bytes
    }
  }
  //Serial.println(nodeId);
  // for (int i=0;i<8;i++){
  //   Serial.print(i);
  //   Serial.print(" position: ");
  //   Serial.println(msg.getCANmessage(MSG_SIZE).buf[i*4+1]);
  // }
  canFD.write(msg.getCANmessage(MSG_SIZE));
}

///--------------------------------------------------
void motorCalibration() {

  if (motors[1]->isCalibrating()) {

    for (uint16_t i = 1000; i > 0; i--) {
      for (int j = 0; j < NUM_3D_PIXELS; j++) {
        motors[j]->motorBegin();
      }
      delayMicroseconds(4);

      for (int j = 0; j < NUM_3D_PIXELS; j++) {
        motors[j]->motorEnd();
      }
      delayMicroseconds(4);
    }

    for (int j = 0; j < NUM_3D_PIXELS; j++) {
      if (motors[j]->isEnable()) {
        motors[j]->incCalibrationSteps();
      }
    }
  }
}

//-------------------------------------------------------------------
void toggleMotor(int motorId) {
  enableMotor(motorId);
  motors[motorId]->toggleMotor();
  Serial.print("Motor toggle: ");
  Serial.println(motorId);
  // Serial.println(motors[motorId]->stepsCounter);
}

void toggleMotorShaft(int motorId) {
  enableMotor(motorId);
  motors[motorId]->toggleShaft(driver);
  motors[motorId]->testConnection(driver);
}
//-------------------------------------------------------------------
void enableMotor(int id) {
  if (id == 0) {
    sx00->digitalWrite(selectMotor[0], LOW);
    sx00->digitalWrite(selectMotor[1], LOW);
    sx00->digitalWrite(selectMotor[2], LOW);
  } else if (id == 1) {
    sx00->digitalWrite(selectMotor[0], HIGH);
    sx00->digitalWrite(selectMotor[1], LOW);
    sx00->digitalWrite(selectMotor[2], LOW);
  } else if (id == 2) {
    sx00->digitalWrite(selectMotor[0], LOW);
    sx00->digitalWrite(selectMotor[1], HIGH);
    sx00->digitalWrite(selectMotor[2], LOW);
  } else if (id == 3) {
    sx00->digitalWrite(selectMotor[0], HIGH);
    sx00->digitalWrite(selectMotor[1], HIGH);
    sx00->digitalWrite(selectMotor[2], LOW);
  } else if (id == 4) {
    sx00->digitalWrite(selectMotor[0], LOW);
    sx00->digitalWrite(selectMotor[1], LOW);
    sx00->digitalWrite(selectMotor[2], HIGH);
  } else if (id == 5) {
    sx00->digitalWrite(selectMotor[0], HIGH);
    sx00->digitalWrite(selectMotor[1], LOW);
    sx00->digitalWrite(selectMotor[2], HIGH);
  } else if (id == 6) {
    sx00->digitalWrite(selectMotor[0], LOW);
    sx00->digitalWrite(selectMotor[1], HIGH);
    sx00->digitalWrite(selectMotor[2], HIGH);
  } else if (id == 7) {
    sx00->digitalWrite(selectMotor[0], HIGH);
    sx00->digitalWrite(selectMotor[1], HIGH);
    sx00->digitalWrite(selectMotor[2], HIGH);
  }
}

//--- INIT MUX
//----------------------------------------------------------------
void initMUX() {
  Serial.println(".");
  Serial.println("Setting up the SX MUX:");

  sx00 = new SX1509();
  sx01 = new SX1509();
  sx02 = new SX1509();
  sx03 = new SX1509();

  int muxCounter = 0;
  while (muxCounter != 4) {
    if (!sx00->begin(SX1509_ADDRESS_00)) {
      Serial.println("Failed  00 ");
      Serial.println(muxCounter);
      delay(100);
      muxCounter++;
      activeSX00 = false;
    } else {
      Serial.println("Connected MUX 00 ");
      activeSX00 = true;
      break;
    }
  }
  delay(200);

  ///--------------------------------------------
  muxCounter = 0;
  while (muxCounter != 4) {
    if (!sx01->begin(SX1509_ADDRESS_01)) {
      Serial.print("Failed MUX 01 ");
      Serial.println(muxCounter);
      delay(100);
      muxCounter++;
      activeSX01 = false;
    } else {
      Serial.println("Connected MUX 01 ");
      activeSX01 = true;
      break;
    }
  }
  delay(200);

  ///--------------------------------------------
  muxCounter = 0;
  while (muxCounter != 4) {
    if (!sx02->begin(SX1509_ADDRESS_10)) {
      Serial.print("Failed MUX 02 ");
      Serial.println(muxCounter);
      delay(100);
      muxCounter++;
      activeSX02 = false;
    } else {
      Serial.println("Connected MUX 02 ");
      activeSX02 = true;
      break;
    }
  }
  delay(200);

  //---------------------------------------
  muxCounter = 0;
  while (muxCounter != 4) {
    if (!sx03->begin(SX1509_ADDRESS_11)) {
      Serial.print("Failed MUX 03");
      Serial.println(muxCounter);
      delay(100);
      muxCounter++;
      activeSX03 = false;
    } else {
      Serial.println("Connected MUX 03");
      activeSX03 = true;
      break;
    }
  }
  delay(200);

  Serial.println("Done setting up MUX");
}

//----------------------------------------------------------------
void initSX00() {
  //buttons
  if (activeSX00) {
    Serial.println("init pins SX 00");

    sx00->pinMode(CALIBRATION_01_SX00, INPUT);
    sx00->pinMode(CALIBRATION_02_SX00, INPUT);
    sx00->pinMode(CALIBRATION_03_SX00, INPUT);
    sx00->pinMode(CALIBRATION_04_SX00, INPUT);

    //MUX PINS for selecting the motor
    sx00->pinMode(selectMotor[0], OUTPUT);
    sx00->pinMode(selectMotor[1], OUTPUT);
    sx00->pinMode(selectMotor[2], OUTPUT);

    //MUX
    sx00->digitalWrite(selectMotor[0], LOW);
    sx00->digitalWrite(selectMotor[1], LOW);
    sx00->digitalWrite(selectMotor[2], LOW);


    delay(200);
    Serial.println("Done init pins SX 00");
  }
}

//----------------------------------------------------------------
void initSX01() {
  if (activeSX01) {
    Serial.println("init pins SX 01");

    sx01->pinMode(SWITCH_01_SX01, INPUT);
    sx01->pinMode(SWITCH_02_SX01, INPUT);
    sx01->pinMode(SWITCH_03_SX01, INPUT);
    sx01->pinMode(SWITCH_04_SX01, INPUT);

    sx01->pinMode(DOWN_01_SX01, INPUT);
    sx01->pinMode(DOWN_02_SX01, INPUT);
    sx01->pinMode(DOWN_03_SX01, INPUT);
    sx01->pinMode(DOWN_04_SX01, INPUT);

    sx01->pinMode(UP_01_SX01, INPUT);
    sx01->pinMode(UP_02_SX01, INPUT);
    sx01->pinMode(UP_03_SX01, INPUT);
    sx01->pinMode(UP_04_SX01, INPUT);

    sx01->pinMode(TOUCH_01_SX01, INPUT);
    sx01->pinMode(TOUCH_02_SX01, INPUT);
    sx01->pinMode(TOUCH_03_SX01, INPUT);
    sx01->pinMode(TOUCH_04_SX01, INPUT);

    delay(200);
    Serial.println("Done init pins SX 01");
  }
}
//----------------------------------------------------------------
void initSX02() {
  if (activeSX02) {
    Serial.println("init pins SX 02");

    sx02->pinMode(SWITCH_05_SX02, INPUT);
    sx02->pinMode(SWITCH_06_SX02, INPUT);
    sx02->pinMode(SWITCH_07_SX02, INPUT);
    sx02->pinMode(SWITCH_08_SX02, INPUT);

    sx02->pinMode(DOWN_05_SX02, INPUT);
    sx02->pinMode(DOWN_06_SX02, INPUT);
    sx02->pinMode(DOWN_07_SX02, INPUT);
    sx02->pinMode(DOWN_08_SX02, INPUT);

    sx02->pinMode(UP_05_SX02, INPUT);
    sx02->pinMode(UP_06_SX02, INPUT);
    sx02->pinMode(UP_07_SX02, INPUT);
    sx02->pinMode(UP_08_SX02, INPUT);

    sx02->pinMode(TOUCH_05_SX02, INPUT);
    sx02->pinMode(TOUCH_06_SX02, INPUT);
    sx02->pinMode(TOUCH_07_SX02, INPUT);
    sx02->pinMode(TOUCH_08_SX02, INPUT);

    delay(200);
    Serial.println("Done init pins SX 02");
  }
}

//-------------------------------------------------------------------
void initSX03() {
  if (activeSX03) {

    Serial.println("init pins SX 03");

    sx03->pinMode(DIP_01_SX03, INPUT);
    sx03->pinMode(DIP_02_SX03, INPUT);
    sx03->pinMode(DIP_03_SX03, INPUT);
    sx03->pinMode(DIP_04_SX03, INPUT);
    sx03->pinMode(DIP_05_SX03, INPUT);
    sx03->pinMode(DIP_06_SX03, INPUT);
    sx03->pinMode(DIP_07_SX03, INPUT);
    sx03->pinMode(DIP_08_SX03, INPUT);
    sx03->pinMode(DIP_09_SX03, INPUT);
    sx03->pinMode(DIP_10_SX03, INPUT);

    sx03->pinMode(STATUS_PIN_SX03, OUTPUT);

    sx03->digitalWrite(STATUS_PIN_SX03, HIGH);
    delay(200);
    sx03->digitalWrite(STATUS_PIN_SX03, LOW);
    delay(200);
    sx03->digitalWrite(STATUS_PIN_SX03, HIGH);
    delay(200);
    sx03->digitalWrite(STATUS_PIN_SX03, LOW);

    //sx03
    //calibration status pin
    sx03->pinMode(CALIBRATION_STATUS_SX03, INPUT);

    Serial.println("Done init pins SX 03");
  }
}

//-------------------------------------------------------------------
// EPROM - read last position
int readPos(int motorId){
  int rawPos = EEPROM.read(motorId);
  return map(rawPos, 0, 255, motors[motorId]->minPos, motors[motorId]->maxPos);
}

//-------------------------------------------------------------------
// EPROM - save last position
void savePos(int motorId){
  // only able to save 1 byte per address
  // 100000 saves/lifecycle, is this a problem possibly?
  // 3.3 ms to complete write
  int mappedPos = map(motors[motorId]->currPos, motors[motorId]->minPos, motors[motorId]->maxPos, 0, 255);
  // int prevVal = EEPROM.read(motorId);
  // if (prevVal != mappedPos){
  //   EEPROM.write(motorId, mappedPos);
  // }
  EEPROM.update(motorId, mappedPos); // doesnt write if value doesnt change
}

//-------------------------------------------------------------------
//SX 01
void checkLimit() {
  if (activeSX01 && sx01->digitalRead(SWITCH_01_SX01) == HIGH) {
    Serial.println("LIMIT PIN 01");
    // Serial.println(motors[1]->dirMotor);
    if (motors[1]->dirMotor){
      motors[1]->motorOff();
    }
  }
  if (activeSX01 && sx01->digitalRead(SWITCH_02_SX01) == HIGH) {
    Serial.println("LIMIT PIN 02");
    if (motors[0]->dirMotor){
      motors[0]->motorOff();
    }
  }
  if (activeSX01 && sx01->digitalRead(SWITCH_03_SX01) == HIGH) {
    Serial.println("LIMIT PIN 03");
    if (motors[3]->dirMotor){
      motors[3]->motorOff();
    }
  }
  if (activeSX01 && sx01->digitalRead(SWITCH_04_SX01) == HIGH) {
    Serial.println("LIMIT PIN 04");
    if (motors[2]->dirMotor){
      motors[2]->motorOff();
    }
  }
  if (activeSX02 && sx02->digitalRead(SWITCH_05_SX02) == HIGH) {
    Serial.println("LIMIT PIN 05");
    if (motors[5]->dirMotor){
      motors[5]->motorOff();
    }
  }
  if (activeSX02 && sx02->digitalRead(SWITCH_06_SX02) == HIGH) {
    Serial.println("LIMIT PIN 06");
    if (motors[4]->dirMotor){
      motors[4]->motorOff();
    }
  }
  if (activeSX02 && sx02->digitalRead(SWITCH_07_SX02) == HIGH) {
    Serial.println("LIMIT PIN 07");
    if (motors[7]->dirMotor){
      motors[7]->motorOff();
    }
  }
  if (activeSX02 && sx02->digitalRead(SWITCH_08_SX02) == HIGH) {
    Serial.println("LIMIT PIN 08");
    if (motors[6]->dirMotor){
      motors[6]->motorOff();
    }
  }
}
//-------------------------------------------------------------------
void checkDip() {
  int dipLen = 10;
  uint8_t dip[dipLen];

  for (int i = 0; i < dipLen; i++) {
    dip[i] = 0;
  }
  if (activeSX03 && sx03->digitalRead(DIP_01_SX03) == LOW) {
    Serial.println("DIP PIN 01");
    dip[0] = 1;
  }
  if (activeSX03 && sx03->digitalRead(DIP_02_SX03) == LOW) {
    Serial.println("DIP PIN 02");
    dip[1] = 1;
  }
  if (activeSX03 && sx03->digitalRead(DIP_03_SX03) == LOW) {
    Serial.println("DIP PIN 03");
    dip[2] = 1;
  }
  if (activeSX03 && sx03->digitalRead(DIP_04_SX03) == LOW) {
    Serial.println("DIP PIN 04");
    dip[3] = 1;
  }
  if (activeSX03 && sx03->digitalRead(DIP_05_SX03) == LOW) {
    Serial.println("DIP PIN 05");
    dip[4] = 1;
  }
  if (activeSX03 && sx03->digitalRead(DIP_06_SX03) == LOW) {
    Serial.println("DIP PIN 06");
    dip[5] = 1;
  }
  if (activeSX03 && sx03->digitalRead(DIP_07_SX03) == LOW) {
    Serial.println("DIP PIN 07");
    dip[6] = 1;
  }
  if (activeSX03 && sx03->digitalRead(DIP_08_SX03) == LOW) {
    Serial.println("DIP PIN 08");
    dip[7] = 1;
  }
  if (activeSX03 && sx03->digitalRead(DIP_09_SX03) == LOW) {
    Serial.println("DIP PIN 09");
    dip[8] = 1;
  }
  if (activeSX03 && sx03->digitalRead(DIP_10_SX03) == LOW) {
    Serial.println("DIP PIN 10");
    dip[9] = 1;
  }

  ///convert array to int
  int dipOutput = 0;
  int power = 1;

  for (uint8_t i = 0; i < dipLen; i++) {
    dipOutput += dip[i] * power;
    //(dipLen - 1) - i
    // output goes 1*2^0 + 0*2^1 + 0*2^2 + ...
    power *= 2;
  }

  dipNumber = dipOutput;
  nodeId = dipNumber;
  // Serial.print("Node Id: ");
  // Serial.println(nodeId);
}

//-------------------------------------------------------------------
/*

  Color ids mapped:

  | 0 | 2 | 4 | 6 |
  -----------------
  | 1 | 3 | 5 | 7 |

  Pushdown pins mapped:

  | 2 | 4 | 7 | 8 |
  -----------------
  | 1 | 3 | 5 | 6 |

*/
void checkPushDown() {
  //SX 02 down
  if (activeSX01 && sx01->digitalRead(DOWN_01_SX01) == HIGH) {
    Serial.println("PIN 01 DOWN");
    colorChange = true;
    colorId = 1;

  }
  if (activeSX01 && sx01->digitalRead(DOWN_02_SX01) == HIGH) {
    Serial.println("PIN 02 DOWN");
    colorChange = true;
    colorId = 0;
  }
  if (activeSX01 && sx01->digitalRead(DOWN_03_SX01) == HIGH) {
    Serial.println("PIN 03 DOWN");
    colorChange = true;
    colorId = 3;
  }
  if (activeSX01 && sx01->digitalRead(DOWN_04_SX01) == HIGH) {
    Serial.println("PIN 04 DOWN");
    colorChange = true;
    colorId = 2;
  }
  if (activeSX02 && sx02->digitalRead(DOWN_05_SX02) == HIGH) {
    Serial.println("PIN 05 DOWN");
    colorChange = true;
    colorId = 5;
  }
  if (activeSX02 && sx02->digitalRead(DOWN_06_SX02) == HIGH) {
    Serial.println("PIN 06 DOWN");
    colorChange = true;
    colorId = 7;
    // sendLastPos();
  }
  if (activeSX02 && sx02->digitalRead(DOWN_07_SX02) == HIGH) {
    Serial.println("PIN 07 DOWN");
    colorChange = true;
    colorId = 4;
  }
  if (activeSX02 && sx02->digitalRead(DOWN_08_SX02) == HIGH) {
    Serial.println("PIN 08 DOWN");
    colorChange = true;
    colorId = 6;
  }

  if (colorChange) {
    int randRed = random(0, 255);
    int randBlue = random(0, 255);
    int randGreen = random(0, 255);
    for (int j = 0; j < NUM_LEDS; j++) {
      pixels[colorId]->setPixelColor(j, pixels[colorId]->Color(randRed, randBlue, randGreen));  // Moderately bright green color.
    }
    pixels[colorId]->show();
    colorChange = false;
  }
}


//-------------------------------------------------------------------
void checkPushUp() {
  if (activeSX01 && sx01->digitalRead(UP_01_SX01) == HIGH) {
    Serial.println("LIMIT UP PIN 01");
  }
  if (activeSX01 && sx01->digitalRead(UP_02_SX01) == HIGH) {
    Serial.println("LIMIT UP PIN 02");
  }
  if (activeSX01 && sx01->digitalRead(UP_03_SX01) == HIGH) {
    Serial.println("LIMIT UP PIN 03");
  }
  if (activeSX01 && sx01->digitalRead(UP_04_SX01) == HIGH) {
    Serial.println("LIMIT UP PIN 04");
  }
  if (activeSX02 && sx02->digitalRead(UP_05_SX02) == HIGH) {
    Serial.println("LIMIT UP PIN 05");
  }
  if (activeSX02 && sx02->digitalRead(UP_06_SX02) == HIGH) {
    Serial.println("LIMIT UP PIN 06");
  }
  if (activeSX02 && sx02->digitalRead(UP_07_SX02) == HIGH) {
    Serial.println("LIMIT UP PIN 07");
  }
  if (activeSX02 && sx02->digitalRead(UP_08_SX02) == HIGH) {
    Serial.println("LIMIT UP PIN 08");
  }
}
//------------CAN BUS
//------------------------------------------------------------------
void initCanBus() {
  Serial.println("Init Can Bus: ");
  Serial.println("Can Bus FD");

  canFD.begin();
  CANFD_timings_t config;
  config.clock = CLK_40MHz;
  config.baudrate = 1000000;
  config.baudrateFD = 8000000;
  config.propdelay = 190;
  config.bus_length = 1;
  config.sample = 87.5;
  canFD.setRegions(64);
  canFD.setBaudRate(config);
  canFD.mailboxStatus();

  canFD.enableMBInterrupts();
  canFD.onReceive(reading);
  canFD.distribute();
  //

  //setup CAN BUS 2.0
  /*
    Serial.println("Can Bus 2.0");
    can2.begin();
    can2.setBaudRate(500000);     // 500kbps data rate
    can2.enableFIFO();
    can2.enableFIFOInterrupt();
    can2.onReceive(FIFO, canBusSniff);
    can2.mailboxStatus();
  */

  Serial.println("Done Can Bus");
  delay(200);
}

//--- Function that read the CAND Bus ----------------------------
//----------------------------------------------------------------
void reading(const CANFD_message_t& msg) {
  Serial.println("message received");
  //turn on LED
  if (activeSX03) {
    sx03->digitalWrite(STATUS_PIN_SX03, LOW);
  }
  /*
  Serial.print(" Node: ");
  Serial.print(msg.id);
  Serial.print(" NHard ");
  Serial.print(nodeId);
*/
  if (msg.id == nodeId) {
    Serial.print("Got Msg ");
    Serial.print(msg.id);
    Serial.print(" ");
    Serial.println(nodeId);
    if (activeSX03) {
      sx03->digitalWrite(STATUS_PIN_SX03, HIGH);
    }
    /*
    Serial.println(" ");

    Serial.print(" Node: ");
    Serial.print(msg.id);
    Serial.print(" LEN: ");
    Serial.print(msg.len);
    Serial.print(" NHard ");
    Serial.print(nodeId);
    Serial.print(" DATA: ");


    for (uint8_t i = 0; i < msg.len; i++) {
      Serial.print(msg.buf[i]);
      Serial.print(" ");
    }
    Serial.println(" ");

    // if the board is the same Id
    Serial.print(msg.id);
    Serial.print(" ");
    Serial.println(nodeId);
    Serial.println(" ");
    //turn on the status LED if the current node is the can bus msg node

*/

    // interaction
    for (uint8_t i = 0; i < NUM_3D_PIXELS; i++) {
      if (msg.buf[i * 8 + 0] == 1) {
        // action based on sent interaction
        // color change if in land use mode
        // height change if in density mode
      } 
    }

    //shaff

    // for (uint8_t i = 0; i < NUM_3D_PIXELS; i++) {
    //   if (msg.buf[i * 8 + 1] == 1) {
    //     enableMotor(i);
    //     motors[i]->shaftOn(driver);
    //   } else if (msg.buf[i * 8 + 1] == 0) {
    //     enableMotor(i);
    //     motors[i]->shaftOff(driver);
    //   }
    // }

    // zero
    if (msg.buf[1] == 1){
      int r = 0;
      int g = 0;
      int b = 255;
      int w = 0;
      for (int j = 0; j < NUM_3D_PIXELS; j++) {
        for (int i = 0; i < NUM_LEDS; i++) {
          pixels[j]->setPixelColor(i, pixels[j]->Color(r, g, b, w));
          pixels[j]->setPixelColor(i, pixels[j]->Color(r, g, b, w));
        }
        pixels[j]->show();
      }
      Serial.println("Random LEDs color");
      initPositions();
    } else {

      //enable/disable motor
      for (uint8_t i = 0; i < NUM_3D_PIXELS; i++) {
        if (msg.buf[i * 8 + 2] == 1) {
          enableMotor(i);
          motors[i]->start(sx01, driver);
          motors[i]->motorOn();
        } else if (msg.buf[i * 8 + 2] == 0){
          enableMotor(i);
          motors[i]->stop(sx01, driver);
        }
      }



      //stop motor
      // previous position
      for (uint8_t i = 0; i < NUM_3D_PIXELS; i++) {
        // if (msg.buf[i * 8 + 3] == 1) {
        //   enableMotor(i);
        //   motors[i]->stop(sx01, driver);
        // }
        motors[i]->currPos = msg.buf[i * 8 + 3];
      }






      //update all the 3d Pixels
      for (uint8_t i = 0; i < NUM_3D_PIXELS; i++) {
        char str[16];
        // sprintf(str, "%X%X", msg.buf[i * 4 + 2], msg.buf[i * 4 + 3]);
        //uint16_t color = convertFrom8To16(msg.buf[i * 4 + 2], msg.buf[i * 4 + 3]);
        grid(msg.id, i, msg.buf[i * 8 + 5], msg.buf[i * 8 + 6], msg.buf[i * 8 + 7], msg.buf[i * 8 + 4]);
      }



      //height
      for (uint8_t i = 0; i < NUM_3D_PIXELS; i++) {
        uint8_t height = msg.buf[i * 8 + 4];
        uint8_t seqAct = msg.buf[i * 8 + 0];
        if (height > 0 && seqAct == 1) {
          motors[i]->toggleSequence();
          motors[i]->setTargetPos(height);
          enableMotor(i);
          motors[i]->start(sx01, driver);
          motors[i]->motorOn();

          Serial.print(i);
          Serial.print(" got Seq: ");
          Serial.println(height);
        }
      }
    }
  }
}

/*

   Colors:
   https://stackoverflow.com/questions/51785689/what-is-an-effective-way-to-map-rgbw-pixel-data-to-rgb-for-a-visualisation
*/

//----------------------------------------------------------------
void grid(int node, int local, uint8_t r, uint8_t g, uint8_t b, int height) {
  //uint64_t color = (uint64_t)strtoull(str, 0, 16);
  //uint16_t r = ((((color >> 11) & 0x1F) * 527) + 23) >> 6;
  //uint16_t g = ((((color >> 5) & 0x3F) * 259) + 33) >> 6;
  //uint16_t b = (((color & 0x1F) * 527) + 23) >> 6;
  // Serial.print(local);
  // Serial.print(" ");
  // Serial.print("R: ");
  // Serial.print(r);
  // Serial.print(" Green: ");
  // Serial.print(g);
  // Serial.print(" Blue: ");
  // Serial.print(b);



  if (r != tempRGB[local].r | g != tempRGB[local].g | b != tempRGB[local].b) {
    int mini = min(r, min(g, b));
    for (int j = 0; j < NUM_LEDS; j++) {
      pixels[local]->setPixelColor(j, pixels[local]->gamma8(pixels[local]->Color(r - mini, g - mini, b - mini, mini)));  // Moderately bright green color.
    }
    // Serial.print(r);
    // Serial.print(" ");
    Serial.print(r - mini);
    Serial.print(" ");
    Serial.print(g - mini);
    Serial.print(" ");
    Serial.print(b - mini);
    Serial.print(" ");
    Serial.print(mini);
    Serial.println(" ");

    pixels[local]->show();
    tempRGB[local] = { r, g, b };
  }
}


//CALIBRATION FUNCTIONS
//----------------------------------------------------------------
void initCalibration() {
  if (activeSX03) {
    if (sx03->digitalRead(CALIBRATION_STATUS_SX03) == HIGH) {
      Serial.println("Calibration module is connected");
    } else {
      Serial.println("Calibration module is not connected");
    }
  }
}

void updateCalibration() {
  if (activeSX00) {
    if (sx00->digitalRead(CALIBRATION_01_SX00) == HIGH) {
      Serial.println("Calibration pin 01 ON");
    }
    if (sx00->digitalRead(CALIBRATION_02_SX00) == HIGH) {
      Serial.println("Calibration pin 02 ON");
    }
    if (sx00->digitalRead(CALIBRATION_03_SX00) == HIGH) {
      Serial.println("Calibration pin 03 ON");
    }
    if (sx00->digitalRead(CALIBRATION_04_SX00) == HIGH) {
      Serial.println("Calibration pin 04 ON");
    }
  }
}

void calculteCalibration() {

  //do this process several times,
  // 5 times until we get a mean.


  if (activeSX00 && sx00->digitalRead(CALIBRATION_01_SX00) == HIGH) {
    Serial.println("Calibration pin 01 ON");
    //go down
  }

  if (activeSX01 && sx01->digitalRead(SWITCH_01_SX01) == HIGH) {
    Serial.println("LIMIT PIN 01");
  }
}

//----------------------------------------------------------------
void keyCommands() {
  if (Serial.available() > 0) {
    char key = Serial.read();

    if (key == '0'){
      Serial.println("start zeroing");
      Serial.println(".............");

      initPositions();

      Serial.println(".............");
      Serial.println("zeroing done");
    }
    if (key == '9'){
      Serial.println("target");
      Serial.print("Motor 3: ");
      Serial.println(motors[3]->currPos);
      Serial.print("Motor 5: ");
      Serial.println(motors[5]->currPos);
      // motors[7]->startSequence(driver, 1000);
      // motors[7]->motorOn();
    }
    if (key == '-'){
      sendLastPos();
    }

    //------- toggle direction
    if (key == '1') {
      toggleMotorShaft(0);
    }
    if (key == '2') {
      toggleMotorShaft(1);
    }
    if (key == '3') {
      toggleMotorShaft(2); 
    }
    if (key == '4') {
      toggleMotorShaft(3);
    }
    if (key == '5') {
      toggleMotorShaft(4);
    }
    if (key == '6') {
      toggleMotorShaft(5);
    }
    if (key == '7') {
      toggleMotorShaft(6);
    }
    if (key == '8') {
      toggleMotorShaft(7);
    }

    //------- toggle Motor
    if (key == 'q') {
      toggleMotor(0);
    }
    if (key == 'w') {
      toggleMotor(1);
    }
    if (key == 'e') {
      toggleMotor(2);
    }
    if (key == 'r') {
      toggleMotor(3);
    }
    if (key == 't') {
      toggleMotor(4);
    }
    if (key == 'y') {
      toggleMotor(5);
    }
    if (key == 'u') {
      toggleMotor(6);
    }
    if (key == 'i') {
      toggleMotor(7);
    }

    //turn on/off motor


    if (key == 'a') {
      for (int i = 0; i < NUM_3D_PIXELS; i++) {
        enableMotor(i);
        motors[i]->start(sx01, driver);
      }
      Serial.println("Start Motor");
    }
    if (key == 's') {
      for (int i = 0; i < NUM_3D_PIXELS; i++) {
        motors[i]->motorOn();
      }
      Serial.println("Start Motor");
    }
    if (key == 'd') {
      for (int i = 0; i < NUM_3D_PIXELS; i++) {
        enableMotor(i);
        motors[i]->stop(sx01, driver);
      }
      Serial.println("End Motor");
    }

    //change motor direction
    if (key == 'z') {
      for (int i = 0; i < NUM_3D_PIXELS; i++) {
        enableMotor(i);
        motors[i]->shaftOn(driver);
      }
    }
    if (key == 'x') {
      for (int i = 0; i < NUM_3D_PIXELS; i++) {
        enableMotor(i);
        motors[i]->shaftOff(driver);
      }
    }

    //LEDS
    if (key == 'k') {
      //reset and init the neopixels
      if (nodeId >= 6) {
        for (int i = 0; i < NUM_3D_PIXELS; i++) {
          pixels[i] = new Adafruit_NeoPixel(NUM_LEDS, NEO_PIN_DOWN[i], NEO_GRBW + NEO_KHZ800);
          pixels[i]->begin();
          pixels[i]->clear();
        }
      } else {
        for (int i = 0; i < NUM_3D_PIXELS; i++) {
          pixels[i] = new Adafruit_NeoPixel(NUM_LEDS, NEO_PIN_UP[i], NEO_GRBW + NEO_KHZ800);
          pixels[i]->begin();
          pixels[i]->clear();
        }
      }
      //set pixels to different colors
      for (int j = 0; j < NUM_3D_PIXELS; j++) {
        for (int i = 0; i < NUM_LEDS; i++) {
          pixels[j]->setPixelColor(i, pixels[j]->Color(0, 0, 0, 255));  // White
          pixels[j]->show();
        }
      }

      Serial.println("Reset LEDs to white");
    }

    if (key == 'l') {
      //set pixels to different colors
      int r = 0;
      int g = 0;
      int b = 255;
      int w = 0;

      for (int j = 0; j < NUM_3D_PIXELS; j++) {
        for (int i = 0; i < NUM_LEDS; i++) {
          pixels[j]->setPixelColor(i, pixels[j]->Color(r, g, b, w));
          pixels[j]->setPixelColor(i, pixels[j]->Color(r, g, b, w));
        }
        pixels[j]->show();
      }
      Serial.println("Random LEDs color");
      delay(2000);

      r = 0;
      g = 255;
      b = 0;

      for (int j = 0; j < NUM_3D_PIXELS; j++) {
        for (int i = 0; i < NUM_LEDS; i++) {
          pixels[j]->setPixelColor(i, pixels[j]->Color(r, g, b, w));
          pixels[j]->setPixelColor(i, pixels[j]->Color(r, g, b, w));
        }
        pixels[j]->show();
      }

      Serial.println("Random LEDs color");
      delay(2000);

      r = 255;
      g = 0;
      b = 0;

      for (int j = 0; j < NUM_3D_PIXELS; j++) {
        for (int i = 0; i < NUM_LEDS; i++) {
          pixels[j]->setPixelColor(i, pixels[j]->Color(r, g, b, w));
          pixels[j]->setPixelColor(i, pixels[j]->Color(r, g, b, w));
        }
        pixels[j]->show();
      }

      delay(2000);

      r = 0;
      g = 0;
      b = 0;
      w = 255;

      for (int j = 0; j < NUM_3D_PIXELS; j++) {
        for (int i = 0; i < NUM_LEDS; i++) {
          pixels[j]->setPixelColor(i, pixels[j]->Color(r, g, b, w));
          pixels[j]->setPixelColor(i, pixels[j]->Color(r, g, b, w));
        }
        pixels[j]->show();
      }
    }

    if (key == 'm') {
      initPixels();
    }

    //calibration
    if (key == 'c') {
      motors[1]->toggleCalibration();
      if (!motors[1]->isCalibrating()) {
        Serial.println("steps reset");
        motors[1]->resetSteps();
      } else {
        toggleMotor(1);
        Serial.println("activate Calibration");
      }
    }

    if (key == 'v') {
      for (int j = 0; j < NUM_3D_PIXELS; j++) {
        int steps = motors[j]->getNumCaliSteps();
        Serial.print("steps ");
        Serial.print(j);
        Serial.print(" ");
        Serial.println(steps);
      }
    }

    if (key == 'b') {
      // for (int j = 0; j < NUM_3D_PIXELS; j++) {
      motors[1]->toggleSequence();
      motors[1]->setTargetPos(50);

      enableMotor(1);
      motors[1]->start(sx01, driver);
      motors[1]->motorOn();

      Serial.println("sequence");
      //}
    }

    if (key == 'n') {
      Serial.println("serquence");
      for (int i = 0; i < NUM_3D_PIXELS; i++) {
        motors[i]->toggleSequence();
        motors[i]->setTargetPos((int)random(50, 100));

        enableMotor(i);
        motors[i]->start(sx01, driver);
        motors[i]->motorOn();
        //  motors[i]->shaftOff(driver);
      }
    }
  }
}

//----------------------------------------------------------------
//convert bit array to int
int conversion(int array[], int len) {
  int output = 0;
  int power = 1;

  for (int i = 0; i < len; i++) {
    output += array[(len - 1) - i] * power;
    // output goes 1*2^0 + 0*2^1 + 0*2^2 + ...
    power *= 2;
  }

  return output;
}

//----------------------------------------------------------------
//Convert 2 bytes to a single 16bit number
uint16_t convertFrom8To16(uint8_t dataFirst, uint8_t dataSecond) {
  uint16_t dataBoth = 0x0000;

  dataBoth = dataFirst;
  dataBoth = dataBoth << 8;
  dataBoth |= dataSecond;
  return dataBoth;
}
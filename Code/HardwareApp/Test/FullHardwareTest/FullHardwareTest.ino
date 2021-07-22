#include <Arduino.h>
#include <Wire.h>

#include "StepperMotor.h"
#include "Pins.h"

//SX
#include <SparkFunSX1509.h>
#include <Adafruit_NeoPixel.h>
#include <Wire.h>

//CAN BUS
#include <FlexCAN_T4.h>

//CAN BUS
FlexCAN_T4FD<CAN3, RX_SIZE_512, TX_SIZE_64> canFD;
FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16>   can2;


//NEO PIXELS PINS
const byte NEO_PIN[8] = {NEO_PIN_01, NEO_PIN_02, NEO_PIN_03, NEO_PIN_04,
                         NEO_PIN_05, NEO_PIN_06, NEO_PIN_07, NEO_PIN_08
                        };


//Pixels
struct rgb {
  uint8_t   r;
  uint8_t   g;
  uint8_t   b;
};

Adafruit_NeoPixel * pixels[8];

boolean colorChange = false;
int colorId = -1;
rgb tempRGB[8]; //store temp current color

//MUX
//Motor commands, step calibration
SX1509 * sx00;

//limit and push down buttons
SX1509 * sx01;
bool interruptActivate01 = false;

//push push interaction and touch enable
SX1509 * sx02;
bool interruptActivate02 = false;

//dip switch  and notifications
SX1509 * sx03;



//Motors
StepperMotor * motors[8];
//DRV8880 class
TMC2209Stepper  * driver;

//
int motorDirPins[]     =  {DIR_PIN_01, DIR_PIN_02 , DIR_PIN_03, DIR_PIN_04, DIR_PIN_05, DIR_PIN_06, DIR_PIN_07, DIR_PIN_08};
int motorStepPins[]    =  {STEP_PIN_01, STEP_PIN_02, STEP_PIN_03, STEP_PIN_04, STEP_PIN_05, STEP_PIN_06, STEP_PIN_07, STEP_PIN_08};
int motorEnablePins[]  =  {ENABLE_01_SX00, ENABLE_02_SX00, ENABLE_03_SX00, ENABLE_04_SX00, ENABLE_05_SX00, ENABLE_06_SX00, ENABLE_07_SX00, ENABLE_08_SX00};
//------------------------
//Motor Mux
const int selectMotor[3] = {5, 7, 9};
const int motorEnable = 15;
//

//number form the Dip
int dipNumber = 0;

//Node Id
int nodeId = 0; //in case dip number is different from node id
//

void setup() {
  //Start Serial
  Serial.begin(9600);
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

  //NEO PIXELS
  Serial.println("......");
  initPixels();

  //get Node Number Dip
  Serial.println("......");
  checkDip();

  //
  //setup CAN Bus FD
  Serial.println("......");
  initCanBus();

  Serial.println("done setup");
  Serial.println(".............");
}


void loop() {

  motorUpdate();

  //swithces
  //checkLimit();
  //checkPushDown();
  //checkPushUp();
  //checkDip();

  //calibration Module
  //updateCalibration();

  //CAN BUS
  //canFD.events();

  //keyCommands
  keyCommands();

}

//--PIXELS
//-------------------------------------------------------------------
void initPixels() {
  Serial.println("Init LEDs");
  for (int i = 0; i < NUM_3D_PIXELS; i++) {
    pixels[i] = new Adafruit_NeoPixel(NUM_LEDS, NEO_PIN[i], NEO_GRBW + NEO_KHZ800);
    pixels[i]->begin();
    pixels[i]->clear();
  }
  //set pixels to different colors
  for (uint8_t j = 0; j < NUM_3D_PIXELS; j++) {
    for (uint8_t i = 0; i < NUM_LEDS; i++) {
      pixels[j]->setPixelColor(i, pixels[j]->Color(0, 0, 0, 255)); // Moderately bright green color.
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
    motors[i] =  new StepperMotor(i, GMOTOR_STEPS, motorDirPins[i], motorStepPins[i], motorEnablePins[i]);
    motors[i]->initPins(sx00);
    motors[i]->initMotor(driver);
    motors[i]->shaftOff(driver);
    motors[i]->motorOff();
    Serial.println("-----");
    delay(10);
  }


  Serial.println("Done Motors");
}
//-------------------------------------------------------------------
//MOTOR UPDATE BLOCKING WIHT DELAY
void motorUpdate() {

  for (uint16_t i = 500; i > 0; i--) {
    for (int i = 0; i < NUM_3D_PIXELS; i++) {
      motors[i]->motorStart(sx00);
    }
    delayMicroseconds(4);

    for (uint8_t i = 0; i < NUM_3D_PIXELS; i++) {
      motors[i]->motorEnd(sx00);
    }
    delayMicroseconds(4);
  }
}

//-------------------------------------------------------------------
void toggleMotor(int motorId) {
  motors[motorId]->toggleMotor();
  Serial.print("Motor toggle: ");
  Serial.println(motorId);
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
  } else if ( id == 1) {
    sx00->digitalWrite(selectMotor[0], HIGH);
    sx00->digitalWrite(selectMotor[1], LOW);
    sx00->digitalWrite(selectMotor[2], LOW);
  } else if ( id == 2) {
    sx00->digitalWrite(selectMotor[0], LOW);
    sx00->digitalWrite(selectMotor[1], HIGH);
    sx00->digitalWrite(selectMotor[2], LOW);
  } else if ( id == 3) {
    sx00->digitalWrite(selectMotor[0], HIGH);
    sx00->digitalWrite(selectMotor[1], HIGH);
    sx00->digitalWrite(selectMotor[2], LOW);
  } else if ( id == 4) {
    sx00->digitalWrite(selectMotor[0], LOW);
    sx00->digitalWrite(selectMotor[1], LOW);
    sx00->digitalWrite(selectMotor[2], HIGH);
  } else if ( id == 5) {
    sx00->digitalWrite(selectMotor[0], HIGH);
    sx00->digitalWrite(selectMotor[1], LOW);
    sx00->digitalWrite(selectMotor[2], HIGH);
  } else if ( id == 6) {
    sx00->digitalWrite(selectMotor[0], LOW);
    sx00->digitalWrite(selectMotor[1], HIGH);
    sx00->digitalWrite(selectMotor[2], HIGH);
  } else if ( id == 7) {
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
    if (!sx00->begin(SX1509_ADDRESS_00) ) {
      Serial.println("Failed MUX 00 ");
      Serial.println(muxCounter);
      delay(100);
      muxCounter++;
    } else {
      Serial.println("Connected MUX 00 ");
      break;
    }
  }
  delay(500);

  ///--------------------------------------------
  muxCounter = 0;
  while (muxCounter != 4) {
    if (!sx01->begin(SX1509_ADDRESS_01) ) {
      Serial.print("Failed MUX 01 ");
      Serial.println(muxCounter);
      delay(100);
      muxCounter++;
    } else {
      Serial.println("Connected MUX 01 ");
      break;
    }
  }
  delay(500);

  ///--------------------------------------------
  muxCounter = 0;
  while (muxCounter != 4) {
    if (!sx02->begin(SX1509_ADDRESS_10) ) {
      Serial.print("Failed MUX 02 ");
      Serial.println(muxCounter);
      delay(100);
      muxCounter++;
    } else {
      Serial.println("Connected MUX 02 ");
      break;
    }
  }
  delay(500);

  //---------------------------------------
  muxCounter = 0;
  while (muxCounter != 4) {
    if (!sx03->begin(SX1509_ADDRESS_11) ) {
      Serial.print("Failed MUX 03");
      Serial.println(muxCounter);
      delay(100);
      muxCounter++;
    } else {
      Serial.println("Connected MUX 03");
      break;
    }
  }
  delay(500);

  Serial.println("Done setting up MUX");
}

//----------------------------------------------------------------
void initSX00() {
  //buttons
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
  for (int i = 0; i < 3; i++) {
    sx00->digitalWrite(selectMotor[i], LOW);
  }

  delay(200);
  Serial.println("Done init pins SX 00");
}

//----------------------------------------------------------------
void initSX01() {

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
//----------------------------------------------------------------
void initSX02() {
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
  sx02->pinMode(UP_08_SX02,  INPUT);

  sx02->pinMode(TOUCH_05_SX02, INPUT);
  sx02->pinMode(TOUCH_06_SX02, INPUT);
  sx02->pinMode(TOUCH_07_SX02, INPUT);
  sx02->pinMode(TOUCH_08_SX02, INPUT);

  delay(200);
  Serial.println("Done init pins SX 02");
}

//-------------------------------------------------------------------
void initSX03() {
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

//-------------------------------------------------------------------
//SX 01
void checkLimit() {
  if (sx01->digitalRead(SWITCH_01_SX01) == HIGH) {
    Serial.println("LIMIT PIN 01");
  }
  if (sx01->digitalRead(SWITCH_02_SX01) == HIGH) {
    Serial.println("LIMIT PIN 02");
  }
  if (sx01->digitalRead(SWITCH_03_SX01) == HIGH) {
    Serial.println("LIMIT PIN 03");
  }
  if (sx01->digitalRead(SWITCH_04_SX01) == HIGH) {
    Serial.println("LIMIT PIN 04");
  }
  if (sx02->digitalRead(SWITCH_05_SX02) == HIGH) {
    Serial.println("LIMIT PIN 05");
  }
  if (sx02->digitalRead(SWITCH_06_SX02) == HIGH) {
    Serial.println("LIMIT PIN 06");
  }
  if (sx02->digitalRead(SWITCH_07_SX02) == HIGH) {
    Serial.println("LIMIT PIN 07");
  }
  if (sx02->digitalRead(SWITCH_08_SX02) == HIGH) {
    Serial.println("LIMIT PIN 08");
  }
}
//-------------------------------------------------------------------
void checkDip() {
  int dipLen = 10;
  uint8_t dip[dipLen];

  for (int i = 0; i < dipLen; i++) {
    dip[i] = 0;
  }
  if (sx03->digitalRead(DIP_01_SX03) == HIGH) {
    Serial.println("DIP PIN 01");
    dip[0] = 1;
  }
  if (sx03->digitalRead(DIP_02_SX03) == HIGH) {
    Serial.println("DIP PIN 02");
    dip[1] = 1;
  }
  if (sx03->digitalRead(DIP_03_SX03) == HIGH) {
    Serial.println("DIP PIN 03");
    dip[2] = 1;
  }
  if (sx03->digitalRead(DIP_04_SX03) == HIGH) {
    Serial.println("DIP PIN 04");
    dip[3] = 1;
  }
  if (sx03->digitalRead(DIP_05_SX03) == HIGH) {
    Serial.println("DIP PIN 05");
    dip[4] = 1;
  }
  if (sx03->digitalRead(DIP_06_SX03) == HIGH) {
    Serial.println("DIP PIN 06");
    dip[5] = 1;
  }
  if (sx03->digitalRead(DIP_07_SX03) == HIGH) {
    Serial.println("DIP PIN 07");
    dip[6] = 1;
  }
  if (sx03->digitalRead(DIP_08_SX03) == HIGH) {
    Serial.println("DIP PIN 08");
    dip[7] = 1;
  }
  if (sx03->digitalRead(DIP_09_SX03) == HIGH) {
    Serial.println("DIP PIN 09");
    dip[8] = 1;
  }
  if (sx03->digitalRead(DIP_10_SX03) == HIGH) {
    Serial.println("DIP PIN 10");
    dip[9] = 1;
  }

  ///convert array to int
  int dipOutput = 0;
  int power = 1;

  for (uint8_t i = 0; i < dipLen; i++) {
    dipOutput += dip[(dipLen - 1) - i] * power;
    // output goes 1*2^0 + 0*2^1 + 0*2^2 + ...
    power *= 2;
  }

  dipNumber = dipOutput;
  nodeId =  dipNumber;
  Serial.print("Node Id: ");
  Serial.println(nodeId);
}

//-------------------------------------------------------------------
void checkPushDown() {
  //SX 02 down
  if (sx01->digitalRead(DOWN_01_SX01) == HIGH) {
    Serial.println("PIN 01 DOWN");
    colorChange = true;
    colorId = 0;
  }
  if (sx01->digitalRead(DOWN_02_SX01) == HIGH) {
    Serial.println("PIN 02 DOWN");
    colorChange = true;
    colorId = 1;
  }
  if (sx01->digitalRead(DOWN_03_SX01) == HIGH) {
    Serial.println("PIN 03 DOWN");
    colorChange = true;
    colorId = 2;
  }
  if (sx01->digitalRead(DOWN_04_SX01) == HIGH) {
    Serial.println("PIN 04 DOWN");
    colorChange = true;
    colorId = 3;
  }
  if (sx02->digitalRead(DOWN_05_SX02) == HIGH) {
    Serial.println("PIN 05 DOWN");
    colorChange = true;
    colorId = 4;
  }
  if (sx02->digitalRead(DOWN_06_SX02) == HIGH) {
    Serial.println("PIN 06 DOWN");
    colorChange = true;
    colorId = 5;
  }
  if (sx02->digitalRead(DOWN_07_SX02) == HIGH) {
    Serial.println("PIN 07 DOWN");
    colorChange = true;
    colorId = 6;
  }
  if (sx02->digitalRead(DOWN_08_SX02) == HIGH) {
    Serial.println("PIN 08 DOWN");
    colorChange = true;
    colorId = 7;
  }

  if (colorChange) {
    int randRed    = random(0, 255);
    int randBlue   = random(0, 255);
    int randGreen  = random(0, 255);
    for (int j = 0; j < NUM_LEDS; j++) {
      pixels[colorId]->setPixelColor(j, pixels[colorId]->Color(randRed, randBlue, randGreen)); // Moderately bright green color.
    }
    pixels[colorId]->show();
    colorChange = false;
  }
}


//-------------------------------------------------------------------
void checkPushUp() {
  if (sx01->digitalRead(UP_01_SX01) == HIGH) {
    Serial.println("LIMIT UP PIN 01");
  }
  if (sx01->digitalRead(UP_02_SX01) == HIGH) {
    Serial.println("LIMIT UP PIN 02");
  }
  if (sx01->digitalRead(UP_03_SX01) == HIGH) {
    Serial.println("LIMIT UP PIN 03");
  }
  if (sx01->digitalRead(UP_04_SX01) == HIGH) {
    Serial.println("LIMIT UP PIN 04");
  }
  if (sx02->digitalRead(UP_05_SX02) == HIGH) {
    Serial.println("LIMIT UP PIN 05");
  }
  if (sx02->digitalRead(UP_06_SX02) == HIGH) {
    Serial.println("LIMIT UP PIN 06");
  }
  if (sx02->digitalRead(UP_07_SX02) == HIGH) {
    Serial.println("LIMIT UP PIN 07");
  }
  if (sx02->digitalRead(UP_08_SX02) == HIGH) {
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

//----------------------------------------------------------------
void reading(const CANFD_message_t & msg) {
  //turn on LED
  sx03->digitalWrite(STATUS_PIN_SX03, LOW);

  Serial.print("  Node: "); Serial.print(msg.id);
  Serial.print("  LEN: "); Serial.print(msg.len);
  Serial.print(" DATA: ");
  for ( uint8_t i = 0; i < msg.len; i++ ) {
    Serial.print(msg.buf[i]); Serial.print(" ");
  } Serial.println(" ");
  if (msg.id == nodeId) { // if the board is the same Id

    //turn on the status LED if the current node is the can bus msg node
    sx03->digitalWrite(STATUS_PIN_SX03, HIGH);

    //update all the 3d Pixels
    for ( uint8_t i = 0; i < NUM_3D_PIXELS; i++ ) {
      char str[15];
      sprintf(str, "%X%X", msg.buf[i * 4 + 2], msg.buf[i * 4 + 3]);
      grid(msg.id, i, str, msg.buf[i * 4]);
    }
  }
}

//----------------------------------------------------------------
void grid(int node, int local, char* str, int height) {
  uint64_t color = (uint64_t) strtoull(str, 0, 16);
  int r = ((((color >> 11) & 0x1F) * 527) + 23) >> 6;
  int g = ((((color >> 5) & 0x3F) * 259) + 33) >> 6;
  int b = (((color & 0x1F) * 527) + 23) >> 6;
  Serial.println(local);

  if (r != tempRGB[local].r | g != tempRGB[local].g | b != tempRGB[local].b) {
    int mini = min(r, min(g, b));
    for (int j = 0; j < NUM_LEDS; j++) {
      pixels[local]->setPixelColor(j, pixels[local]->gamma32(pixels[local]->Color(r - mini, g - mini, b - mini, mini))); // Moderately bright green color.
    }

    Serial.print(r - mini); Serial.print(" ");
    Serial.print(g - mini); Serial.print(" ");
    Serial.print(b - mini); Serial.print(" ");
    Serial.print(mini);
    Serial.println(" ");
    pixels[local]->show();
    tempRGB[local] = {r, g, b};
  }
}


//CALIBRATION FUNCTIONS
//----------------------------------------------------------------
void initCalibration() {

  if (sx03->digitalRead(CALIBRATION_STATUS_SX03) == HIGH) {
    Serial.println("Calibration module is connected");
  } else {
    Serial.println("Calibration module is not connected");
  }
}
void updateCalibration() {
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
//----------------------------------------------------------------
void keyCommands() {
  if (Serial.available() > 0) {
    char key = Serial.read();

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
        motors[i]->motorOff();
      }
    }
    if (key == 's') {
      for (int i = 0; i < NUM_3D_PIXELS; i++) {
        motors[i]->motorOn();
      }
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
      for (int i = 0; i < NUM_3D_PIXELS; i++) {
        pixels[i] = new Adafruit_NeoPixel(NUM_LEDS, NEO_PIN[i], NEO_GRBW + NEO_KHZ800);
        pixels[i]->begin();
        pixels[i]->clear();
      }
      //set pixels to different colors
      for (int j = 0; j < NUM_3D_PIXELS; j++) {
        for (int i = 0; i < NUM_LEDS; i++) {
          pixels[j]->setPixelColor(NUM_LEDS, pixels[j]->Color(0, 0, 0, 255));// White
          pixels[j]->show();
        }
      }

      Serial.println("Reset LEDs to white");
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

      for (int j = 0; j < NUM_3D_PIXELS; j++) {
        pixels[j]->setPixelColor(0, pixels[j]->Color(r, g, b, w));
        pixels[j]->setPixelColor(1, pixels[j]->Color(r, g, b, w));
        pixels[j]->show();
      }

      Serial.println("Random LEDs color");
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

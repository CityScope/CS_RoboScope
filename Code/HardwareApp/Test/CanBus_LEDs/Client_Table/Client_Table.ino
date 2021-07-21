#include <FlexCAN_T4.h>
#include <Arduino.h>
#include <TeensyThreads.h>
#include <Wire.h> // Include the I2C library (required)
#include <SparkFunSX1509.h> // Include SX1509 library
#include <Adafruit_NeoPixel.h>
#include "message.h"
#include "InterfacePanel.h"
#include "BoardPins.h"

#define START 0  //example definition
#define DOWN 1  //example definition
#define UP 2
const int SHORT_TIMEOUT = 1000; //button timeout in milliseconds
const int LONG_TIMEOUT = 5000;
FlexCAN_T4FD<CAN3, RX_SIZE_512, TX_SIZE_64>   FD;   // fd port
FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16>     canBus;  // can1 port

int local_name = 1;

const  byte NEO_PIN_01 = 4;
const  byte NEO_PIN_02 = 13;
const  byte NEO_PIN_03 = 14;
const  byte NEO_PIN_04 = 15;
const  byte NEO_PIN_05 = 24;
const  byte NEO_PIN_06 = 25;
const  byte NEO_PIN_07 = 28;
const  byte NEO_PIN_08 = 29;

//NEO PIXELS PINS
const byte NEO_PIN[8] = {NEO_PIN_01, NEO_PIN_02, NEO_PIN_03, NEO_PIN_04,
                         NEO_PIN_05, NEO_PIN_06, NEO_PIN_07, NEO_PIN_08
                        };
boolean colorChange = false;
int colorId = -1;
const byte NUMPIXELS = 4;
Adafruit_NeoPixel * pixels[8];
//InterfacePanel * interfacePanel;

uint8_t state;  //system state (feel free to use)
uint8_t num_count; //variable for storing the number of times the button has been pressed before timeout
unsigned long timer;  //used for storing millis() readings.
int last_time;
int down_initial;
int up_initial;
bool long_press = false;


struct rgb {
  unsigned int   r;
  unsigned int   g;
  unsigned int   b;
};

bool StatusLED = false;

rgb pixel_rgb[8];
//----------------------------------------------------------------
void setup(void) {
  Serial.begin(2000000);
  delay(500);
  Serial.println("Client CAN Bus FD and CAN BUS 1");

  FD.begin();
  CANFD_timings_t config;
  config.clock = CLK_40MHz;
  config.baudrate = 1000000;
  config.baudrateFD = 8000000;
  config.propdelay = 190;
  config.bus_length = 1;
  config.sample = 87.5;
  FD.setRegions(64);
  FD.setBaudRate(config);

  FD.enableMBInterrupts();
  FD.onReceive(reading);
  FD.distribute();

  FD.mailboxStatus();
  Serial.println("Init client done");


  last_time = millis();
  state = START; //start system in IDLE state!
  num_count = 0;

  initPixel();
  for (int i = 0; i < 8; i++) {
    pixel_rgb[i] = {0, 0, 0};
  }

  //STATUS LED


  //  interfacePanel = new InterfacePanel();
  //  interfacePanel->init();
}

//----------------------------------------------------------------
void loop() {
  FD.events();
  //  number_fsm(interfacePanel->getPushCurrentState(2));
}

//----------------------------------------------------------------
void reading(const CANFD_message_t &msg) {
  Serial.print("  Node: "); Serial.print(msg.id);
  Serial.print("  LEN: "); Serial.print(msg.len);
  Serial.print(" DATA: ");
  for ( uint8_t i = 0; i < msg.len; i++ ) {
    Serial.print(msg.buf[i]); Serial.print(" ");
  } Serial.println(" ");
  if (msg.id == local_name) {
    for ( uint8_t i = 0; i < 8; i++ ) {
      char str[15];
      sprintf(str, "%X%X", msg.buf[i * 4 + 2], msg.buf[i * 4 + 3]);
      grid(msg.id, i, str, msg.buf[i * 4]);
    }
  }

  StatusLED = true;
}

//----------------------------------------------------------------
void grid(int node, int local, char* str, int height) {
  uint64_t color = (uint64_t) strtoull(str, 0, 16);
  int r = ((((color >> 11) & 0x1F) * 527) + 23) >> 6;
  int g = ((((color >> 5) & 0x3F) * 259) + 33) >> 6;
  int b = (((color & 0x1F) * 527) + 23) >> 6;
  Serial.println(local);
  if (r != pixel_rgb[local].r | g != pixel_rgb[local].g | b != pixel_rgb[local].b) {
    int mini = min(r, min(g, b));
    for (int j = 0; j < NUMPIXELS; j++) {
      pixels[local]->setPixelColor(j, pixels[local]->gamma32(pixels[local]->Color(r - mini, g - mini, b - mini, mini))); // Moderately bright green color.
    }
    Serial.print(r - mini); Serial.print(" ");
    Serial.print(g - mini); Serial.print(" ");
    Serial.print(b - mini); Serial.print(" ");
    Serial.print(mini);
    Serial.println(" ");
    pixels[local]->show();
    pixel_rgb[local] = {r, g, b};
  }
}

//----------------------------------------------------------------
void initPixel() {
  for (int i = 0; i < 8; i++) {
    pixels[i] = new Adafruit_NeoPixel(NUMPIXELS, NEO_PIN[i], NEO_GRBW + NEO_KHZ800);
    pixels[i]->begin();
    pixels[i]->clear();
  }

  //set pixels to different colors
  for (int j = 0; j < 8; j++) {
    pixels[j]->setPixelColor(0, pixels[j]->Color(0, 0, 0, 255)); // Moderately bright green color.
    pixels[j]->setPixelColor(1, pixels[j]->Color(0, 0, 0, 255)); // Moderately bright green color.
    pixels[j]->setPixelColor(2, pixels[j]->Color(0, 0, 0, 255)); // Moderately bright green color.
    pixels[j]->setPixelColor(3, pixels[j]->Color(0, 0, 0, 255)); // Moderately bright green color.
    pixels[j]->show();
  }
  Serial.println("Done LEDs");
}
//
//void number_fsm(uint8_t input){
//  switch(state){
//    case START:
//      if (num_count!=0) {
//        byte byte3 = 0;
//        if (long_press == true) {
//          Serial.println("long press");
//          bitSet(byte3, 6);
//        } else {
//          Serial.println("short press");
//          bitSet(byte3, 7);
//        }
//
//        if (num_count > 1) {
//          bitSet(byte3, 0);
//        }
//
////        CANMotorMessage msg = CANMotorMessage(0);
//        for (int i = 0; i < 8; i++) {
////          msg.addMessage(i, 10, 255, byte3, 0);
//        }
//        FD.write(msg.getCANmessage(32));
//        Serial.println(num_count);
//        Serial.println(byte3);
//        num_count=0;
//        long_press = false;
//      }
//
//      if (input==1){
//        down_initial = millis();
//        state = DOWN;
//        Serial.println("pushed");
//      }
//      break; //don't forget break statements
//    case DOWN:
//      if ((millis()-down_initial)< SHORT_TIMEOUT) {
//        if (millis()-down_initial > 20 && input==0) {
//          up_initial = millis();
//          state = UP;
//          Serial.println("released\n");
//        }
//      } else if ((millis()-down_initial) > SHORT_TIMEOUT && input==0) {
//          up_initial = millis();
//          state = UP;
//          long_press = true;
//          Serial.println("released long\n");
//      } else if ((millis()-down_initial) > LONG_TIMEOUT) {
//        state = START;
//        Serial.println("start");
//      }
//      break;
//    case UP:
//      if ((millis()-up_initial)< SHORT_TIMEOUT && long_press == false) {
//        if (millis()-up_initial > 20 && input==1) {
//          num_count++;
//          down_initial = millis();
//          state = DOWN;
//          Serial.println("pushed");
//        }
//      } else {
//        num_count++;
//        state = START;
//        Serial.println("start");
//      }
//      break;
//  }
//}

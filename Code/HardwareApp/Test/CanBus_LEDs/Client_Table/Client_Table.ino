#include <FlexCAN_T4.h>
#include <Arduino.h>
#include <TeensyThreads.h>
#include <Wire.h> // Include the I2C library (required)
#include <SparkFunSX1509.h> // Include SX1509 library
#include <Adafruit_NeoPixel.h>
#include "message.h"

FlexCAN_T4FD<CAN3, RX_SIZE_512, TX_SIZE_64>   FD;   // fd port
FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16>     canBus;  // can1 port

int local_name = 0;

unsigned long timer;
int counter; 
bool change;

const int MOTOR_ID = 9;

const  byte NEO_PIN_01 = 16;
const  byte NEO_PIN_02 = 17;
const  byte NEO_PIN_03 = 2;
const  byte NEO_PIN_04 = 3;
const  byte NEO_PIN_05 = 4;
const  byte NEO_PIN_06 = 24;
const  byte NEO_PIN_07 = 25;
const  byte NEO_PIN_08 = 13;

//NEO PIXELS PINS
const byte NEO_PIN[8] = {NEO_PIN_01, NEO_PIN_02, NEO_PIN_03, NEO_PIN_04,
                         NEO_PIN_05, NEO_PIN_06, NEO_PIN_07, NEO_PIN_08
                        };
boolean colorChange = false;
int colorId = -1;
const byte NUMPIXELS = 4;
Adafruit_NeoPixel * pixels[8];

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
  
  initPixel();
}

//----------------------------------------------------------------
void loop() {
  FD.events();
//  if (Serial.available()>0){
//    int node = Serial.parseInt();
//    CANMotorMessage msg = CANMotorMessage(node);
//    for (int i = 0; i < 8; i++) {
//      msg.addMessage(i, 10, 20, 224, 195);
//    }
//    FD.write(msg.getCANmessage(32));
//  }
}

//----------------------------------------------------------------
void reading(const CANFD_message_t &msg) {
  counter++;
  Serial.println(counter);
  Serial.print("  Node: "); Serial.print(msg.id);
  Serial.print("  LEN: "); Serial.print(msg.len);
  Serial.print(" DATA: ");
  for ( uint8_t i = 0; i < msg.len; i++ ) {
    Serial.print(msg.buf[i]); Serial.print(" ");
  } Serial.println(" ");
  if (msg.id==local_name){
    for ( uint8_t i = 0; i < 8; i++ ) {
      char str[15];
      sprintf(str, "%X%X", msg.buf[i*4+2], msg.buf[i*4+3]); 
      grid(msg.id, i, str, msg.buf[i*4]);
    }
  }
}

//----------------------------------------------------------------
void grid(int node, int local, char* str, int height) {
  uint64_t color = (uint64_t) strtoull(str, 0, 16); 
  int r = ((((color >> 11) & 0x1F) * 527) + 23) >> 6;
  int g = ((((color >> 5) & 0x3F) * 259) + 33) >> 6;
  int b = (((color & 0x1F) * 527) + 23) >> 6;
  
  for (int j = 0; j < NUMPIXELS; j++) {
    pixels[local]->setPixelColor(j, pixels[local]->gamma32(pixels[local]->Color(r, g, b))); // Moderately bright green color.
  }
  pixels[local]->show();
}

//----------------------------------------------------------------

//----------------------------------------------------------------
void initPixel() {
  for (int i = 0; i < 8; i++) {
    pixels[i] = new Adafruit_NeoPixel(NUMPIXELS, NEO_PIN[i], NEO_GRBW + NEO_KHZ800);
    pixels[i]->begin();
    pixels[i]->clear();
  }
  for (int j = 0; j < NUMPIXELS; j++) {
    pixels[0]->setPixelColor(j, pixels[0]->Color(255, 0, 0)); // Moderately bright green color.
    pixels[1]->setPixelColor(j, pixels[1]->Color(255, 255, 0)); // Moderately bright green color.
    pixels[2]->setPixelColor(j, pixels[2]->Color(255, 0, 255)); // Moderately bright green color.
    pixels[3]->setPixelColor(j, pixels[3]->Color(0, 255, 255)); // Moderately bright green color.
    pixels[4]->setPixelColor(j, pixels[4]->Color(0, 255, 0)); // Moderately bright green color.
    pixels[5]->setPixelColor(j, pixels[5]->Color(50, 155, 255)); // Moderately bright green color.
    pixels[6]->setPixelColor(j, pixels[6]->Color(0, 0, 255)); // Moderately bright green color.
    pixels[7]->setPixelColor(j, pixels[7]->Color(255, 255, 255)); // Moderately bright green color.
  } 

  for (int i = 0; i < 8; i++) {
    pixels[i]->show();
  }
}

/*
   Can Bus Client
   Sending Can Bus FD
   Sending Can TD 0
*/
#include <FlexCAN_T4.h>
#include "message.h"
#include <SparkFunSX1509.h>
#include "Pins.h"
#include <Wire.h>

#define MSG_SIZE           32

//dip switch  and status
SX1509 sx03;

//CAN BUS FD
FlexCAN_T4FD<CAN3, RX_SIZE_256, TX_SIZE_32>   FD;   // fd port

//CAN BUS 1
FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16>     canBus;  // can1 port

int local_name = 0;
int msg_array[8] = {1, 2, 3, 4, 32, 6, 7, 8};
int state;
uint32_t timer;

//Key pins
const int KEY_PIN_01 = 6;
const int KEY_PIN_02 = 7;
const int KEY_PIN_03 = 8;
const int KEY_PIN_04 = 9;

const int LED_FD = 22; //22
const int LED_16 = 23; //23

const int MOTOR_ID = 0;

//----------------------------------------------------------------
void setup(void) {
  Serial.begin(115200);
  delay(500);
  Serial.println("Client CAN Bus FD and CAN BUS 1");

  //---------------------------------------
  int muxCounter = 0;
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
  delay(200);

  sx03.pinMode(STATUS_PIN_SX03, OUTPUT);
  sx03.digitalWrite(STATUS_PIN_SX03, HIGH);
  delay(500);
  sx03.digitalWrite(STATUS_PIN_SX03, LOW);


  //setup CAN 0 BUS
  canBus.begin();
  canBus.setBaudRate(500000);     // 500kbps data rate
  canBus.enableFIFO();
  canBus.enableFIFOInterrupt();
  canBus.onReceive(FIFO, canBusSniff);
  canBus.mailboxStatus();

  //setup CAN FD bus
  FD.begin();
  CANFD_timings_t config;
  config.clock = CLK_40MHz; //40. CLK_80MHz
  config.baudrate = 1000000;
  config.baudrateFD = 8000000;
  config.propdelay = 190;
  config.bus_length = 1;
  config.sample = 87.5;
  FD.setRegions(64);
  FD.setBaudRate(config);
  FD.mailboxStatus();

  //Key values
  pinMode(KEY_PIN_01, INPUT);
  pinMode(KEY_PIN_02, INPUT);
  pinMode(KEY_PIN_03, INPUT);
  pinMode(KEY_PIN_04, INPUT);

  //turn on LEDs
  pinMode(LED_FD, OUTPUT);
  pinMode(LED_16, OUTPUT);

  digitalWrite(LED_FD, HIGH); //CAN Bus FD
  digitalWrite(LED_16, HIGH); //CAN Bus 2.0
  delay(1000);

  digitalWrite(LED_FD, LOW);
  digitalWrite(LED_16, LOW);

  state = HIGH;
  timer = millis();

  Serial.println("Init client done");
}

//----------------------------------------------------------------
void loop() {
  CANFD_message_t msg;
  sx03.digitalWrite(STATUS_PIN_SX03, LOW);
  digitalWrite(LED_FD, LOW); 

  if (FD.read(msg)) {
    Serial.println(" ");
    reading(msg);
    sx03.digitalWrite(STATUS_PIN_SX03, HIGH);
    digitalWrite(LED_FD, HIGH); 
  }
  
}

//----------------------------------------------------------------
void canBusSniff(const CAN_message_t &msg) { // global callback
  digitalWrite(LED_FD, HIGH);

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

  digitalWrite(LED_FD, LOW); //CAN Bus FD

}

//----------------------------------------------------------------
void reading(CANFD_message_t msg) {
  //turn on LED
  digitalWrite(LED_FD, HIGH);
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
  digitalWrite(LED_FD, LOW);
}
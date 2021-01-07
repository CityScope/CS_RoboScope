#include <Arduino.h>
#include "UrbanPanel.h"
#include "message.h"
#include <TeensyThreads.h>
#include <FlexCAN_T4.h>

FlexCAN_T4FD<CAN3, RX_SIZE_512, TX_SIZE_64> FD;

FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16> Can2;


UrbanPanel * urbanPanel;

int nodeID = 0; //TODO Set for each motor control board
bool keyboardInput = false;
unsigned long timer;

void setup() {
  Serial.begin(2000000);
  Serial.println("Starting");
  delay(2000);


  //Motor Panel
  Serial.println("Setting Motors");
  urbanPanel = new UrbanPanel(nodeID);
  urbanPanel->init();


  Serial.println("Setting up CAN Bus FD and CAN BUS");

  Can2.begin();
  Can2.setBaudRate(500000);     // 500kbps data rate
  Can2.enableFIFO();
  Can2.enableFIFOInterrupt();
  Can2.onReceive(FIFO, canSniff);
  Can2.mailboxStatus();

  //setup CAN FD bus
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
  FD.onReceive(canFDSniff);
  FD.distribute();

  FD.mailboxStatus();
  Serial.println("Init client done");


  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN_SWITCH), limitswitch, CHANGE);
}

void canSniff(const CAN_message_t &msg) {
  Serial.print("MB "); Serial.print(msg.mb);
  Serial.print("  OVERRUN: "); Serial.print(msg.flags.overrun);
  Serial.print("  LEN: "); Serial.print(msg.len);
  Serial.print(" EXT: "); Serial.print(msg.flags.extended);
  Serial.print(" TS: "); Serial.print(msg.timestamp);
  Serial.print(" ID: "); Serial.print(msg.id, HEX);
  Serial.print(" Buffer: ");
  for ( uint8_t i = 0; i < msg.len; i++ ) {
    Serial.print(msg.buf[i], HEX); Serial.print(" ");
  } Serial.println();
}

void canFDSniff(const CANFD_message_t &msg) {
    Serial.print("  Node: "); Serial.print(msg.id);
    Serial.print("  LEN: "); Serial.print(msg.len);
    Serial.print(" DATA: ");

    for ( uint8_t i = 0; i < msg.len; i++ ) {
      Serial.print(msg.buf[i]); Serial.print(" ");
    }
    Serial.println(" ");

    for ( uint8_t i = 0; i < 8; i++ ) {
      char str[15];
      sprintf(str, "%X%X", msg.buf[i*4+2], msg.buf[i*4+3]);
      // grid(msg.id, i, str, msg.buf[i*4]);
    }
}



void limitswitch() {
  urbanPanel->limitswitch();
}

void loop() {
  urbanPanel->motorTimerUpdate();
  FD.events();


  if (Serial.available() > 0) {
    test(Serial.parseInt());

    if (keyboardInput) {
      keyboardInputTest();
    }


/*
    if (key == 'q') {
      urbanPanel->setPixelColor(0, 255, 0, 0);
      Serial.println("LED red");
    }

    if (key == 'w') {
      urbanPanel->setPixelColor(0, 0, 255, 0);
      Serial.println("LED red");
    }

    if (key == 'e') {
      urbanPanel->setPixelColor(0, 0, 0, 255);
      Serial.println("LED red");
    }*/
  }
}

void keyboardInputTest() {
  char key = Serial.read();
  if (key == 'q') {
    urbanPanel->movePixelUp(0,5);
  }
  if (key == 'w') {
    urbanPanel->movePixelUp(1,5);
  }
  if (key == 'e') {
    urbanPanel->movePixelUp(2,5);
  }
  if (key == 'r') {
    urbanPanel->movePixelUp(3,5);
  }
  if (key == 't') {
    urbanPanel->movePixelUp(4,5);
  }
  if (key == 'y') {
    urbanPanel->movePixelUp(5,5);
  }
  if (key == 'u') {
    urbanPanel->movePixelUp(6,5);
  }
  if (key == 'i') {
    urbanPanel->movePixelUp(7,5);
  }


  if (key == 'a') {
    urbanPanel->movePixelDown(0,5);
  }
  if (key == 's') {
    urbanPanel->movePixelDown(1,5);
  }
  if (key == 'd') {
    urbanPanel->movePixelDown(2,5);
  }
  if (key == 'f') {
    urbanPanel->movePixelDown(3,5);
  }
  if (key == 'g') {
    urbanPanel->movePixelDown(4,5);
  }
  if (key == 'h') {
    urbanPanel->movePixelDown(5,5);
  }
  if (key == 'j') {
    urbanPanel->movePixelDown(6,5);
  }
  if (key == 'k') {
    urbanPanel->movePixelDown(7,5);
  }
}

void setPixel(int node, int local, char* str, int height) {
  uint64_t color = (uint64_t) strtoull(str, 0, 16);
  int r = ((((color >> 11) & 0x1F) * 527) + 23) >> 6;
  int g = ((((color >> 5) & 0x3F) * 259) + 33) >> 6;
  int b = (((color & 0x1F) * 527) + 23) >> 6;

  urbanPanel->movePixelUp(local, height);
  urbanPanel->setPixelColor(local, r, g, b);

}

void test(int node) {
  if (node != nodeID) {
    return
  }
  CANMotorMessage msg = CANMotorMessage(node);
  for (int i =0; i < 8; i++) {
    msg.addMessage(i, 10, 170, 105, 243);
  }
  FD.write(msg.getCANmessage());

  char str[15];
  sprintf(str, "%X%X", 105, 243);
  for (int i =0; i < 8; i++) {
    setPixel(node, i, str, 10);
  }
  change=true;
  timer = millis();
}

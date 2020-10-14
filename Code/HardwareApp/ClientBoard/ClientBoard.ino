#include <Arduino.h>
#include "UrbanPanel.h"
#include <TeensyThreads.h>
#include <FlexCAN_T4.h>
FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> Can0;

UrbanPanel * urbanPanel;

int panelID = 0;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  Serial.println("Starting");
  delay(2000);


  //Motor Panel
  Serial.println("Setting Motors");
  urbanPanel = new UrbanPanel(panelID);
  urbanPanel->init();


  Can0.begin();
  Can0.setBaudRate(1000000);
  Can0.setMaxMB(16);
  Can0.enableFIFO();
  Can0.enableFIFOInterrupt();
  Can0.onReceive(canSniff);
  Can0.mailboxStatus();

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

void limitswitch() {
  urbanPanel->limitswitch();
}

void loop() {
  urbanPanel->motorTimerUpdate();
  Can0.events();

  if (Serial.available() > 0) {
    char key = Serial.read();
    if (key == 'a') {
      urbanPanel->movePixelUp(0,30);
      urbanPanel->movePixelUp(1,30);
      urbanPanel->movePixelUp(2,30);
      urbanPanel->movePixelUp(3,30);
      urbanPanel->movePixelUp(4,30);
      urbanPanel->movePixelUp(5,30);
      urbanPanel->movePixelUp(6,30);
      urbanPanel->movePixelUp(7,30);
      Serial.println("forward");
    }
  }
}

// void pLoop(int i) {
//   while(1) {
//   urbanPanel->pixelLoop(i);
//   }
// }

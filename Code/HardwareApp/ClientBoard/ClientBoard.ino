#include <Arduino.h>
#include "UrbanPanel.h"
#include "message.h"
#include <TeensyThreads.h>
#include <FlexCAN_T4.h>

// CAN BUS FD
FlexCAN_T4FD<CAN3, RX_SIZE_512, TX_SIZE_64> FD;

// CAN BUS 1
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
  Serial.println("Init client done");


  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN_SWITCH), limitswitch, CHANGE);
}


void limitswitch() {
  urbanPanel->limitswitch();
}

void loop() {
  urbanPanel->motorTimerUpdate();


  if (Serial.available() > 0) {
    //test(Serial.parseInt());

    if (keyboardInput) {
      keyboardInputTest();
    }
  }

}

void keyboardInputTest() {
  char key = Serial.read();
  if (key == 'q') {
    // TODO
  }
}

void setPixel(int node, int local, char* str, int height) {
  uint64_t color = (uint64_t) strtoull(str, 0, 16);
  int r = ((((color >> 11) & 0x1F) * 527) + 23) >> 6;
  int g = ((((color >> 5) & 0x3F) * 259) + 33) >> 6;
  int b = (((color & 0x1F) * 527) + 23) >> 6;

  //urbanPanel->movePixelUp(local, height);
  urbanPanel->setPixelColor(local, r, g, b);

}

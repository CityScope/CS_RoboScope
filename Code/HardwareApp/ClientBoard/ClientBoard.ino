#include <Arduino.h>
#include "UrbanPanel.h"
#include <TeensyThreads.h>

UrbanPanel     * urbanPanel;

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


  // for (int i=0; i < 8; i++) {
  //   threads.addThread(pLoop, i);
  // }
  
  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN_SWITCH), limitswitch, CHANGE);
}

void limitswitch() {
  urbanPanel->limitswitch();
}

void loop() {
  urbanPanel->motorTimerUpdate();

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

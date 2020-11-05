#include <Arduino.h>
#include "UrbanPanel.h"
#include <TeensyThreads.h>
#include <FlexCAN_T4.h>
FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> Can0;

UrbanPanel * urbanPanel;

int panelID = 0;


int trq1Mode = 1;
int trq2Mode = 1;


void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  Serial.println("Starting");
  delay(2000);


  //Motor Panel
  Serial.println("Setting Motors");
  urbanPanel = new UrbanPanel(panelID);
  urbanPanel->init();

  pinMode(TRQ1_PIN, OUTPUT);
  pinMode(TRQ2_PIN, OUTPUT);

  if (trq1Mode == 1) {
    digitalWrite(TRQ1_PIN, HIGH);
  }

  if (trq2Mode == 1) {
    digitalWrite(TRQ2_PIN, HIGH);
  }



 // Can0.begin();
 // Can0.setBaudRate(1000000);
 // Can0.setMaxMB(16);
 // Can0.enableFIFO();
 // Can0.enableFIFOInterrupt();
 // Can0.onReceive(canSniff);
  //Can0.mailboxStatus();

//  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN_SWITCH), limitswitch, CHANGE);
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
 // urbanPanel->limitswitch();
}

void loop() {
  urbanPanel->update();
  //Can0.events();

  if (Serial.available() > 0) {
    char key = Serial.read();
    if (key == 'a') {
      urbanPanel->movePixelUp(0, 30);
      urbanPanel->movePixelUp(1, 30);
      urbanPanel->movePixelUp(2, 30);
      urbanPanel->movePixelUp(3, 30);
      urbanPanel->movePixelUp(4, 30);
      urbanPanel->movePixelUp(5, 30);
      urbanPanel->movePixelUp(6, 30);
      urbanPanel->movePixelUp(7, 30);
      Serial.println("forward");
    }

    if (key == 'd') {
      urbanPanel->disableMotors();
      Serial.println("disable");
    }

    if (key == 'e') {
      urbanPanel->enableMotors();
      Serial.println("enable");
    }


    //motor test
    if (key == '1') {
      int motorId = 0;
      if ( urbanPanel->isEnable(motorId)) {
        urbanPanel->disableMotor(motorId);
      } else {
        urbanPanel->movePixelUp(motorId, 30);
        urbanPanel->enableMotor(motorId);
      }
      Serial.println(motorId);
    }
    
    if (key == '2') {
      int motorId = 1;
      if ( urbanPanel->isEnable(motorId)) {
        urbanPanel->disableMotor(motorId);
      } else {

        urbanPanel->movePixelUp(motorId, 30);
        urbanPanel->enableMotor(motorId);
      }
      Serial.println(motorId);
    }
    
    if (key == '3') {
      int motorId = 2;
      if ( urbanPanel->isEnable(motorId)) {
        urbanPanel->disableMotor(motorId);
      } else {
        urbanPanel->movePixelUp(motorId, 30);
        urbanPanel->enableMotor(motorId);
      }
      Serial.println(motorId);
    }
    if (key == '4') {
      int motorId = 3;
      if ( urbanPanel->isEnable(motorId)) {
        urbanPanel->disableMotor(motorId);
      } else {
        urbanPanel->movePixelUp(motorId, 30);
        urbanPanel->enableMotor(motorId);
      }
      Serial.println(motorId);
    }
    if (key == '5') {
      int motorId = 4;
      if ( urbanPanel->isEnable(motorId)) {
        urbanPanel->disableMotor(motorId);
      } else {
        urbanPanel->movePixelUp(motorId, 30);
        urbanPanel->enableMotor(motorId);
      }
      Serial.println(motorId);
    }
    if (key == '6') {
      int motorId = 5;
      if ( urbanPanel->isEnable(motorId)) {
        urbanPanel->disableMotor(motorId);
      } else {
        urbanPanel->movePixelUp(motorId, 30);
        urbanPanel->enableMotor(motorId);
      }
      Serial.println(motorId);
    }
    if (key == '7') {
      int motorId = 6;
      if ( urbanPanel->isEnable(motorId)) {
        urbanPanel->disableMotor(motorId);
      } else {
        urbanPanel->movePixelUp(motorId, 30);
        urbanPanel->enableMotor(motorId);
      }
      Serial.println(motorId);
    }
    if (key == '8') {
      int motorId = 7;
      if ( urbanPanel->isEnable(motorId)) {
        urbanPanel->disableMotor(motorId);
      } else {
        urbanPanel->movePixelUp(motorId, 30);
        urbanPanel->enableMotor(motorId);
      }
      Serial.println(motorId);
    }
  }
}

// void pLoop(int i) {
//   while(1) {
//   urbanPanel->pixelLoop(i);
//   }
// }

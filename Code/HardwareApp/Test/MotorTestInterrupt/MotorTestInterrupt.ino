#include <Arduino.h>
#include "Stepper.h"
#include "BoardPins.h"
#include <TeensyThreads.h>
#include "InterfacePanel.h"
#include "MotorPanel.h"

//false -> backwards
//true  -> forwards
boolean dirMotor = false;

//current motor
int currentMotor = 2;

boolean enableMotor = true;

//
InterfacePanel * interfacePanel;
MotorPanel     * motorPanel;

int panelID = 0;

volatile boolean limitActivated = false;
volatile boolean limitActivatedPrev = false;
boolean enableReturn   = false;

bool activateLEDs = false;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  Serial.println("Starting");
  delay(2000);

  //
  interfacePanel = new InterfacePanel(panelID);
  Serial.println("Setting Interface");
  interfacePanel->init();
  //interfacePanel->changeRandColor();

  //Motor Panel
  Serial.println("Setting Motors");
  motorPanel     = new MotorPanel(panelID);
  motorPanel->init();

  /*

    motorPanel->startMoveForward(0, 30);
    motorPanel->startMoveForward(1, 30);
    motorPanel->startMoveForward(2, 40);
    motorPanel->startMoveForward(3, 40);
    motorPanel->startMoveForward(4, 40);
    motorPanel->startMoveForward(5, 40);
    motorPanel->startMoveForward(6, 40);
    motorPanel->startMoveForward(7, 40);

  */
  //motorPanel->startMoveBackward(0, 5);
  //motorPanel->startMoveBackward(1, 45);
  // motorPanel->startMoveBackward(2, 45);
  // motorPanel->startMoveBackward(3, 5);
  // motorPanel->startMoveBackward(4, 45);
  //  motorPanel->startMoveBackward(5, 45);
  // motorPanel->startMoveBackward(6, 45);
  //  motorPanel->startMoveBackward(7, 45);


  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN_SWITCH), limitswitch, CHANGE  );
}

void loop() {

  if (activateLEDs) {
    //interfacePanel->rainbow(millis());
  }

  //timer for moving up and down several motors at once
  for (int i = 0 ; i < MOTORS_PER_PANEL; i++) {
    if (motorPanel->getMotor(i).isMotorStop()) {
      motorPanel->setDisable(i);
      Serial.println("waintg lock");

      //limit switch
      if ( !motorPanel->getMotor(i).isMotorLock()) {
        bool lock = motorPanel->updateLock(i, millis());
        if (lock) {
          motorPanel->setEnable(i);
          motorPanel->getMotor(i).startMoveForward( 5 );
          motorPanel->getMotor(i).resetlimit();

          //
          //interfacePanel->changeRandColor();

          Serial.print(i);
          Serial.println(": lock and back");
        }
      }



    }
  }

  //lock the button and push up
  for (int i = 0 ; i < MOTORS_PER_PANEL; i++) {
    unsigned waitTimeMicros = motorPanel->getMotor(i).getNextAction();
    if (waitTimeMicros <= 0) {
      //motorPanel->getMotor(i).disable();
      //Serial.print(i);
      //Serial.println(": stop motor");
    }
  }


  // limitActivatedPrev = limitActivated;
  if (limitActivated) {

    //limit
    interfacePanel->updateLimitState();
    //interfacePanel->updatePushState();

    //get limit
    for (int i = 0; i < MOTORS_PER_PANEL; i++) {
      if (interfacePanel->getLimitSwitchState(i)) {
        motorPanel->getMotor(i).activeLimit();
        Serial.print(i);
        Serial.println(" :limit up");
      }
      if (interfacePanel->getLimitState(i)) {
        interfacePanel->resetLimitSwitch(i);
      }
    }

    //push
   // if (!activateLEDs) {
      for (int i = 0; i < MOTORS_PER_PANEL; i++) {
        if (interfacePanel->getPushSwitchState(i)) {
          motorPanel->getMotor(i).activeLimit();
          Serial.print(i);
          Serial.println(" :push down");
        }
        if (interfacePanel->getPushState(i)) {
          interfacePanel->resetPushSwitch(i);
        }
      }
    //}

    limitActivated = false;
  }




  if (Serial.available() > 0) {
    char key = Serial.read();
    if (key == 'a') {
      motorPanel->startMoveForward(0, 55);
      motorPanel->startMoveForward(1, 55);
      motorPanel->startMoveForward(2, 55);
      motorPanel->startMoveForward(3, 55);
      motorPanel->startMoveForward(4, 55);
      motorPanel->startMoveForward(5, 55);
      motorPanel->startMoveForward(6, 55);
      motorPanel->startMoveForward(7, 55);
      Serial.println("forward");
    }
    if (key == 's') {
      motorPanel->startMoveBackward(0, 58);
      motorPanel->startMoveBackward(1, 58);
      motorPanel->startMoveBackward(2, 58);
      motorPanel->startMoveBackward(3, 58);
      motorPanel->startMoveBackward(4, 58);
      motorPanel->startMoveBackward(5, 58);
      motorPanel->startMoveBackward(6, 58);
      motorPanel->startMoveBackward(7, 58);
      Serial.println("back");
    }

    if (key == '1') {
      motorPanel->startMoveForward(0, 10);
      motorPanel->startMoveForward(1, 10);
      motorPanel->startMoveForward(2, 10);
      motorPanel->startMoveForward(3, 10);
      motorPanel->startMoveForward(4, 10);
      motorPanel->startMoveForward(5, 10);
      motorPanel->startMoveForward(6, 10);
      motorPanel->startMoveForward(7, 10);
      Serial.println("forward");
    }

    if (key == '2') {
      motorPanel->startMoveBackward(0, 10);
      motorPanel->startMoveBackward(1, 10);
      motorPanel->startMoveBackward(2, 10);
      motorPanel->startMoveBackward(3, 10);
      motorPanel->startMoveBackward(4, 10);
      motorPanel->startMoveBackward(5, 10);
      motorPanel->startMoveBackward(6, 10);
      motorPanel->startMoveBackward(7, 10);
      Serial.println("back");
    }

    if (key == '3') {
      activateLEDs = true;
      Serial.println("activate lights");
    }

    if (key == '4') {
      activateLEDs = false;
      Serial.println("disable lights");
    }


  }

}


//
//https://arduino.stackexchange.com/questions/20608/pass-classs-public-function-as-an-argument-to-external-command
void limitswitch() {
  limitActivated = true;
}

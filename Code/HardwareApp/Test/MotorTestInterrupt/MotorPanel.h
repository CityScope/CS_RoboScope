/*

   The Motor Panel controls the output for all the motors available to the urban panel
*/
#ifndef MOTOR_PANEL_H
#define MOTOR_PANEL_H

#include <Arduino.h>

#include "StepperMotor.h"
#include "BoardPins.h"

class MotorPanel {
  public:

    //constructor
    MotorPanel(int pId) {
      id = pId;

      // motors = new StepperMotor[MOTORS_PER_PANEL];
      int motorDirPins[]  = {DIR_PIN_01, DIR_PIN_02 , DIR_PIN_03, DIR_PIN_04, DIR_PIN_05, DIR_PIN_06, DIR_PIN_07, DIR_PIN_08};
      int motorStepPins[] = {STEP_PIN_01, STEP_PIN_02, STEP_PIN_03, STEP_PIN_04, STEP_PIN_05, STEP_PIN_06, STEP_PIN_07, STEP_PIN_08};

      for (int i = 0; i < MOTORS_PER_PANEL; i++) {
        motors[i] = new StepperMotor(i, GMOTOR_STEPS, motorDirPins[i], motorStepPins[i], GENABLE_PIN, GM0_PIN, GM1_PIN);
      }
    }

    // initialize motors
    void init() {
      for (int i = 0; i < MOTORS_PER_PANEL; i++) {
        motors[i]->setRPM(GRPM);
        motors[i]->init();
      }
    }

    int getNumberOfMotors() {
      return MOTORS_PER_PANEL;
    }

    // Returns a motor based on id
    StepperMotor & getMotor(int i) {
      return *motors[i];
    }

    // Prints the motor info
    void printDebugInfo() {
      for (int i = 0; i < MOTORS_PER_PANEL; i++) {
        motors[i]->printMotorInfo();
      }
    }

    void startMoveForward(int id, int steps) {
      motors[id]->startMoveForward(steps);
    }
    void startMoveBackward(int id, int steps) {
      motors[id]->startMoveBackward(steps);
    }

    void setDisable(int i) {
      motors[i]->disable();
    }
    void setEnable(int i) {
      motors[i]->enable();
    }

    bool updateLock(int i, long time) {
      return motors[i]->updateLock(time);
    }

  private:
    int id;
    StepperMotor * motors[MOTORS_PER_PANEL];

};

#endif

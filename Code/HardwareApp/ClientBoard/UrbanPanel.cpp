#include "UrbanPanel.h"

UrbanPanel::UrbanPanel(int panelId) {
  id = panelId;

  // Set up the motors
  int motorDirPins[]  = {DIR_PIN_01, DIR_PIN_02 , DIR_PIN_03, DIR_PIN_04, DIR_PIN_05, DIR_PIN_06, DIR_PIN_07, DIR_PIN_08};
  int motorStepPins[] = {STEP_PIN_01, STEP_PIN_02, STEP_PIN_03, STEP_PIN_04, STEP_PIN_05, STEP_PIN_06, STEP_PIN_07, STEP_PIN_08};

  for (int i = 0; i < MOTORS_PER_PANEL; i++) {
    motors[i] = new StepperMotor(i, GMOTOR_STEPS, motorDirPins[i], motorStepPins[i], GENABLE_PIN, GM0_PIN, GM1_PIN);
    //waitTimeMicros[i] = 0;
    //threads.addThread(motorThreadTimer, i);
    motors[i]->setRPM(GRPM);
    motors[i]->init();
  }

  interfacePanel = new InterfacePanel();
  //displayPanel = new DisplayPanel();

}

int UrbanPanel::getPushState(int i) {
  return interfacePanel->getPushCurrentState(i);
}

int UrbanPanel::getLimitState(int i) {
  return interfacePanel->getLimitCurrentState(i);
}


void UrbanPanel::init() {
  for (int i = 0; i < MOTORS_PER_PANEL; i++) {
    //waitTimeMicros[i] = 0;
    //threads.addThread(motorThreadTimer, i);
    motors[i]->setRPM(GRPM);
    motors[i]->init();
  }
  interfacePanel->init();


}

void UrbanPanel::motorTimerUpdate() {
  for (int i = 0; i < MOTORS_PER_PANEL; i++) {
    if (motors[i]->isMotorStop()) {
      motors[i]->disable();
      //Waiting lock

      if (!motors[i]->isMotorLock()) {
        bool lock = motors[i]->updateLock(millis());

        if (lock) {
          motors[i]->enable();
          motors[i]->startMoveForward(5);
          motors[i]->resetLimit();
        }
      }
    }
  }

  //lock the button and push up
  for (int i = 0 ; i < MOTORS_PER_PANEL; i++) {
    unsigned waitTimeMicros = motors[i]->getNextAction();
    if (waitTimeMicros <= 0) {
      //motorPanel->getMotor(i).disable();
      //Serial.print(i);
      //Serial.println(": stop motor");
    }
  }

  if (limitActivated) {
    interfacePanel->updateLimitState();

    for (int i = 0; i < MOTORS_PER_PANEL; i++) {
      if (interfacePanel->getLimitSwitchState(i)) {
        motors[i]->activeLimit();
      }
      if (interfacePanel->getLimitState(i)) {
        interfacePanel->resetLimitSwitch(i);
      }
    }

    for (int i = 0; i < MOTORS_PER_PANEL; i++) {
      if (interfacePanel->getPushSwitchState(i)) {
        motors[i]->activeLimit();
      }
      if (interfacePanel->getPushState(i)) {
        interfacePanel->resetPushSwitch(i);
      }
    }

    limitActivated = false;

  }

}

void UrbanPanel::limitswitch() {
  limitActivated = true;
}


/*
*/
void UrbanPanel::movePixelUp(int i, int steps) {
  //motorPanel->startMove
  //motors[i]->moveForward();
  motors[i]->startMoveForward(steps);

}

void UrbanPanel::movePixelDown(int i, int steps) {
  //motorPanel->startMove
  motors[i]->startMoveBackward(steps);
}

/*
  Moves the defined pixel to a specified position
*/
void UrbanPanel::setPixelPosition(int i, float pos) {


}

void UrbanPanel::pixelLoop(int i) {
    waitTimeMicros[i] = motors[i]->getNextAction();

    /*
    if (getLimitState(0) == 1) {
      movePixelDown(i, 5);
      motors[i]->stop();
    }*/
    if (waitTimeMicros[i] <= 0) {
      motors[i]->stop();
    }
}

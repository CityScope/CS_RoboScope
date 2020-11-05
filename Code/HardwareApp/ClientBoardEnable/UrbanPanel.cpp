#include "UrbanPanel.h"

UrbanPanel::UrbanPanel(int panelId) {
  id = panelId;

  // Set up the motors
  motorDirPins[0] = DIR_PIN_01;
  motorDirPins[1] = DIR_PIN_02;
  motorDirPins[2] = DIR_PIN_03;
  motorDirPins[3] = DIR_PIN_04;
  motorDirPins[4] = DIR_PIN_05;
  motorDirPins[5] = DIR_PIN_06;
  motorDirPins[6] = DIR_PIN_07;
  motorDirPins[7] = DIR_PIN_08;

  motorStepPins[0] = STEP_PIN_01;
  motorStepPins[1] = STEP_PIN_02;
  motorStepPins[2] = STEP_PIN_03;
  motorStepPins[3] = STEP_PIN_04;
  motorStepPins[4] = STEP_PIN_05;
  motorStepPins[5] = STEP_PIN_06;
  motorStepPins[6] = STEP_PIN_07;
  motorStepPins[7] = STEP_PIN_08;

  motorEnablePins[0]= SX1509_01_MENABLE;
  motorEnablePins[1]= SX1509_02_MENABLE;
  motorEnablePins[2]= SX1509_03_MENABLE;
  motorEnablePins[3]= SX1509_04_MENABLE;
  motorEnablePins[4]= SX1509_05_MENABLE;
  motorEnablePins[5]= SX1509_06_MENABLE;
  motorEnablePins[6]= SX1509_07_MENABLE;
  motorEnablePins[7]= SX1509_08_MENABLE;
  
  //setup SX
  Serial.println("Setting up Enable Motor Mux:");

  int muxCounter = 0;
  while (muxCounter != 3) {
    if (!sxMotor.begin(SX1509_ADDRESS_00) )
    {
      Serial.print("Failed 00 ");
      Serial.print(" " + SX1509_ADDRESS_00);
      Serial.print(" ");
      Serial.println(muxCounter);
      delay(100);
      muxCounter++;
    } else {
      Serial.println("Connected 00");
      Serial.print(" " + SX1509_ADDRESS_00);
      Serial.print(" ");
      Serial.println(muxCounter);
      break;
    }
  }

  Serial.println(" ");
  for (int i = 0; i < MOTORS_PER_PANEL; i++) {
    Serial.print("new Motor: ");
    Serial.println(i);
    motors[i] = new StepperMotor(i, GMOTOR_STEPS, motorDirPins[i], motorStepPins[i], motorEnablePins[i]);
    waitTimeMicros[i] = 0;
    motors[i]->setRPM(GRPM);
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


//init Urban Panel
void UrbanPanel::init() {
  for (int i = 0; i < MOTORS_PER_PANEL; i++) {
    //waitTimeMicros[i] = 0;
    //threads.addThread(motorThreadTimer, i);

    motors[i]->init();
    motors[i]->initMux(sxMotor);
    motors[i]->printMotorInfo();
  }

  //setup Initerface Panel
//  interfacePanel->init();

  Serial.println("Finish init Panel");
}

void UrbanPanel::update() {

  for (int i = 0 ; i < MOTORS_PER_PANEL; i++) {
    motors[i]->getNextAction();
  }
}

///
void UrbanPanel::motorTimerUpdate() {
  for (int i = 0; i < MOTORS_PER_PANEL; i++) {
    if (motors[i]->isMotorStop()) {
      motors[i]->disable(sxMotor);
      Serial.print("disable ");
      Serial.println(i);
      //Waiting lock

      if (!motors[i]->isMotorLock()) {
        bool lock = motors[i]->updateLock(millis());

        if (lock) {
          motors[i]->enable(sxMotor);
          motors[i]->startMoveForward(5);
          motors[i]->resetlimit();

          Serial.print("enable ");
          Serial.println(i);
        }
      }
    }
  }


  //lock the button and push up
  for (int i = 0 ; i < MOTORS_PER_PANEL; i++) {
    unsigned waitTimeMicros = motors[i]->getNextAction();
    //if (waitTimeMicros <= 0) {
    //  motors[i]->disable(sxMotor);
    //   Serial.print("stop motor: ");
    //   Serial.println(i);
    // }
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

///
void UrbanPanel::disableMotors() {
  for (int i = 0 ; i < MOTORS_PER_PANEL; i++) {
    motors[i]->disable(sxMotor);
  }
}

///
void UrbanPanel::enableMotors() {
  for (int i = 0 ; i < MOTORS_PER_PANEL; i++) {
    motors[i]->enable(sxMotor);
  }
}

bool UrbanPanel::isEnable(int i) {
  return motors[i]->isEnable();
}

void UrbanPanel::disableMotor(int i) {
  motors[i]->disable(sxMotor);
}

void UrbanPanel::enableMotor(int i) {
  motors[i]->enable(sxMotor);
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
    motors[i]->disable(sxMotor);
  }
}

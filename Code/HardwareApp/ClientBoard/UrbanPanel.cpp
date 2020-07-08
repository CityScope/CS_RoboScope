#include "UrbanPanel.h"


//------------------------------------------------------------------------------
//constructor
UrbanPanel::UrbanPanel(int panelId) {
  id = panelId;

  stateChanged = false;

  motorPanel = new MotorPanel(id);
  interfacePanel = new InterfacePanel(id);

  disableMotor = false;
}


//------------------------------------------------------------------------------
// returns the state of interface (sensor output, all button states, LEDs)
int UrbanPanel::getInterfaceState() {
  return 0;
}

//------------------------------------------------------------------------------
// returns the state of all the motors
int UrbanPanel::getMotorState() {
  return 0;
}

//------------------------------------------------------------------------------
// resets the state status variable
void UrbanPanel::resetState() {
  stateChanged = false;
}

//------------------------------------------------------------------------------
// returns true if the interface has been changed
bool UrbanPanel::getStateChange() {
  return stateChanged;
}

//------------------------------------------------------------------------------
// Iteratively goes through all interface push buttons to see if any are activated
// Modifies motor output as necessary
void UrbanPanel::checkInterfaceInput() {
  for (int i = 0; i < interfacePanel->getUrbanPixelCount(); i++) {
    if (interfacePanel->getInterfaceButtonState(i)) {
      moveMotorDown(i);
      stateChanged = true;
    }
  }
}


//------------------------------------------------------------------------------
void UrbanPanel::setUrbanPixelColor(int i, int r, int g, int b) {
  interfacePanel->setColorUrbanPixel(i, r, g, b);
}


//------------------------------------------------------------------------------
// TODO: Check to make sure the specified directions are the directions for the motor
// TODO: Check to see if the stepper motor stops moving up when button is pressed
// Moves the specified urban pixel up

void UrbanPanel::stopMotor(int i) {
  motorPanel->getMotor(i).stop();
}

//------------------------------------------------------------------------------
// Itreatively goes through all the motors and checks if their next action is null
// If it is then the motor is stopped.
// Meant to be placed in the void loop of the main function
void UrbanPanel::stopMotorsToPosition() {
  for (int i = 0; i < motorPanel->getNumberOfMotors(); i++) {
    unsigned long waitTimeMicro = checkMotorState(i);
    if (waitTimeMicro <= 0) {
      stopMotor(i);
    }
  }
}

void UrbanPanel::stopMotorsToLimitPosition() {
  //update motor values
  unsigned long ctime = millis();

  //if the motor is unlocked we can move it if not stop it
  for (int i = 0; i < motorPanel->getNumberOfMotors(); i++) {
    if (motorPanel->getMotor(i).isLock() == false) {
      unsigned long waitTimeMicro = checkMotorState(i);
      if (waitTimeMicro <= 0) {
        stopMotor(i);
      }

      //check if the limit switch was pressed
      if (interfacePanel->getLimitSwitchState(i) == HIGH) {
        // motorPanel->getMotor(i).resetLock();
        motorPanel->getMotor(i).stop();
        //moveMotor(i, ceil(0.1 * GMOTOR_STEPS) * -1);

        //move backwards a bit
        Serial.print(i);
        Serial.print(" ");
        Serial.println("stop motor");
      }

    }
  }

  for (int i = 0; i < motorPanel->getNumberOfMotors(); i++) {
    // motorPanel->getMotor(i).update();
  }

}

void UrbanPanel::moveMotorUp(int i) {
  if (!interfacePanel->getInterfaceButtonState(i)) {
    motorPanel->getMotor(i).moveForward();
  } else {
    motorPanel->getMotor(i).stop();
  }
}

//------------------------------------------------------------------------------
// Moves the specified urban pixel down
void UrbanPanel::moveMotorDown(int i) {
  if (!interfacePanel->getLimitSwitchState(i)) {
    motorPanel->getMotor(i).moveBackward();
  } else {
    motorPanel->getMotor(i).stop();
  }
}


//------------------------------------------------------------------------------
void UrbanPanel::moveMotor(int motorID, int motorDir, int motorStep, int motorTimeActivation, int motorEnable) {
  if (motorDir == 1) {
    moveMotorUpMicro(motorID, motorStep, motorTimeActivation, motorEnable);
  } else {
    moveMotorDownMicro(motorID, motorStep, motorTimeActivation, motorEnable);
  }
}

// TODO: RECOMMENT THE CODE IN TO CHECK FOR INTERFACE BUTTONS I.E. LIMIT SWITCHES
//------------------------------------------------------------------------------
void UrbanPanel::moveMotorUpMicro(int motorID, int motorStep, int motorTimeActivation, int motorEnable) {

  motorPanel->getMotor(motorID).startMoveForward(motorStep);
}

void UrbanPanel::moveMotor(int motorID, int motorStep) {
  motorPanel->getMotor(motorID).startMoveForwardSteps(motorStep);
}


// TODO: RECOMMENT THE CODE IN TO CHECK FOR INTERFACE BUTTONS I.E. LIMIT SWITCHES
//------------------------------------------------------------------------------
void UrbanPanel::moveMotorDownMicro(int motorID, int motorStep, int motorTimeActivation, int motorEnable) {

  motorPanel->getMotor(motorID).startMoveBackward(motorStep);
}

//------------------------------------------------------------------------------
unsigned  UrbanPanel::checkMotorState(int motorID) {
  return  motorPanel->getMotor(motorID).getNextAction();
}


//------------------------------------------------------------------------------
// Interprets the message received from the CAN bus as actions
void UrbanPanel::interpretMsg(uint8_t msg[]) {
  int motorID = int(msg[0]);
  int motorDir = int(msg[1]);
  int motorStep = int(msg[2]);
  int motorTimeActivation = int(msg[3]);
  int motorEnable = int(msg[4]);

  moveMotor(motorID, motorDir, motorStep, motorTimeActivation, motorEnable);

  // TODO: Figure out the CAN communcations for LEDs
  uint8_t interaction = msg[5];

  int motorSensor0 = msg[6];
  int motorSensor1 = msg[7];
}

//------------------------------------------------------------------------------
MotorPanel UrbanPanel::getMotorPanel() {
  return *motorPanel;
}


//------------------------------------------------------------------------------
InterfacePanel UrbanPanel::getInterfacePanel() {
  return *interfacePanel;
}

//------------------------------------------------------------------------------
// initialize motors
void UrbanPanel::setup() {
  Serial.println("Setting Interface");
  interfacePanel->init();

  Serial.println("Setting Motors");
  motorPanel->init();


}

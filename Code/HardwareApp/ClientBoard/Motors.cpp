#include "Motors.h"


Motors::Motors() {
  driver = TMC2209Stepper(&SERIAL_PORT, R_SENSE, DRIVER_ADDRESS);
}

void Motors::init() {


  upperStepLimit = 100;

  SERIAL_PORT.begin(250000);
  driver->begin();

  Serial.println("Initializing Motors...");
  for (int i = 0; i < MOTORS_PER_PANEL; i++) {
    currentStep[i] = 0;
    shaftDir[i] = true;

    enableMotor(i);
    setUpMotor(i);

    activeMotors[i] = true;
  }
}

// The function that runs in the main loop of the program. This function manual steps through each motor based on the current instruction given via CANBUS.
void Motors::motorInstructionLoop() {
  // TODO Check if the interface buttons are pressed, before one stepper tick.

  if (activeMotors[0]) digitalWrite(STEP_PIN_01, HIGH);
  if (activeMotors[1]) digitalWrite(STEP_PIN_02, HIGH);
  if (activeMotors[2]) digitalWrite(STEP_PIN_03, HIGH);
  if (activeMotors[3]) digitalWrite(STEP_PIN_04, HIGH);
  if (activeMotors[4]) digitalWrite(STEP_PIN_05, HIGH);
  if (activeMotors[5]) digitalWrite(STEP_PIN_06, HIGH);
  if (activeMotors[6]) digitalWrite(STEP_PIN_07, HIGH);
  if (activeMotors[7]) digitalWrite(STEP_PIN_08, HIGH);
  delayMicroseconds(64);

  if (activeMotors[0]) digitalWrite(STEP_PIN_01, LOW);
  if (activeMotors[1]) digitalWrite(STEP_PIN_02, LOW);
  if (activeMotors[2]) digitalWrite(STEP_PIN_03, LOW);
  if (activeMotors[3]) digitalWrite(STEP_PIN_04, LOW);
  if (activeMotors[4]) digitalWrite(STEP_PIN_05, LOW);
  if (activeMotors[5]) digitalWrite(STEP_PIN_06, LOW);
  if (activeMotors[6]) digitalWrite(STEP_PIN_07, LOW);
  if (activeMotors[7]) digitalWrite(STEP_PIN_08, LOW);
  delayMicroseconds(64);

  // Update the step counters for all motors;
  for (int i = 0; i < MOTORS_PER_PANEL; i++) {
    if (activateMotors[i]) {
      int change = (shaftDir[i]) ? 1:-1;
      currentStep[i] += change;

      // Check upper bound of pixel
      if (upperStepLimit - currentStep < MOTOR_STEP_TOLERANCE) {
        currentStep[i] = upperStepLimit;
        stopMotor(i)
      }

      // Check lower bound of pixel
      if (currentStep < MOTOR_STEP_TOLERANCE) {
        stopMotor(i)
        currentStep[i] = 0;
      }


      // Check target Position against target to know when to stop
      if (abs(currentStep[i] - stepTarget) < MOTOR_STEP_TOLERANCE)) {
        currentStep[i] = stepTarget;
        stopMotor(i)
      }

    }
  }
}

int Motors::getMotorTargetPos(int id) {
  return stepTarget;
}

void Motors::setMotorTarget(int id, int targetPos, int tolerance = MOTOR_STEP_TOLERANCE) {
  int dist = targetPos - currentStep[id];

  if (abs(dist) < tolerance) {
    stopMotor(id);
    return:
  }

  stepTarget = targetPos;

  activeMotors[id] = true; // Lets the motor move;

  if (dist < 0) {
    shaftDir[id] = false; // Going Down
  } else {
    shaftDir[id] = true; // Going Up
  }

  enableMotor(id);
  driver->shaft(shaftDir[id]);
  testConnection(0);
}

void Motors::stopMotor(int id) {
  // TODO: DISABLE MOTORS POWER
  // sx->digitalWrite(smthg, HIGH);

  activeMotors[id] = false;
}

int Motors::getMotorCurrentStep(int id) {
  return currentStep[id];
}

bool Motors::getMotorDir(int id) {
  return shaftDir[id]; // false is down, true is up
}

// Returns whether or not the current motor is moving.
bool Motors::isActive(int id) {
  return activeMotors[id];
}

int Motors::getUpperStepLimit() {
  return upperStepLimit;
}

int Motors::setUpperStepLimit(int stepLimit) {
  upperStepLimit = stepLimit;
}

void Motors::enableMotor(int id) {
  switch(id) {
    case 0:
      sx->digitalWrite(selectMotor[0], LOW);
      sx->digitalWrite(selectMotor[1], LOW);
      sx->digitalWrite(selectMotor[2], LOW);
      break;
    case 1:
      sx->digitalWrite(selectMotor[0], HIGH);
      sx->digitalWrite(selectMotor[1], LOW);
      sx->digitalWrite(selectMotor[2], LOW);
      break;
    case 2:
      sx->digitalWrite(selectMotor[0], LOW);
      sx->digitalWrite(selectMotor[1], HIGH);
      sx->digitalWrite(selectMotor[2], LOW);
      break;
    case 3:
      sx->digitalWrite(selectMotor[0], HIGH);
      sx->digitalWrite(selectMotor[1], HIGH);
      sx->digitalWrite(selectMotor[2], LOW);
      break;
    case 4:
      sx->digitalWrite(selectMotor[0], LOW);
      sx->digitalWrite(selectMotor[1], LOW);
      sx->digitalWrite(selectMotor[2], HIGH);
      break;
    case 5:
      sx->digitalWrite(selectMotor[0], HIGH);
      sx->digitalWrite(selectMotor[1], LOW);
      sx->digitalWrite(selectMotor[2], HIGH);
      break;
    case 6:
      sx->digitalWrite(selectMotor[0], LOW);
      sx->digitalWrite(selectMotor[1], HIGH);
      sx->digitalWrite(selectMotor[2], HIGH);
      break;
    case 7:
      sx->digitalWrite(selectMotor[0], HIGH);
      sx->digitalWrite(selectMotor[1], HIGH);
      sx->digitalWrite(selectMotor[2], HIGH);
      break;
  }
}

void Motors::setUpMotor(int id) {
  driver->toff(5);                 // Enables driver in software
  driver->rms_current(500);        // Set motor RMS current
  driver->microsteps(32);          // Set microsteps to 1/16th

  //driver.en_pwm_mode(true);       // Toggle stealthChop on TMC2130/2160/5130/5160
  driver->en_spreadCycle(false);   // Toggle spreadCycle on TMC2208/2209/2224
  driver->pwm_autoscale(true);     // Needed for stealthChop

  Serial.print(F("\nTesting connection... "));
  Serial.println(id);
  uint8_t result = driver->test_connection();
  delay(200);
  if (result) {
    Serial.println(F("failed!"));
    Serial.print(F("Likely cause: "));
    switch (result) {
      case 1: Serial.println(F("loose connection")); break;
      case 2: Serial.println(F("Likely cause: no power")); break;
    }
    Serial.println(F("Fix the problem and reset board."));
    // abort();
  }
  Serial.println(F("OK"));

}

void Motors::setupSXPins() {

  //MOTOR enable
  sx->pinMode(EN_PIN_01, OUTPUT);
  sx->pinMode(EN_PIN_02, OUTPUT);
  sx->pinMode(EN_PIN_03, OUTPUT);
  sx->pinMode(EN_PIN_04, OUTPUT);
  sx->pinMode(EN_PIN_05, OUTPUT);
  sx->pinMode(EN_PIN_06, OUTPUT);
  sx->pinMode(EN_PIN_07, OUTPUT);
  sx->pinMode(EN_PIN_08, OUTPUT);

  //Enables the motors
  sx->digitalWrite(EN_PIN_01, LOW);
  sx->digitalWrite(EN_PIN_02, LOW);
  sx->digitalWrite(EN_PIN_03, LOW);
  sx->digitalWrite(EN_PIN_04, LOW);
  sx->digitalWrite(EN_PIN_05, LOW);
  sx->digitalWrite(EN_PIN_06, LOW);
  sx->digitalWrite(EN_PIN_07, LOW);
  sx->digitalWrite(EN_PIN_08, LOW);

  //MUX
  sx->pinMode(selectMotor[0], OUTPUT);
  sx->pinMode(selectMotor[1], OUTPUT);
  sx->pinMode(selectMotor[2], OUTPUT);

  //LED
  sx->pinMode(selectLED[0], OUTPUT);
  sx->pinMode(selectLED[1], OUTPUT);
  sx->pinMode(selectLED[2], OUTPUT);

  //MUX
  for (int i = 0; i < 3; i++) {
    sx->digitalWrite(selectMotor[i], LOW);
    sx->digitalWrite(selectLED[i], LOW);
  }
}

void Motors::gotoMaxStep() {

}

void Motors::zeroMotors() {
  for (int i = 0; i < MOTORS_PER_PANEL; i++) {
    setMotorTarget(i, 0, MOTOR_STEP_TOLERANCE)

  }
}

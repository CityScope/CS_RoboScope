#ifndef MOTORS_H
#define MOTORS_H

#include <Arduino.h>
#include <TMCStepper.h>
#include <AccelStepper.h>
#include <SparkFunSX1509.h>
#include "BoardPins.h"


#define MOTOR_STEP_TOLERANCE 20

class Motors {
public:

  Motors();

  void motorInstructionLoop();

  void init();

  void setMotorTarget(int id, int targetPos, int tolerance=MOTOR_STEP_TOLERANCE);

  void stopMotor(int id);

  int getMotorCurrentStep(int id);

  void getMotorDir(int id);

  void isActive(int id);

  int getUpperStepLimit();

  void setUpperStepLimit(int stepLimit);

  void enableMotor(int id);
  void setUpMotor(int id);
  void setupSXPins();

  void gotoMaxStep();

  void zeroMotors();

private:
  const uint32_t steps_per_mm = 80;
  TMC2209Stepper driver;


  bool * shaftDir[MOTORS_PER_PANEL];      // The direction each motor will go. Changing the ith entry changes the direction of the ith motor
  bool * activeMotors[MOTORS_PER_PANEL];  // Whether a motor is set to move. If true, the motor should be moving, or will move in the next loop
  int * currentStep[MOTORS_PER_PANEL];    // The current step counter for each motor

  SX1509 sx;

  int upperStepLimit;
  int * stepTarget[MOTORS_PER_PANEL];

  const int selectMotor[3] = {5, 7, 9};

  const int selectLED[3] = {11, 13, 15};
  const int LEDEnable = 4;

};
#endif

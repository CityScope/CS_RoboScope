#ifndef MOTORS_H
#define MOTORS_H

#include <Arduino.h>
#include <TMCStepper.h>
#include <AccelStepper.h>
#include <SparkFunSX1509>
#include "BoardPins.h"


struct MotorPins {
  int EN_PIN;
  int DIR_PIN;
  int STEP_PIN;
};


#define MOTOR_STEP_TOLERANCE 20;

class Motors {
public:

  Motors();

  void motorInstructionLoop();

  void setMotorTarget(int id, int targetPos, int tolerance=MOTOR_STEP_TOLERANCE);

  void stopMotors(int id);

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
  constexpr uint32_t steps_per_mm = 80;

  bool * shaftDir[MOTORS_PER_PANEL];
  bool * activeMotors[MOTORS_PER_PANEL];
  int * stepCounter[MOTORS_PER_PANEL];

  SX1509 sx;

  int upperStepLimit;
  int stepTarget = 0;

  const int selectMotor[3] = {5, 7, 9};

  const int selectLED[3] = {11, 13, 15};
  const int LEDEnable = 4;

};
#endif

#include <Arduino.h>
#include <SparkFunSX1509.h>
#include "Pins.h"
#include <TMCStepper.h>

/*
   Control for TMC2209 stepper motor driver
*/

enum Seq {
  UP = 1,
  STOP = 2,
  DOWN = 3,
  DIR = 4,
  FINAL = 5
};

class StepperMotor {
public:


  //direction pin
  uint8_t DIR_PIN;

  //step pin
  uint8_t STEP_PIN;

  //activators
  uint8_t ENABLE_PIN;

  // Motor steps per revolution. Most steppers are 200 steps or 1.8 degrees/step
  int motorSteps;
  int rpm;

  //driver id;
  int id;

  //activate Motor
  boolean enableMotor;

  //dir motor
  boolean dirMotor;
  bool dir;

  // motorStatus 1 connected correctly 0 failed to connect
  uint8_t motorStatus;

  int targetPos;
  int currPos;
  bool targetReached;
  bool updatePos;

  int stepsCounter;

  int minPos;
  int maxPos;

  bool calibration;
  bool positionSaved;

  bool enableSequence;
  Seq seqRoutine;
  int stopTime;
  int maxStopTime;

  //constructor
  StepperMotor(int m_id, int steps, uint8_t dir_pin, uint8_t step_pin, uint8_t enable_pin) {
    id = m_id;
    motorSteps = steps;

    //pins
    DIR_PIN = dir_pin;
    STEP_PIN = step_pin;
    ENABLE_PIN = enable_pin;

    dirMotor = false;

    enableSequence = false;
    seqRoutine = UP;

    stepsCounter = 0;
    targetPos = 0;
    targetReached = false;
    updatePos = true;

    positionSaved = false;

    minPos = 0;
    maxPos = 5800;
    stopTime = 4000;
    

    calibration = false;
    enableMotor = false;
  }

  //delete pointer memory
  ~StepperMotor() {
  }

  void toggleSequence() {
    enableSequence = !enableSequence;
  }
  bool isSequence() {
    return enableSequence;
  }

  void setTargetPos(uint8_t target) {
    targetPos = map(target, 0, 255, minPos, maxPos);
    targetReached = false;
    // stopTime = stopTime + (maxPos - targetPos);
    // seqRoutine = UP;
    // currPos = 0;
  }

  void startSequence(TMC2209Stepper* driver, int target){
    if (currPos == target){
      targetReached = true;
    } else {
      targetReached = false;
      targetPos = target;
      enableSequence = true;
      // if (currPos < targetPos){
      //   dirMotor = true;
      //   driver->shaft(true);
      // } else if (currPos > targetPos){
      //   dirMotor = false;
      //   driver->shaft(false);
      // }
    }
  }

  //calibration process
  void toggleCalibration() {
    calibration = !calibration;
  }

  int getCurrPos() {
    return currPos;
  }
  int getTargetPos() {
    return targetPos;
  }

  // void goToTarget(TMC2209Stepper* driver, SX1509* sxm0, SX1509* sxm1){
  //   if (enableSequence){
  //     if (currPos < targetPos){
  //       currPos
  //     }
  //   }
  // }

  void incPos(TMC2209Stepper* driver, SX1509* sxm0, SX1509* sxm1) {
    if (enableSequence){
      if (currPos == targetPos){
        targetReached = true;
        enableSequence = false;
        enableMotorSX(id, sxm0);
        stop(sxm1, driver);
      } else if (currPos < targetPos) {
        enableMotorSX(id, sxm0);
        start(sxm1, driver);
        shaftOff(driver);
        currPos++;
      } else if (currPos > targetPos) {
        enableMotorSX(id, sxm0);
        start(sxm1, driver);
        shaftOn(driver);
        currPos--;
      }
    } else if (enableMotor) {
      if (dirMotor){
        currPos--;
      } else {
        currPos++;
      }
    }
  }

// Demo function
  // void incPos(TMC2209Stepper* driver, SX1509* sxm0, SX1509* sxm1) {
  //   if (enableSequence) {
  //     if (seqRoutine == UP) {
  //       if (currPos <= targetPos) {
  //         currPos++;
  //       } else {
  //         seqRoutine = STOP;
  //         currPos = 0;
  //         enableMotorSX(id, sxm0);
  //         stop(sxm1, driver);

  //         Serial.print(id);
  //         Serial.print(" ");
  //         Serial.println("GO TO STOP");
  //       }

  //     } else if (seqRoutine == STOP) {
  //       if (currPos <= stopTime) {
  //         currPos++;
  //       } else {
  //         currPos = 0;
  //         enableMotorSX(id, sxm0);
  //         start(sxm1, driver);
  //         shaftOn(driver);
  //         motorOn();

  //         seqRoutine = DIR;

  //         delay(2);
  //         Serial.print(id);
  //         Serial.print(" ");
  //         Serial.println("CHANGE DIR");
  //       }
  //     } else if (seqRoutine == DIR) {
  //      // enableMotorSX(id, sxm0);



  //       seqRoutine = DOWN;

  //       delay(2);
  //       Serial.print(id);
  //       Serial.print(" ");
  //       Serial.println("CHANGE DOWN");
  //     } else if (seqRoutine == DOWN) {
  //       if (currPos <= targetPos) {
  //         currPos++;
  //       } else {
  //         enableMotorSX(id, sxm0);
  //         driver->shaft(false);
  //         stop(sxm1, driver);
  //         currPos = 0;
  //         enableSequence = false;
  //         seqRoutine = FINAL;
  //         Serial.print(id);
  //         Serial.print(" ");
  //         Serial.println("FINAL");
  //       }
  //     }
  //   }
  // }

  void incCalibrationSteps() {
    stepsCounter++;
  }

  int getNumCaliSteps() {
    return stepsCounter;
  }

  void resetSteps() {
    stepsCounter = 0;
  }

  bool isCalibrating() {
    return calibration;
  }

  //init motor pins
  void initPins(SX1509* sxm, bool asx) {
    if (asx) {
      sxm->pinMode(ENABLE_PIN, OUTPUT);
      sxm->digitalWrite(ENABLE_PIN, LOW);
    } else {
      Serial.print("Motor Enable SX not loadead  ");
      Serial.print(id);
      Serial.println();
    }
    pinMode(STEP_PIN, OUTPUT);
    // pinMode(DIR_PIN, OUTPUT);

    //digitalWrite(DIR_PIN, LOW);
    digitalWrite(STEP_PIN, LOW);
  }

  //init pins and driver
  void initMotor(TMC2209Stepper* driver) {

    Serial.print(" Setup Driver: ");
    Serial.println(id);

    //disable motors
    driver->toff(4);  // Enables driver in software

    //turn default down
    //true -> Motor down, false  -> Motor up
    //driver->shaft(true);
    driver->rms_current(RMS_CURRENT);  // Set motor RMS current
    driver->microsteps(MICRO_STEPS);   //128         // Set microsteps to 1/16th

    //driver.en_pwm_mode(true);       // Toggle stealthChop on TMC2130/2160/5130/5160
    driver->en_spreadCycle(false);  // Toggle spreadCycle on TMC2208/2209/2224
    driver->pwm_autoscale(true);    // Needed for stealthChop

    Serial.print(F("\nTesting connection... "));
    Serial.print(id);
    uint8_t result = driver->test_connection();
    motorStatus = result;
    delay(100);
    if (result) {
      Serial.println(F("failed!"));
      Serial.print(F("Likely cause: "));
      switch (result) {
        case 1: Serial.println(F("loose connection")); break;
        case 2: Serial.println(F("Likely cause: no power")); break;
      }
      Serial.println(F("Fix the problem and reset board."));
      motorStatus = 0;
    }
    Serial.println(F("  OK"));
    ///

    //start with the motor off
    //enableMotor = false;
  }

  // down
  void shaftOn(TMC2209Stepper* driver) {
    dirMotor = true;
    driver->shaft(true);
  }
  // up
  void shaftOff(TMC2209Stepper* driver) {
    dirMotor = false;
    driver->shaft(false);
  }

  void toggleShaft(TMC2209Stepper* driver) {
    dirMotor = !dirMotor;
    driver->shaft(dirMotor);
  }

  //Simple start and end Motor contr
  void motorBegin() {
    if (enableMotor) {
      digitalWrite(STEP_PIN, HIGH);
    }
  }
  void motorEnd() {
    if (enableMotor) {
      digitalWrite(STEP_PIN, LOW);
    }
  }

  void moveForward() {
    //motor->move(-motorSteps);
  }

  void startMoveForward(int steps) {
    // motor->startMove(steps * -motorSteps);
  }

  void startMoveBackward(int steps) {
    // motor->startMove(steps * motorSteps);
  }

  //unsigned getNextAction() {
  //  return motor->nextAction();
  //}

  void start(SX1509* sxm, TMC2209Stepper* driver) {
    driver->toff(4);
    sxm->digitalWrite(ENABLE_PIN, HIGH);

    //enableMotor = true;
  }

  void stop(SX1509* sxm, TMC2209Stepper* driver) {
    driver->toff(0);  //5
    sxm->digitalWrite(ENABLE_PIN, LOW);
    enableMotor = false;
  }

  bool isEnable() {
    return enableMotor;
  }

  void motorOn() {
    enableMotor = true;
  }

  void motorOff() {
    enableMotor = false;
  }

  void toggleMotor() {
    enableMotor = !enableMotor;
  }


  void testConnection(TMC2209Stepper* driver) {
    motorStatus = driver->test_connection();
    Serial.print(F("Motor: "));
    Serial.print(id);
    Serial.print(F(" "));
    Serial.println(motorStatus);
  }


  //print out information
  void printMotorInfo() {
    Serial.print("MOTOR id: ");
    Serial.println(id);
    Serial.print("MOTOR Steps: ");
    Serial.println(motorSteps);
    Serial.print("DIR pin: ");
    Serial.println(DIR_PIN);
    Serial.print("STEP PIN: ");
    Serial.println(STEP_PIN);
    Serial.print("ENABLE PIN: ");
    Serial.println(ENABLE_PIN);
  }

  void enableMotorSX(int id, SX1509* sxm) {
    int selectMotor[3] = { 5, 7, 9 };
    if (id == 0) {
      sxm->digitalWrite(selectMotor[0], LOW);
      sxm->digitalWrite(selectMotor[1], LOW);
      sxm->digitalWrite(selectMotor[2], LOW);
    } else if (id == 1) {
      sxm->digitalWrite(selectMotor[0], HIGH);
      sxm->digitalWrite(selectMotor[1], LOW);
      sxm->digitalWrite(selectMotor[2], LOW);
    } else if (id == 2) {
      sxm->digitalWrite(selectMotor[0], LOW);
      sxm->digitalWrite(selectMotor[1], HIGH);
      sxm->digitalWrite(selectMotor[2], LOW);
    } else if (id == 3) {
      sxm->digitalWrite(selectMotor[0], HIGH);
      sxm->digitalWrite(selectMotor[1], HIGH);
      sxm->digitalWrite(selectMotor[2], LOW);
    } else if (id == 4) {
      sxm->digitalWrite(selectMotor[0], LOW);
      sxm->digitalWrite(selectMotor[1], LOW);
      sxm->digitalWrite(selectMotor[2], HIGH);
    } else if (id == 5) {
      sxm->digitalWrite(selectMotor[0], HIGH);
      sxm->digitalWrite(selectMotor[1], LOW);
      sxm->digitalWrite(selectMotor[2], HIGH);
    } else if (id == 6) {
      sxm->digitalWrite(selectMotor[0], LOW);
      sxm->digitalWrite(selectMotor[1], HIGH);
      sxm->digitalWrite(selectMotor[2], HIGH);
    } else if (id == 7) {
      sxm->digitalWrite(selectMotor[0], HIGH);
      sxm->digitalWrite(selectMotor[1], HIGH);
      sxm->digitalWrite(selectMotor[2], HIGH);
    }
  }
};

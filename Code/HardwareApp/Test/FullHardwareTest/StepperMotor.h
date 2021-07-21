#include <Arduino.h>
#include <SparkFunSX1509.h>
#include "Pins.h"
#include <TMCStepper.h>

/*
   Control for TMC2209 stepper motor driver
*/
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

    // motorStatus 1 connected correctly 0 failed to connect
    uint8_t motorStatus;

    //constructor
    StepperMotor(int m_id, int steps, uint8_t dir_pin, uint8_t step_pin, uint8_t enable_pin) {
      id  = m_id;
      motorSteps = steps;

      //pins
      DIR_PIN   = dir_pin;
      STEP_PIN  = step_pin;
      ENABLE_PIN = enable_pin;

      dirMotor =  false;
    }

    //delete pointer memory
    ~StepperMotor() {

    }

    //init motor pins
    void initPins(SX1509 * sxm) {
      sxm->pinMode(ENABLE_PIN, OUTPUT);
      sxm->digitalWrite(ENABLE_PIN, LOW);

      pinMode(STEP_PIN, OUTPUT);
      digitalWrite(STEP_PIN, LOW);

      pinMode(DIR_PIN, OUTPUT);
      digitalWrite(DIR_PIN, LOW);
    }

    //init pins and driver
    void initMotor(TMC2209Stepper  * driver) {

      Serial.print(" Setup Driver: ");
      Serial.println(id);

      driver->begin();
      driver->toff(4);                 // Enables driver in software
      driver->rms_current(RMS_CURRENT);        // Set motor RMS current
      driver->microsteps(motorSteps); //128         // Set microsteps to 1/16th

      //driver.en_pwm_mode(true);       // Toggle stealthChop on TMC2130/2160/5130/5160
      driver->en_spreadCycle(false);   // Toggle spreadCycle on TMC2208/2209/2224
      driver->pwm_autoscale(true);     // Needed for stealthChop



      Serial.print(F("\nTesting connection... "));
      Serial.println(id);
      uint8_t result = driver->test_connection();
      motorStatus  = result;
      delay(200);
      if (result) {
        Serial.println(F("failed!"));
        Serial.print(F("Likely cause: "));
        switch (result) {
          case 1: Serial.println(F("loose connection")); break;
          case 2: Serial.println(F("Likely cause: no power")); break;
        }
        Serial.println(F("Fix the problem and reset board."));
        motorStatus  = 0;
      }
      Serial.println(F("OK"));
      ///

      //start with the motor off
      enableMotor = false;
      driver->shaft(false);
    }

    void shaftOn(TMC2209Stepper  * driver) {
      driver->shaft(true);
    }

    void shaftOff(TMC2209Stepper  * driver) {
      driver->shaft(false);
    }

    void toggleShaft(TMC2209Stepper  * driver) {
      dirMotor = !dirMotor;
      driver->shaft(dirMotor);
    }

    //Simple start and end Motor contr
    void motorStart(SX1509 * sxm) {
      if (enableMotor) {
        sxm->digitalWrite(STEP_PIN, HIGH);
      }
    }
    void motorEnd(SX1509 * sxm) {
      if (enableMotor) {
        sxm->digitalWrite(STEP_PIN, HIGH);
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

    void start(SX1509 * sxm) {
      sxm->digitalWrite(ENABLE_PIN, HIGH);
      enableMotor = true;
    }

    void stop(SX1509 * sxm) {
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


    void testConnection(TMC2209Stepper  * driver) {
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

};

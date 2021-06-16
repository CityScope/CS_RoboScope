#include <Arduino.h>
#include "DRV8880.h"
#include <SparkFunSX1509.h>
#include "Pins.h"
#include <TMCStepper.h>

/*
   Control for DRV8834 stepper motor driver
*/
class Stepper {
  public:

    //direction pin
    int DIR_PIN;

    //step pin
    int STEP_PIN;

    //activators
    int ENABLE_PIN;

    // Motor steps per revolution. Most steppers are 200 steps or 1.8 degrees/step
    int motorSteps;
    int rpm;

    //driver id;
    int id;

    //DRV8880 class
    TMC2209Stepper  * motor;

    int enableMotor;

    // motorStatus 1 connected correctly 0 failed to connect
    int motorStatus;

    //constructor
    Stepper(int m_id, int steps, int dir_pin, int step_pin, int enable_pin) {
      id  = m_id;
      motorSteps = steps;

      //pins
      DIR_PIN   = dir_pin;
      STEP_PIN  = step_pin;
      ENABLE_PIN = enable_pin;

      //init motor driver
      motor = new TMC2209Stepper(&SERIAL_PORT, R_SENSE, DRIVER_ADDRESS);
    }

    //init mux
    void init(SX1509 & sxm) {

      //enabler
      sxm.pinMode(ENABLE_PIN, OUTPUT);
      sxm.digitalWrite(ENABLE_PIN, LOW);

      //sleep
      Serial.print("Setup Motor ");
      Serial.print(id);
      Serial.println("...");

      motor->toff(4);                 // Enables driver in software
      motor->rms_current(RMS_CURRENT);        // Set motor RMS current
      motor->microsteps(motorSteps); //128         // Set microsteps to 1/16th

      //driver.en_pwm_mode(true);       // Toggle stealthChop on TMC2130/2160/5130/5160
      motor->en_spreadCycle(false);   // Toggle spreadCycle on TMC2208/2209/2224
      motor->pwm_autoscale(true);     // Needed for stealthChop

      Serial.print(F("\nTesting connection... "));
      Serial.println(id);
      uint8_t result = motor->test_connection();
      motorStatus = 1;
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
        // abort();
      }
      Serial.println(F("OK"));
      ///
      
      enableMotor = true;
    }


    //delete pointer memory
    ~Stepper() {
      //delete motor;
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

    void start(SX1509 & sxm) {
      sxm.digitalWrite(ENABLE_PIN, HIGH);
      enableMotor = true;
    }

    void stop(SX1509 & sxm) {
      sxm.digitalWrite(ENABLE_PIN, LOW);
      enableMotor = false;
    }

    bool isEnable() {
      return enableMotor;
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

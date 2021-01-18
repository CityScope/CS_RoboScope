#include <Arduino.h>
#include "DRV8880.h"
#include <SparkFunSX1509.h>
#include "Pins.h"

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

    //Microstepping control pins
    int M0_PIN;
    int M1_PIN;

    // Motor steps per revolution. Most steppers are 200 steps or 1.8 degrees/step
    int motorSteps;
    int rpm;

    //driver id;
    int id;

    //DRV8880 class
    DRV8880  * motor;

    int enableMotor;



    //constructor
    Stepper(int m_id, int steps, int dir_pin, int step_pin, int enable_pin, int m0_pin, int m1_pin) {
      id  = m_id;
      motorSteps = steps;

      //pins
      DIR_PIN   = dir_pin;
      STEP_PIN  = step_pin;
      ENABLE_PIN = enable_pin;
      M0_PIN = m0_pin;
      M1_PIN = m1_pin;

      //init motor driver
      motor = new DRV8880(motorSteps, DIR_PIN, STEP_PIN);//, ENABLE_PIN, M0_PIN, M1_PIN);
    }

    //init mux
    void initMux(SX1509 & sxm) {

      //enabler
      sxm.pinMode(ENABLE_PIN, OUTPUT);
      sxm.digitalWrite(ENABLE_PIN, LOW);

      //sleep
      // sxm.pinMode(SLEEP_PIN, OUTPUT);
      //m
      // sxm.digitalWrite(SLEEP_PIN, HIGH); //sleep on

      enableMotor = true;
    }

    void setRPM(int RPM) {
      rpm = RPM;
    }

    //delete pointer memory
    ~Stepper() {
      //delete motor;
    }

    void init() {
      motor->begin(rpm);

      // if using enable/disable on ENABLE pin (active LOW) instead of SLEEP uncomment next line

      motor->setEnableActiveState(LOW);

      motor->enable();

      // motor->setMicrostep(16);
    }

    void moveForward() {
      motor->move(-motorSteps);
      // motor->rotate(360);
      //      motor->runToPosition();
    }

    void startMoveForward(int steps) {
      motor->startMove(steps * -motorSteps);
    }

    void startMoveBackward(int steps) {
      motor->startMove(steps * motorSteps);
    }

    unsigned getNextAction() {
      return motor->nextAction();
    }

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

    void sleepOn(SX1509 & sxm) {
     // sxm.digitalWrite(SLEEP_PIN, LOW);

    }

    void sleepOff(SX1509 & sxm) {
     // sxm.digitalWrite(SLEEP_PIN, HIGH);
    }



    void moveBackward() {
      motor->move(motorSteps);
      //  motor->rotate(-360);
      // motor->runToPosition();
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
      Serial.print("M0 PIN: ");
      Serial.println(M0_PIN);
      Serial.print("M1 PIN: ");
      Serial.println(M1_PIN);
    }

};

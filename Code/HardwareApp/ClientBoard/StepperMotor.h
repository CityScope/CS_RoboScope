/*
   Control for DRV8880 stepper motor driver
*/
#ifndef STEPPER_MOTOR_H
#define STEPPER_MOTOR_H

#include <Arduino.h>
#include "DRV8880.h"
#include "Timer.h"


//---------------------------------------------------------------------------------
class StepperMotor {
  public:

    //constructor
    StepperMotor(int m_id, int steps, int dir_pin, int step_pin, int sleep_pin, int m0_pin, int m1_pin) {
      id  = m_id;
      motorSteps = steps;

      //update the pins
      DIR_PIN   = dir_pin;
      STEP_PIN  = step_pin;
      ENABLE_PIN = sleep_pin;
      M0_PIN = m0_pin;
      M1_PIN = m1_pin;

      //enable the motor
      //pinMode(ENABLE_PIN, OUTPUT);
      //digitalWrite(ENABLE_PIN, HIGH);

      //limit timer
      mLimitTimer = new Timer(500);

      //lock the state of the motor
      lock = false;

      //init motor driver
      motor = new DRV8880(motorSteps, DIR_PIN, STEP_PIN);// ENABLE_PIN, M0_PIN, M1_PIN);

    }

    void setRPM(int RPM) {
      rpm = RPM;
    }

    //delete pointer memory
    ~StepperMotor() {
      //delete motor;
    }

    //-----------------------------------------------------------
    void init() {
      motor->begin(rpm);

      // if using enable/disable on ENABLE pin (active LOW) instead of SLEEP uncomment next line
      //  motor->setEnableActiveState(LOW);
      motor->enable();
    }


    //-----------------------------------------------------------
    void stop() {
      motor->stop();
    }

    //-----------------------------------------------------------
    void startMoveForward(int steps) {
      motor->startMove( -1 * (steps * motorSteps) );
    }

    //-----------------------------------------------------------
    void startMoveBackward(int steps) {
      motor->startMove(steps * motorSteps);
    }

    //-----------------------------------------------------------
    void startMoveForwardSteps(int steps) {
      motor->startMove( -1 * steps);
    }

    //-----------------------------------------------------------
    void startMoveBackwardSteps(int steps) {
      motor->startMove(steps);
    }

    //-----------------------------------------------------------
    unsigned  getNextAction() {
      return motor->nextAction();
    }

    //-----------------------------------------------------------
    void moveForward() {
      motor->setMicrostep(1);
      motor->move(-motorSteps);
    }

    //-----------------------------------------------------------
    void moveBackward() {
      motor->setMicrostep(1);
      motor->move(motorSteps);
    }

    //-----------------------------------------------------------
    void moveForwardMicro(int step) {
      motor->setMicrostep(step);
      motor->move(step * -motorSteps);
    }

    /*
       Logic high to enable device outputs and internal indexer
       Enable driver input logic low to disable; internal pulldown
    */
    void enable() {
     // digitalWrite(ENABLE_PIN, HIGH);
    }

    void disable() {
    //  digitalWrite(ENABLE_PIN, LOW);
    }

    //update motor states
    void update(){
      mLimitTimer->update(millis());

      if(!mLimitTimer->isDone()){
        lock = true;
        Serial.println("lock");
      }else{
        lock = false;
      }
    }

    void changeState(){
      
    }

    void resetLock(){
      mLimitTimer->reset();
      lock = true;
    }

    boolean isLock(){
      return lock;
    }
    
    /*


    */
    void sleep() {


    }


    //-----------------------------------------------------------
    void moveBackwardMicro(int step) {
      motor->setMicrostep(step);
      motor->move(-step * motorSteps);
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
      Serial.println(DIR_PIN);
      Serial.print("SLEEP PIN: ");
      Serial.println(ENABLE_PIN);
      Serial.print("M0 PIN: ");
      Serial.println(M0_PIN);
      Serial.print("M1 PIN: ");
      Serial.println(M1_PIN);
    }

  private:

    //direction pin
    int DIR_PIN;

    //step pin
    int STEP_PIN;

    //eanble pin
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
    Timer    * mLimitTimer;

    boolean   lock;
};

#endif

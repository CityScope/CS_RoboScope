/*
   Interface Panel Code

*/
#ifndef INTERFACE_PANEL_H
#define INTERFACE_PANEL_H

#include <Arduino.h>
#include "BoardPins.h"
#include <Wire.h> // Include the I2C library (required)
#include <SparkFunSX1509.h>

// default
//const byte SX1509_ADDRESS_00 = 0x3E;

//
//const byte SX1509_ADDRESS_00 = 0x3F;

class InterfacePanel {
  public:

    // constructor
    InterfacePanel();

    int getUrbanPixelCount();

    void init();


    int getPushCurrentState(int i);
    int getLimitCurrentState(int i);

    //interrupt limit
    static  void limitswitch();

    //print limit state
    void updateLimitState();

    bool getLimitSwitchState(int i);
    bool getLimitState(int i);

    void resetLimitSwitch(int i) {
      limitSwithStatePrev[i] = false;
      limitSwithState[i] = false;
    }


    ////
    bool getPushSwitchState(int i);
    bool getPushState(int i);

    void updatePushState();

    void resetPushSwitch(int i) {
      pushSwithStatePrev[i] = false;
      pushSwithState[i] = false;
    }

  private:

    static  volatile  bool limitSwitchPressed;

    int id;

    // interface button pins
    int pushPin[MOTORS_PER_PANEL];

    // limit switch pin high
    int limitSwithPin[MOTORS_PER_PANEL];

    bool limitSwithStatePrev[MOTORS_PER_PANEL];
    bool limitSwithState[MOTORS_PER_PANEL];

    bool pushSwithStatePrev[MOTORS_PER_PANEL];
    bool pushSwithState[MOTORS_PER_PANEL];

    // SX1509 I2C address (set by ADDR1 and ADDR0 (00 by default):
    // SX1509 I2C address
    SX1509 sx00; // Create an SX1509 object to be used throughout
};

#endif

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
const byte SX1509_ADDRESS_00 = 0x3F;

class InterfacePanel {
  public:

    // constructor
    InterfacePanel();

    int getUrbanPixelCount();

    void init();

    int getPushCurrentState(int i);
    int getLimitCurrentState(int i);

    bool getLimitSwitchState(int i);
    bool getLimitState(int i);

    bool getPushSwitchState(int i);
    bool getPushState(int i);

    void updateLimitState();
    void updatePushState();

    void resetLimitSwitch(int i);
    void resetPushSwitch(int i);


  private:

    static  volatile  bool limitSwitchPressed;

    int id;

    // interface button pins
    int pushPin[MOTORS_PER_PANEL];

    // limit switch pin high
    int limitSwitchPin[MOTORS_PER_PANEL];

    bool limitSwitchStatePrev[MOTORS_PER_PANEL];
    bool limitSwitchState[MOTORS_PER_PANEL];

    bool pushSwitchStatePrev[MOTORS_PER_PANEL];
    bool pushSwitchState[MOTORS_PER_PANEL];

    // SX1509 I2C address (set by ADDR1 and ADDR0 (00 by default):
    // SX1509 I2C address
    SX1509 sx00; // Create an SX1509 object to be used throughout
};

#endif

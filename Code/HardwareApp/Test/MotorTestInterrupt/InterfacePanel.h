/*
   Interface Panel Code

*/
#ifndef INTERFACE_PANEL_H
#define INTERFACE_PANEL_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "BoardPins.h"
#include <Wire.h> // Include the I2C library (required)
#include <SparkFunSX1509.h>
#include "Timer.h"

// default
//const byte SX1509_ADDRESS_00 = 0x3E;

//
const byte SX1509_ADDRESS_00 = 0x3F;

class InterfacePanel {
  public:

    // constructor
    InterfacePanel(int pId);

    int getUrbanPixelCount();

    void init();

    void setColorAll(int r, int g, int b);

    // Sets the color on a per urban pixel basis
    void setColorUrbanPixel(int urbanPixelId, int r, int g, int b);

    //color wheel
    uint32_t Wheel(byte WheelPos);
    void rainbow(unsigned long time);
    void changeRandColor();

    // Sets the color on a sub urban pixel basis
    // Each urban pixel may be made up of multiple neopixels, this function gives control over them individually
    void setColorSubUrbanPixel(int urbanPixelId, int subPixel, int r, int g, int b);

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

    //Timer for colors
    Timer * colorTimer;

    int id;

    // interface button pins
    int pushPin[MOTORS_PER_PANEL];

    // limit switch pin high
    int limitSwithPin[MOTORS_PER_PANEL];

    bool limitSwithStatePrev[MOTORS_PER_PANEL];
    bool limitSwithState[MOTORS_PER_PANEL];

    bool pushSwithStatePrev[MOTORS_PER_PANEL];
    bool pushSwithState[MOTORS_PER_PANEL];

    Adafruit_NeoPixel * neopixels;

    // SX1509 I2C address (set by ADDR1 and ADDR0 (00 by default):
    // SX1509 I2C address
    SX1509 sx00; // Create an SX1509 object to be used throughout
};

#endif

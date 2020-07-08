/*
   Interface Panel Code

*/
#ifndef INTERFACE_PANEL_H
#define INTERFACE_PANEL_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "InterfaceDefines.h"
#include "BoardPins.h"
#include <Wire.h> // Include the I2C library (required)
#include <SparkFunSX1509.h>
#include "Timer.h"

const byte SX1509_ADDRESS_00 = 0x3E;

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

    // Sets the color on a sub urban pixel basis
    // Each urban pixel may be made up of multiple neopixels, this function gives control over them individually
    void setColorSubUrbanPixel(int urbanPixelId, int subPixel, int r, int g, int b);

    int getInterfaceButtonState(int i);

    int getLimitSwitchState(int i);

    //interrupt limit
    static  void limitswitch();

    //print limit state
    void printLimitState();


  private:

    static  volatile  bool limitSwitchPressed;

    //Timer for colors 
    Timer * colorTimer;
    
    int id;

    // interface button pins
    int pushDownPin[MOTORS_PER_PANEL];

    // limit switch pin high
    int limitSwithPin[MOTORS_PER_PANEL];

    Adafruit_NeoPixel * neopixels;

    // SX1509 I2C address (set by ADDR1 and ADDR0 (00 by default):
    // SX1509 I2C address
    SX1509 sx00; // Create an SX1509 object to be used throughout
};

#endif

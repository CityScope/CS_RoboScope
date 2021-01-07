#ifndef URBAN_PANEL_H
#define URBAN_PANEL_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "BoardPins.h"
#include "Stepper.h"
#include "InterfacePanel.h"
#include <SparkFunSX1509.h>

class UrbanPanel {
  public:
    UrbanPanel(int panelId);

    void init();


    int getPushState(int i);

    int getLimitState(int i);

    void resetMotorRange();

    void zeroMotors();

    void maxMotors();

    void motorTimerUpdate();

    void movePixelUp(int i, int steps);

    void movePixelDown(int i, int steps);

    void setPixelPosition(int i, float pos);

    void setPixelColor(int i, int r, int g, int b);

    void pixelLoop(int i);

    void limitswitch();

    void stopMotor(int i);

    void setTrq1Mode(int i);

    void setTrq2Mode(int i);


  private:
    int id;

    SX1509 motorSX;
    int trq1Mode = 1;
    int trq2Mode = 0;

    Stepper * motors[MOTORS_PER_PANEL];
    volatile unsigned * waitTimeMicros[MOTORS_PER_PANEL];


    InterfacePanel * interfacePanel;
    volatile boolean limitActivated = false;

    Adafruit_NeoPixel * pixels[MOTORS_PER_PANEL];
    int NUMPIXELS = 4; //number of neo pixels per pixel

};
#endif

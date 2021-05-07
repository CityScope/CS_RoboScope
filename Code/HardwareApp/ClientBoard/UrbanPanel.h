#ifndef URBAN_PANEL_H
#define URBAN_PANEL_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "BoardPins.h"
#include "Motors.h"
#include "InterfacePanel.h"
#include <SparkFunSX1509.h>

class UrbanPanel {
  public:
    UrbanPanel(int panelId);

    void init();

    void zeroAllMotors();

    void initializeMotorHeightLimit(int callibrationMotorsIds[], int callibrationMotorCount);

    void setPixelColor(int i, int r, int g, int b);

    int getPushState(int i);

    int getLimitState(int i);

    void motorTimerUpdate();

    void limitswitch();


    void setPixelPosition(int i, float pos, int tolerance=MOTOR_STEP_TOLERANCE);



  private:
    int id;

    Motors * motors;
    volatile unsigned * waitTimeMicros[MOTORS_PER_PANEL];


    InterfacePanel * interfacePanel;
    volatile boolean limitActivated = false;

    Adafruit_NeoPixel * pixels[MOTORS_PER_PANEL];
    int NUMPIXELS = 4; //number of neo pixels per pixel

};
#endif

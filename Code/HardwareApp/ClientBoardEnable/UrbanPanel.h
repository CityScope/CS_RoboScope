#ifndef URBAN_PANEL_H
#define URBAN_PANEL_H

#include <Arduino.h>
#include "BoardPins.h"
#include "StepperMotor.h"
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
    void update();

    void movePixelUp(int i, int steps);

    void movePixelDown(int i, int steps);

    void setPixelPosition(int i, float pos);

    void pixelLoop(int i);

    void limitswitch();

    void disableMotors();
    void enableMotors();

    bool isEnable(int i);

    void disableMotor(int i);
    void enableMotor(int i);


  private:
    int id;
    StepperMotor * motors[8];
    volatile unsigned * waitTimeMicros[8];
    InterfacePanel * interfacePanel;
    volatile boolean limitActivated = false;

    int motorDirPins[8];
    int motorStepPins[8];
    int motorEnablePins[8];

    SX1509 sxMotor; // Create an SX1509 object to be used throughout
};
#endif

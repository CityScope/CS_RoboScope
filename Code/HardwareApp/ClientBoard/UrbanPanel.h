#ifndef URBAN_PANEL_H
#define URBAN_PANEL_H

#include <Arduino.h>
#include "BoardPins.h"
#include "StepperMotor.h"
#include "InterfacePanel.h"

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

    void pixelLoop(int i);

    void limitswitch();


  private:
    int id;
    StepperMotor * motors[MOTORS_PER_PANEL];
    volatile unsigned * waitTimeMicros[MOTORS_PER_PANEL];
    InterfacePanel * interfacePanel;
    volatile boolean limitActivated = false;
    volatile boolean limitActivatedPrev = false;

};
#endif

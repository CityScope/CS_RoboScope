/*
   The class that controls the overall functions of the Urban Panel (a panel made up of Urban Pixels)
   Can handle the output and inputs of each urban pixel in the panel
   The individual pixel unit that controls the stepper, the lights, and handles the inputs of the respective pixel
*/
#ifndef URBAN_PANEL_H
#define URBAN_PANEL_H

#include <Arduino.h>

#include "MotorPanel.h"
#include "InterfacePanel.h"

class UrbanPanel {
  public:
    //constructor
    UrbanPanel(int panelId);

    // returns the state of interface (sensor output, all button states, LEDs)
    int getInterfaceState();

    // returns the state of all the motors
    int getMotorState();

    // resets the state status variable
    void resetState();

    // returns true if the interface has been changed
    bool getStateChange();

    // Iteratively goes through all interface push buttons to see if any are activated
    // Modifies motor output as necessary
    void checkInterfaceInput();

    void stopMotorsToLimitPosition();

    void setUrbanPixelColor(int i, int r, int g, int b);

    // TODO: Check to make sure the specified directions are the directions for the motor
    // TODO: Check to see if the stepper motor stops moving up when button is pressed
    // Moves the specified urban pixel up
    void moveMotorUp(int i);

    unsigned checkMotorState(int motorID);

    void stopMotor(int i);

    // Moves the specified urban pixel down
    void moveMotorDown(int i);

    void stopMotorsToPosition();

    void moveMotor(int motorID, int motorDir, int motorStep, int motorTimeActivation, int motorEnable);

    void moveMotorUpMicro(int motorID, int motorStep, int motorTimeActivation, int motorEnable);

    void moveMotorDownMicro(int motorID, int motorStep, int motorTimeActivation, int motorEnable);


    void moveMotor(int motorID, int motorStep);
    

    void interpretMsg(uint8_t msg[]);

    MotorPanel getMotorPanel();

    InterfacePanel getInterfacePanel();

    // initialize motors
    void setup();

     InterfacePanel * interfacePanel;
     
  private:
    // ID for the pixel (also used for the motor ID)
    int id;

    MotorPanel * motorPanel;
   

    // variable that keeps track of whether or not elements in the urban panel changed (i.e. motor moved, buttons pressed, etc)
    bool stateChanged;

    bool disableMotor;
};

#endif

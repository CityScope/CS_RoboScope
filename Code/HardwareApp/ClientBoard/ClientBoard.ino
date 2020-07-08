/*
   Client Code to run on each board. Board controls up to n type of motors

   Thomas Sanchez Lengeling
   Terry Kang

   MIT Media Lab
   City Science

*/


#include <Wire.h> // Include the I2C library (required)
#include <SparkFunSX1509.h> // Include SX1509 library

#include <Arduino.h>
#include "UrbanPanel.h"
#include "CanBusParser.h"
#include "Debug.h"

#define SERIAL_BR 96000

int motorsPerPanel = 8;

//int dir = -1;

//int dir[] = { -1, -1, -1, -1, -1, -1, -1, -1};

int dir[] = { -1, 0, 0, 0, 0, 0, 0, 0};

//int dir[] = {1, 1, 0, 0, 1, 1, 0, 0};

int motorIDs[] = {0, 2, 4, 6, 1, 3, 5, 7};

//real
//int motorIDs[] = {1, 3, 5, 7, 2, 4, 6, 8};

// Panel Control
int panelID = 0;

UrbanPanel * urbanPanel;
//wqqCanBusParser * canBusParser = new CanBusParser();

//false cycle
boolean enableReturn = true;

int stepTime = ceil(15 * GMOTOR_STEPS);

float stepTimeS = 5.0;

int stepTimes[] = {ceil(stepTimeS * GMOTOR_STEPS), ceil(stepTimeS * GMOTOR_STEPS), ceil(stepTimeS * GMOTOR_STEPS), ceil(stepTimeS * GMOTOR_STEPS),
                   ceil(stepTimeS * GMOTOR_STEPS), ceil(stepTimeS * GMOTOR_STEPS), ceil(stepTimeS * GMOTOR_STEPS), ceil(stepTimeS * GMOTOR_STEPS)
                  };


bool limitActivated  = false;

void setup(void)
{
  Serial.begin(SERIAL_BR);
  delay(2000);

  urbanPanel = new UrbanPanel(panelID);

  Serial.println(F("Setting up Panels:"));
  urbanPanel->setup();

  Serial.println(F("Starting Sending"));

  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN_SWITCH), limitswitch, CHANGE);


  /*
    urbanPanel->moveMotor(0, stepTime * dir[0]);
    urbanPanel->moveMotor(2, stepTime * dir[1]);
    urbanPanel->moveMotor(4, stepTime * dir[2]);
    urbanPanel->moveMotor(6, stepTime * dir[3]);


    urbanPanel->moveMotor(1, stepTime * dir[4]);
    urbanPanel->moveMotor(3, stepTime * dir[5]);
    urbanPanel->moveMotor(5, stepTime * dir[6]);
    urbanPanel->moveMotor(7, stepTime * dir[7]);
  */

  urbanPanel->moveMotor(0, stepTimes[0] * dir[0]);
  urbanPanel->moveMotor(2, stepTimes[1] * dir[1]);
  urbanPanel->moveMotor(4, stepTimes[2] * dir[2]);
  urbanPanel->moveMotor(6, stepTimes[3] * dir[3]);


  urbanPanel->moveMotor(1, stepTimes[4] * dir[4]);
  urbanPanel->moveMotor(3, stepTimes[5] * dir[5]);
  urbanPanel->moveMotor(5, stepTimes[6] * dir[6]);
  urbanPanel->moveMotor(7, stepTimes[7] * dir[7]);
}

// -------------------------------------------------------------
void loop(void)
{
  wave();

  urbanPanel->interfacePanel->rainbow(millis());
  // limitMove();

  if (limitActivated) {
    urbanPanel->interfacePanel->printLimitState();
    
    limitActivated = false;
    enableReturn = true;
  }


  // // Checks to see if certain buttons are being pressed to move the motor (represents user interaction)
  // urbanPanel->checkInterfaceInput();

  // // if there is a change to the state send a message
  //if (urbanPanel->getStateChange()) {



  // }
  //   uint8_t message[MSG_LENGTH];

  //   // Temporary just to emphasize that there was a change to the system
  //   for (int i = 0; i < MSG_LENGTH; i++){
  //     message[i] = 1;
  //   }
  //   canBusParser->updateMsg(message);
  //   canBusParser->sendMsg();
  // }

  // canBusParser->waitforMsg();

  // uint8_t msg[8];
  // canBusParser->getRxMsg(msg);

  // urbanPanel->interpretMsg(msg);
}

void limitMove() {

  urbanPanel->stopMotorsToLimitPosition();

}

// -------------------------------------------------------------
void wave() {
  for (int i = 0; i < motorsPerPanel; i++) {
    unsigned waitTime = urbanPanel->checkMotorState(motorIDs[i]);

    if (waitTime != 0) {
      Serial.print(i);
      Serial.print(" ");
      Serial.println(waitTime);
    }

    //times


    if (waitTime <= 0) {
      urbanPanel->stopMotor(motorIDs[i]);

      if (enableReturn  ==  true) {

        urbanPanel->moveMotor(0, stepTimes[0] * dir[0] * -1);
        urbanPanel->moveMotor(2, stepTimes[1] * dir[1] * -1);
        urbanPanel->moveMotor(4, stepTimes[2] * dir[2] * -1);
        urbanPanel->moveMotor(6, stepTimes[3] * dir[3] * -1);

        urbanPanel->moveMotor(1, stepTimes[4] * dir[4] * -1);
        urbanPanel->moveMotor(3, stepTimes[5] * dir[5] * -1);
        urbanPanel->moveMotor(5, stepTimes[6] * dir[6] * -1);
        urbanPanel->moveMotor(7, stepTimes[7] * dir[7] * -1);

        enableReturn = false;
      }
      //reset backwards
    }
  }
}

//
//https://arduino.stackexchange.com/questions/20608/pass-classs-public-function-as-an-argument-to-external-command
void limitswitch() {
  limitActivated = true;
}

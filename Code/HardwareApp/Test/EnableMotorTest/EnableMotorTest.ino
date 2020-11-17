#include <Arduino.h>
#include "Stepper.h"
#include "MotorPins.h"

//Motor 1
Stepper motor01(0, GMOTOR_STEPS, DIR_PIN_02, STEP_PIN_02, GENABLE_PIN);

//false -> backwards
//true  -> forwards
boolean dirMotor = false;

//current motor
int currentMotor = 1;

boolean enableMotor = true;
boolean moveMotor = true;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  Serial.println("Starting");

  delay(1000);

  motor01.setRPM(GRPM);

  //start the motors;
  motor01.init();

  delay(2000);

  //print the motor info
  motor01.printMotorInfo();

}


void loop() {

  if (motor01.isEnable()) {

    if (moveMotor) {
      motor01.moveForward();
    }
  }

  if (Serial.available() > 0) {
    char key = Serial.read();
    if (key == 'a') {
      motor01.stop();
      Serial.println("stop");
    }

    if (key == 'z') {
      motor01.start();
      Serial.println("start");
    }
    if (key == 'm') {
      moveMotor = !moveMotor;
      Serial.print(moveMotor);
      Serial.println(" move");
    }

    if (key == 'q') {
      motor01.sleepOn();
      Serial.println("sleep ON");
    }
    if (key == 'w') {
         motor01.sleepOff();
      Serial.println("sleep OFF");
    }

  }
}

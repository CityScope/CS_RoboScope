#include <Arduino.h>

#include <Wire.h> // Include the I2C library (required)
#include <SparkFunSX1509.h> // Include SX1509 library


#include "Stepper.h"
#include "MotorPins.h"

// SX1509 I2C address (set by ADDR1 and ADDR0 (00 by default):

const byte SX1509_ADDRESS_00 = 0x3E;  // SX1509 I2C address
SX1509 sx00;

//Motor 1
Stepper * motor01;

/*
//Motor 2
Stepper motor02(2, GMOTOR_STEPS, DIR_PIN_02, STEP_PIN_02, 0, GM0_PIN, GM1_PIN);

//Motor 3
Stepper motor03(3, GMOTOR_STEPS, DIR_PIN_03, STEP_PIN_03, 0, GM0_PIN, GM1_PIN);

//Motor 4
Stepper motor04(4, GMOTOR_STEPS, DIR_PIN_04, STEP_PIN_04, 0, GM0_PIN, GM1_PIN);

//Motor 5
Stepper motor05(5, GMOTOR_STEPS, DIR_PIN_05, STEP_PIN_05, 0, GM0_PIN, GM1_PIN);

//Motor 6
Stepper motor06(6, GMOTOR_STEPS, DIR_PIN_06, STEP_PIN_06, 0, GM0_PIN, GM1_PIN);

//Motor 7
Stepper motor07(7, GMOTOR_STEPS, DIR_PIN_07, STEP_PIN_07, 0, GM0_PIN, GM1_PIN);

//Motor 8
Stepper motor08(8, GMOTOR_STEPS, DIR_PIN_08, STEP_PIN_08, 0, GM0_PIN, GM1_PIN);
*/

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
  
  Serial.println("SX1509: ");

  if (!sx00.begin(SX1509_ADDRESS_00) ) {
    Serial.println("Failed 0");
    Serial.println(SX1509_ADDRESS_00);
    //while (1) ; // If we fail to communicate, loop forever.
  } else {
    Serial.println("Connected 0");
  }


  //sx 00
  sx00.pinMode(0, OUTPUT); // ENABLE PIN FOR MOTOR01
  sx00.pinMode(1, OUTPUT); // SLEEP PIN FOR MOTOR01
  motor01 = new Stepper(1, GMOTOR_STEPS, DIR_PIN_01, STEP_PIN_01, &sx00, 0, 1, GM0_PIN, GM1_PIN);
  
  motor01->setRPM(GRPM);
  /*
  motor02.setRPM(GRPM);
  motor03.setRPM(GRPM);
  motor04.setRPM(GRPM);
  motor05.setRPM(GRPM);
  motor06.setRPM(GRPM);
  motor07.setRPM(GRPM);
  motor08.setRPM(GRPM);
  */
  
  //start the motors;
  motor01->init();
  /*
  motor02.init();
  motor03.init();
  motor04.init();
  motor05.init();
  motor06.init();
  motor07.init();
  motor08.init();
  */

  delay(2000);

  //print the motor info
  motor01->printMotorInfo();
  /*
  motor02.printMotorInfo();
  motor03.printMotorInfo();
  motor04.printMotorInfo();
  motor05.printMotorInfo();
  motor06.printMotorInfo();
  motor07.printMotorInfo();
  motor08.printMotorInfo();
  */

  
}
/*
void move_all_motors() {
  
  motor01.moveForward();
  motor02.moveForward();
  motor03.moveForward();
  motor04.moveForward();
  motor05.moveForward();
  motor06.moveForward();
  motor07.moveForward();
  motor08.moveForward();
}
*/

void loop() {
  /*
  if (Serial.available() > 0) {
    char key = Serial.read();

    if (key == 'a') {
      move_all_motors();
      Serial.println("Moving all motors");
    }
  }

  */
  
  if (motor01->isEnable()) {

    if (moveMotor) {
      motor01->moveForward();
    }
  }

  if (Serial.available() > 0) {
    char key = Serial.read();
    if (key == 'a') {
      motor01->stop();
      Serial.println("stop");
    }

    if (key == 'z') {
      motor01->start();
      Serial.println("start");
    }
    if (key == 'm') {
      moveMotor = !moveMotor;
      Serial.print(moveMotor);
      Serial.println(" move");
    }

    if (key == 'q') {
      motor01->sleepOn();
      Serial.println("sleep ON");
    }
    if (key == 'w') {
         motor01->sleepOff();
      Serial.println("sleep OFF");
    }

  }
  
}

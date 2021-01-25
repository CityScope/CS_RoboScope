#include <Arduino.h>
#include "Stepper.h"
#include "MotorPins.h"
#include <TeensyThreads.h>



//Motor 1
Stepper motor01(0, GMOTOR_STEPS, DIR_PIN_01, STEP_PIN_01, GENABLE_PIN);

//Motor 2
Stepper motor02(1, GMOTOR_STEPS, DIR_PIN_02, STEP_PIN_02, GENABLE_PIN);

//Motor 3
Stepper motor03(2, GMOTOR_STEPS, DIR_PIN_03, STEP_PIN_03, GENABLE_PIN);

//Motor 4
Stepper motor04(3, GMOTOR_STEPS, DIR_PIN_04, STEP_PIN_04, GENABLE_PIN);

//Motor 5
Stepper motor05(4, GMOTOR_STEPS, DIR_PIN_05, STEP_PIN_05, GENABLE_PIN);

//Motor 6
Stepper motor06(5, GMOTOR_STEPS, DIR_PIN_06, STEP_PIN_06, GENABLE_PIN);

//Motor 7
Stepper motor07(6, GMOTOR_STEPS, DIR_PIN_07, STEP_PIN_07, GENABLE_PIN);

//Motor 8
Stepper motor08(7, GMOTOR_STEPS, DIR_PIN_08, STEP_PIN_08, GENABLE_PIN);



//false -> backwards
//true  -> forwards
boolean dirMotor = false;

//current motor
int currentMotor = 2;

boolean enableMotor = true;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  Serial.println("Starting");
  delay(2000);

  motor01.setRPM(GRPM);
  motor02.setRPM(GRPM);
  motor03.setRPM(GRPM);
  motor04.setRPM(GRPM);
  motor05.setRPM(GRPM);
  motor06.setRPM(GRPM);
  motor07.setRPM(GRPM);
  motor08.setRPM(GRPM);

  //start the motors;
  motor01.init();
  motor02.init();
  motor03.init();
  motor04.init();
  motor05.init();
  motor06.init();
  motor07.init();
  motor08.init();



  delay(2000);

  motor01.startMoveBackward( 10 );
  motor02.startMoveBackward( 10 );

  //print the motor info
  //motor01.printMotorInfo();
  // motor02.printMotorInfo();
  //motor03.printMotorInfo();


  threads.addThread(moveMotor);


}

volatile unsigned waitTimeMicros01 = 0;

void moveMotor() {
  while (1) {
    waitTimeMicros01 = motor01.getNextAction();

    if (waitTimeMicros01 != 0) {
      Serial.print("1: ");
      Serial.println(waitTimeMicros01);
    } else {
      Serial.print("1: ");
      Serial.println("stop");
    }

  }
}


void loop() {
  //timer for moving up and down several motors at once

  unsigned waitTimeMicros02 = motor02.getNextAction();

  if (waitTimeMicros02 != 0) {
    Serial.print("2: ");
    Serial.println(waitTimeMicros02);
  }

  if (waitTimeMicros02 <= 0) {
    motor02.stop();
    Serial.print("2: ");
    Serial.println("stop");
  }

  if (waitTimeMicros01 <= 0) {
    motor01.stop();
    //Serial.print("1: ");
    //Serial.println("stop");
  }


  /*
    if (enableMotor) {
      switch (currentMotor) {
        case 1:
          if (dirMotor) {
            motor01.moveForward();
          } else {
            motor01.moveBackward();
          }
          break;
        case 2:
          if (dirMotor) {
            motor02.moveForward();
          } else {
            motor02.moveBackward();
          }
          break;
        case 3:
          if (dirMotor) {
            motor03.moveForward();
          } else {
            motor03.moveBackward();
          }
          break;
        case 4:
          if (dirMotor) {
            motor04.moveForward();
          } else {
            motor04.moveBackward();
          }
          break;
        case 5:
          if (dirMotor) {
            motor05.moveForward();
          } else {
            motor05.moveBackward();
          }
          break;
        case 6:
          if (dirMotor) {
            motor06.moveForward();
          } else {
            motor06.moveBackward();
          }
          break;

        case 7:
          if (dirMotor) {
            motor07.moveForward();
          } else {
            motor07.moveBackward();
          }
          break;
        case 8:
          if (dirMotor) {
            motor08.moveForward();
          } else {
            motor08.moveBackward();
          }
          break;
      }
    }
    /*

        if (Serial.available() > 0) {
          char key = Serial.read();
          if (key == 'z') {
            Serial.println("dir backwards");
            dirMotor = false;
          }
          if (key == 'x') {
            Serial.println("dir forwards");
            dirMotor = true;
          }

          //motors
          if (key == '1') {
            currentMotor = 1;
            Serial.println("Motor 1");
          }

          if (key == '2') {
            currentMotor = 2;
            Serial.println("Motor 2");
          }

          if (key == '3') {
            currentMotor = 3;
            Serial.println("Motor 3");
          }

          if (key == '4') {
            currentMotor = 4;
            Serial.println("Motor 4");
          }

          if (key == '5') {
            currentMotor = 5;
            Serial.println("Motor 5");
          }

          if (key == '6') {
            currentMotor = 6;
            Serial.println("Motor 6");
          }

          if (key == '7') {
            currentMotor = 7;
            Serial.println("Motor 7");
          }

          if (key == '8') {
            currentMotor = 8;
            Serial.println("Motor 8");
          }

          if (key == 'q') {
            enableMotor = !enableMotor;
            Serial.print("ENABLE MOTOR ");
            Serial.print(currentMotor);
            Serial.print("  ");
            Serial.println(enableMotor);
          }

        }
  */
}

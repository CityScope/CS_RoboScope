#include <Arduino.h>
#include "Stepper.h"
#include "MotorPins.h"


//false -> backwards
//true  -> forwards
boolean dirMotor = false;

boolean enableMotor = true;
boolean moveMotor = true;

//index
int indexMotor = 0;

Stepper * motorDRV8880;

int trq0Mode = 1;
int trq1Mode = 0;

int activateMotor = 1;

//------------------------
void setup() {
  Serial.begin(9600);
  Serial.println("Starting");
  delay(500);

  delay(1000);

  motorDRV8880 = new Stepper(0, GMOTOR_STEPS, DIR_PIN_01, STEP_PIN_01, ENABLE_PIN_01, M0_PIN_01, M1_PIN_01);
  
 // motorDRV8880 = new Stepper(0, GMOTOR_STEPS, DIR_PIN_02, STEP_PIN_02, ENABLE_PIN_02, M0_PIN_02, M1_PIN_02);


  delay(2000);

  pinMode(trq0Mode, OUTPUT);
  pinMode(trq1Mode, OUTPUT);

  if (trq0Mode == 1) {
    digitalWrite(TRQ0_PIN, HIGH);
  }

  if (trq1Mode == 1) {
    digitalWrite(TRQ1_PIN, HIGH);
  }

}


void loop() {

  //if (motor01->isEnable()) {

  if (moveMotor) {
    //  motor01->moveForward();
  }
  // }

  if (activateMotor ==  1) {
    motorDRV8880->getNextAction();

  }

  if (Serial.available() > 0) {
    char key = Serial.read();

    if (key == '1') {
      indexMotor = 0;

      if ( motorDRV8880->isEnable()) {
        motorDRV8880->stop();
      } else {
        motorDRV8880->startMoveForward(20);
        motorDRV8880->start();
      }
      Serial.println("1");
    }


    if (key == 'b') {
        if (activateMotor ==  1) {
          motorDRV8880->startMoveBackward(50);
          motorDRV8880->start();
          motorDRV8880->sleepOff();
        
      }
      Serial.println("ON ALL");
    }


    if (key == 'n') {
      if (activateMotor ==  1) {
        motorDRV8880->startMoveForward(50);
        motorDRV8880->start();
        motorDRV8880->sleepOff();
      }

      Serial.println("ON ALL");
    }

    if (key == 'm') {
      if (activateMotor ==  1) {
        motorDRV8880->stop();
        motorDRV8880->sleepOn();
      }
      Serial.println("OFF ALL");
    }


    if (key == 'c') {
      motorDRV8880->startMoveForward(5);
      Serial.println("fowards");
    }

    if (key == 'q') {
      motorDRV8880->stop();
      motorDRV8880->sleepOn();
      Serial.println("OFF");

    }
    if (key == 'w') {
      motorDRV8880->start();
      motorDRV8880->sleepOff();
      Serial.println("ON");
    }

    if (key == 'a') {
      motorDRV8880->stop();
      Serial.println("stop");
    }

    if (key == 's') {
      motorDRV8880->start();
      Serial.println("start");
    }


    if (key == 'r') {
      trq1Mode = 0;
      digitalWrite(TRQ1_PIN, LOW);
      Serial.println("Amp Mode");
      Serial.print(trq1Mode);
      Serial.print(" ");
      Serial.println(trq0Mode);
    }
    if (key == 't') {
      trq1Mode = 1;
      digitalWrite(TRQ1_PIN, HIGH);
      Serial.println("Amp Mode");
      Serial.print(trq1Mode);
      Serial.print(" ");
      Serial.println(trq0Mode);
    }
    if (key == 'f') {
      trq0Mode = 0;
      digitalWrite(TRQ0_PIN, LOW);
      Serial.println("Amp Mode");
      Serial.print(trq1Mode);
      Serial.print(" ");
      Serial.println(trq0Mode);
    }
    if (key == 'g') {
      trq0Mode = 1;
      digitalWrite(TRQ0_PIN, HIGH);
      Serial.println("Amp Mode");
      Serial.print(trq1Mode);
      Serial.print(" ");
      Serial.println(trq0Mode);
    }

  }
}

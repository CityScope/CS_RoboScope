#include "UrbanPanel.h"

UrbanPanel::UrbanPanel(int panelId) {
  id = panelId;

  int muxCounter = 0;
  while (muxCounter != 3) {
    if (!motorSX.begin(SX1509_ADDRESS_00) ) {
      Serial.print("Failed 00 ");
      Serial.print(" " + SX1509_ADDRESS_00);
      Serial.print(" ");
      Serial.println(muxCounter);
      delay(100);
      muxCounter++;
    } else {
      Serial.println("Connected 00");
      Serial.print(" " + SX1509_ADDRESS_00);
      Serial.print(" ");
      Serial.println(muxCounter);
      break;
    }
  }

  interfacePanel = new InterfacePanel();
  motors = new Motors();
  //displayPanel = new DisplayPanel();

  Serial.println("Setting up neo pixel LEDs");


  const byte NEO_PIN[8] = {NEO_PIN_01, NEO_PIN_02, NEO_PIN_03, NEO_PIN_04,
                         NEO_PIN_05, NEO_PIN_06, NEO_PIN_07, NEO_PIN_08
                        };

  for (int i = 0; i < 8; i++) {
    pixels[i] = new Adafruit_NeoPixel(NUMPIXELS, NEO_PIN[i], NEO_GRBW + NEO_KHZ800);
    pixels[i]->begin();
    pixels[i]->clear();
  }

}

void UrbanPanel::setPixelColor(int i, int r, int g, int b) {
  for (int j = 0; j < NUMPIXELS; j++) {
    pixels[i]->setPixelColor(j, pixels[i]->Color(r, g, b));
  }
  pixels[i]->show();
}

int UrbanPanel::getPushState(int i) {
  return interfacePanel->getPushCurrentState(i);
}

int UrbanPanel::getLimitState(int i) {
  return interfacePanel->getLimitCurrentState(i);
}


void UrbanPanel::init() {
  interfacePanel->init();
  motors->init();

  delay(2000);

}

void UrbanPanel::motorTimerUpdate() {
  if (limitActivated) {
    interfacePanel->updateLimitState();

    for (int i = 0; i < MOTORS_PER_PANEL; i++) {
      if (interfacePanel->getLimitSwitchState(i)) {
        motors[i]->activeLimit();
        motors[i]->stop(motorSX);
        motors[i]->sleepOn(motorSX);
      }
      if (interfacePanel->getLimitState(i)) {
        interfacePanel->resetLimitSwitch(i);
      }
    }

    for (int i = 0; i < MOTORS_PER_PANEL; i++) {
      if (interfacePanel->getPushSwitchState(i)) {
        motors[i]->activeLimit();
        motors[i]->stop(motorSX);
        motors[i]->sleepOn(motorSX);
      }
      if (interfacePanel->getPushState(i)) {
        interfacePanel->resetPushSwitch(i);
      }
    }

    limitActivated = false;
  }
}

void UrbanPanel::limitswitch() {
  limitActivated = true;
}

/*
*/
void UrbanPanel::movePixelUp(int i, int steps) {
  //motorPanel->startMove
  //motors[i]->moveForward();
  motors[i]->startMoveForward(steps);
  motors[i]->start(motorSX);
  motors[i]->sleepOff(motorSX);

}

void UrbanPanel::stopMotor(int i) {
  motors[i]->stop(motorSX);
  motors[i]->sleepOn(motorSX);
}

void UrbanPanel::setTrq1Mode(int i) {
  if (i != 0 || i != 1) {
    Serial.println("Set trq mode to either 1 or 0");
    return;
  }

  trq1Mode = i;
  switch (trq1Mode) {
    case 1:
      digitalWrite(TRQ1_PIN, HIGH);
      Serial.println("Amp Mode");
      Serial.print(trq1Mode);
      Serial.print(" ");
      Serial.println(trq2Mode);
      break;
    case 0:
      digitalWrite(TRQ1_PIN, LOW);
      Serial.println("Amp Mode");
      Serial.print(trq1Mode);
      Serial.print(" ");
      Serial.println(trq2Mode);
      break;
  }
}

void UrbanPanel::setTrq2Mode(int i) {
  if (i != 0 || i != 1) {
    Serial.println("Set trq mode to either 1 or 0");
    return;
  }

  trq2Mode = i;
  switch (trq1Mode) {
    case 1:
      digitalWrite(TRQ2_PIN, HIGH);
      Serial.println("Amp Mode");
      Serial.print(trq1Mode);
      Serial.print(" ");
      Serial.println(trq2Mode);
      break;
    case 0:
      digitalWrite(TRQ2_PIN, LOW);
      Serial.println("Amp Mode");
      Serial.print(trq1Mode);
      Serial.print(" ");
      Serial.println(trq2Mode);
      break;
  }
}

void UrbanPanel::movePixelDown(int i, int steps) {
  //motorPanel->startMove
  motors[i]->startMoveBackward(steps);
}

/*
  Moves the defined pixel to a specified position
*/
void UrbanPanel::setPixelPosition(int i, float pos) {


}

void UrbanPanel::pixelLoop(int i) {
    waitTimeMicros[i] = motors[i]->getNextAction();

    /*
    if (getLimitState(0) == 1) {
      movePixelDown(i, 5);
      motors[i]->stop();
    }*/
    if (waitTimeMicros[i] <= 0) {
      motors[i]->stop(motorSX);
    }
}

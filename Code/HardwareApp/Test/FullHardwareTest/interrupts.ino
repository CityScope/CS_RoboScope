//sx 01
void initSX01_interrupts() {
  sx01.pinMode(SWITCH_01_SX01, INPUT_PULLUP);
  sx01.pinMode(SWITCH_02_SX01, INPUT_PULLUP);
  sx01.pinMode(SWITCH_03_SX01, INPUT_PULLUP);
  sx01.pinMode(SWITCH_04_SX01, INPUT_PULLUP);

  sx01.pinMode(DOWN_01_SX01, INPUT_PULLUP);
  sx01.pinMode(DOWN_02_SX01, INPUT_PULLUP);
  sx01.pinMode(DOWN_03_SX01, INPUT_PULLUP);
  sx01.pinMode(DOWN_04_SX01, INPUT_PULLUP);

  sx01.pinMode(UP_01_SX01, INPUT_PULLUP);
  sx01.pinMode(UP_02_SX01, INPUT_PULLUP);
  sx01.pinMode(UP_03_SX01, INPUT_PULLUP);
  sx01.pinMode(UP_04_SX01, INPUT_PULLUP);

  sx01.pinMode(TOUCH_01_SX01, INPUT_PULLUP);
  sx01.pinMode(TOUCH_02_SX01, INPUT_PULLUP);
  sx01.pinMode(TOUCH_03_SX01, INPUT_PULLUP);
  sx01.pinMode(TOUCH_04_SX01, INPUT_PULLUP);

  //interrupt
  sx01.enableInterrupt(SWITCH_01_SX01, CHANGE);
  sx01.enableInterrupt(SWITCH_02_SX01, CHANGE);
  sx01.enableInterrupt(SWITCH_03_SX01, CHANGE);
  sx01.enableInterrupt(SWITCH_04_SX01, CHANGE);

  sx01.enableInterrupt(DOWN_01_SX01, CHANGE);
  sx01.enableInterrupt(DOWN_02_SX01, CHANGE);
  sx01.enableInterrupt(DOWN_03_SX01, CHANGE);
  sx01.enableInterrupt(DOWN_04_SX01, CHANGE);

  sx01.enableInterrupt(UP_01_SX01, CHANGE);
  sx01.enableInterrupt(UP_02_SX01, CHANGE);
  sx01.enableInterrupt(UP_03_SX01, CHANGE);
  sx01.enableInterrupt(UP_04_SX01, CHANGE);

  sx01.enableInterrupt(TOUCH_01_SX01, CHANGE);
  sx01.enableInterrupt(TOUCH_02_SX01, CHANGE);
  sx01.enableInterrupt(TOUCH_03_SX01, CHANGE);
  sx01.enableInterrupt(TOUCH_04_SX01, CHANGE);

  //debouncePin
  sx01.debounceTime(4);

  sx01.debouncePin(SWITCH_01_SX01);
  sx01.debouncePin(SWITCH_02_SX01);
  sx01.debouncePin(SWITCH_03_SX01);
  sx01.debouncePin(SWITCH_04_SX01);

  sx01.debouncePin(DOWN_01_SX01);
  sx01.debouncePin(DOWN_02_SX01);
  sx01.debouncePin(DOWN_03_SX01);
  sx01.debouncePin(DOWN_04_SX01);

  sx01.debouncePin(UP_01_SX01);
  sx01.debouncePin(UP_02_SX01);
  sx01.debouncePin(UP_03_SX01);
  sx01.debouncePin(UP_04_SX01);

  sx01.debouncePin(TOUCH_01_SX01);
  sx01.debouncePin(TOUCH_02_SX01);
  sx01.debouncePin(TOUCH_03_SX01);
  sx01.debouncePin(TOUCH_04_SX01);

  pinMode(INTERRUPT_PIN_SWITCH_01, INPUT_PULLUP);
}


//sx 02 
void initSX02_interrupts() {

  sx02.pinMode(SWITCH_05_SX02, INPUT_PULLUP);
  sx02.pinMode(SWITCH_06_SX02, INPUT_PULLUP);
  sx02.pinMode(SWITCH_07_SX02, INPUT_PULLUP);
  sx02.pinMode(SWITCH_08_SX02, INPUT_PULLUP);

  sx02.pinMode(DOWN_05_SX02, INPUT_PULLUP);
  sx02.pinMode(DOWN_06_SX02, INPUT_PULLUP);
  sx02.pinMode(DOWN_07_SX02, INPUT_PULLUP);
  sx02.pinMode(DOWN_08_SX02, INPUT_PULLUP);

  sx02.pinMode(UP_05_SX02, INPUT_PULLUP);
  sx02.pinMode(UP_06_SX02, INPUT_PULLUP);
  sx02.pinMode(UP_07_SX02, INPUT_PULLUP);
  sx02.pinMode(UP_08_SX02,  INPUT_PULLUP);

  sx02.pinMode(TOUCH_05_SX02, INPUT_PULLUP);
  sx02.pinMode(TOUCH_06_SX02, INPUT_PULLUP);
  sx02.pinMode(TOUCH_07_SX02, INPUT_PULLUP);
  sx02.pinMode(TOUCH_08_SX02, INPUT_PULLUP);

  //interrupt
  sx02.enableInterrupt(SWITCH_05_SX02, CHANGE);
  sx02.enableInterrupt(SWITCH_06_SX02, CHANGE);
  sx02.enableInterrupt(SWITCH_07_SX02, CHANGE);
  sx02.enableInterrupt(SWITCH_08_SX02, CHANGE);

  sx02.enableInterrupt(DOWN_05_SX02, CHANGE);
  sx02.enableInterrupt(DOWN_06_SX02, CHANGE);
  sx02.enableInterrupt(DOWN_07_SX02, CHANGE);
  sx02.enableInterrupt(DOWN_08_SX02, CHANGE);

  sx02.enableInterrupt(UP_05_SX02, CHANGE);
  sx02.enableInterrupt(UP_06_SX02, CHANGE);
  sx02.enableInterrupt(UP_07_SX02, CHANGE);
  sx02.enableInterrupt(UP_08_SX02, CHANGE);

  sx02.enableInterrupt(TOUCH_05_SX02, CHANGE);
  sx02.enableInterrupt(TOUCH_06_SX02, CHANGE);
  sx02.enableInterrupt(TOUCH_07_SX02, CHANGE);
  sx02.enableInterrupt(TOUCH_08_SX02, CHANGE);

  //debouncePin
  sx02.debounceTime(4);

  sx02.debouncePin(SWITCH_05_SX02);
  sx02.debouncePin(SWITCH_06_SX02);
  sx02.debouncePin(SWITCH_07_SX02);
  sx02.debouncePin(SWITCH_08_SX02);

  sx02.debouncePin(DOWN_05_SX02);
  sx02.debouncePin(DOWN_06_SX02);
  sx02.debouncePin(DOWN_07_SX02);
  sx02.debouncePin(DOWN_08_SX02);

  sx02.debouncePin(UP_05_SX02);
  sx02.debouncePin(UP_06_SX02);
  sx02.debouncePin(UP_07_SX02);
  sx02.debouncePin(UP_08_SX02);

  sx02.debouncePin(TOUCH_05_SX02);
  sx02.debouncePin(TOUCH_06_SX02);
  sx02.debouncePin(TOUCH_07_SX02);
  sx02.debouncePin(TOUCH_08_SX02);

  pinMode(INTERRUPT_PIN_SWITCH_02, INPUT_PULLUP);
}

//SX03
void initSX03() {
  sx03.pinMode(DIP_01_SX03, INPUT);
  sx03.pinMode(DIP_02_SX03, INPUT);
  sx03.pinMode(DIP_03_SX03, INPUT);
  sx03.pinMode(DIP_04_SX03, INPUT);
  sx03.pinMode(DIP_05_SX03, INPUT);
  sx03.pinMode(DIP_06_SX03, INPUT);
  sx03.pinMode(DIP_07_SX03, INPUT);
  sx03.pinMode(DIP_08_SX03, INPUT);
  sx03.pinMode(DIP_09_SX03, INPUT);
  sx03.pinMode(DIP_10_SX03, INPUT);
}

//---------------------------------------------
void checkSXO1_interrupts() {
  interruptActivate01 = true;
}
void checkSXO2_interrupts() {
  interruptActivate02 = true;
}
//---------------------------------------------
void updateSX01_interrupts() {
  if (interruptActivate01) {
    Serial.println("interrupt sx01");

    unsigned int intStatus = sx01.interruptSource();
    Serial.println("intStatus = " + String(intStatus, BIN));

    if (bool(intStatus & (1 << SWITCH_01_SX01)) == HIGH) {
      Serial.println("LIMIT PIN 01");
    }
    if (bool(intStatus & (1 << SWITCH_02_SX01)) == HIGH) {
      Serial.println("LIMIT PIN 02");
    }
    if (bool(intStatus & (1 << SWITCH_03_SX01)) == HIGH) {
      Serial.println("LIMIT PIN 03");
    }
    if (bool(intStatus & (1 << SWITCH_04_SX01)) == HIGH) {
      Serial.println("LIMIT PIN 04");
    }

    //SX 02 down
    if (bool(intStatus & (1 << DOWN_01_SX01)) == HIGH) {
      Serial.println("PIN 01 DOWN");
      colorChange = true;
      colorId = 0;
    }
    if (bool(intStatus & (1 << DOWN_02_SX01)) == HIGH) {
      Serial.println("PIN 02 DOWN");
      colorChange = true;
      colorId = 1;
    }
    if (bool(intStatus & (1 << DOWN_03_SX01)) == HIGH) {
      Serial.println("PIN 03 DOWN");
      colorChange = true;
      colorId = 2;
    }
    if (bool(intStatus & (1 << DOWN_04_SX01)) == HIGH) {
      Serial.println("PIN 04 DOWN");
      colorChange = true;
      colorId = 3;
    }
    interruptActivate01 = false;
  }
}
//----------------------------------------------------
void updateSX02_interrupts() {
  if (interruptActivate02) {
    Serial.println("interrupt sx02");

    unsigned int intStatus = sx02.interruptSource();
    Serial.println("intStatus = " + String(intStatus, BIN));

    //
    if (intStatus & (1 << SWITCH_05_SX02) == HIGH) {
      Serial.println("LIMIT PIN 05");
    }
    if (intStatus & (1 << SWITCH_06_SX02) == HIGH) {
      Serial.println("LIMIT PIN 06");
    }
    if (intStatus & (1 << SWITCH_07_SX02) == HIGH) {
      Serial.println("LIMIT PIN 07");
    }
    if (intStatus & (1 << SWITCH_08_SX02) == HIGH) {
      Serial.println("LIMIT PIN 08");
    }

    //SX 02 down
    if (intStatus & (1 << DOWN_05_SX02) == HIGH) {
      Serial.println("PIN 05 DOWN");
      colorChange = true;
      colorId = 4;
    }
    if (intStatus & (1 << DOWN_06_SX02) == HIGH) {
      Serial.println("PIN 06 DOWN");
      colorChange = true;
      colorId = 5;
    }
    if (intStatus & (1 << DOWN_07_SX02) == HIGH) {
      Serial.println("PIN 07 DOWN");
      colorChange = true;
      colorId = 6;
    }
    if (intStatus & (1 << DOWN_08_SX02) == HIGH) {
      Serial.println("PIN 08 DOWN");
      colorChange = true;
      colorId = 7;
    }
    interruptActivate02 = false;
  }

}

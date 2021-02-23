
#include <TMCStepper.h>
#include <SparkFunSX1509.h>
#include <AccelStepper.h>

struct MotorPins {
  int EN_PIN;
  int DIR_PIN;
  int STEP_PIN;
};

#define EN_PIN_01           10 // SX Enable
#define DIR_PIN_01          6 // Direction
#define STEP_PIN_01         5 // Step

#define EN_PIN_02           6 //  SX Enable
#define DIR_PIN_02          8 // Direction
#define STEP_PIN_02         7 // Step 

#define EN_PIN_03           4 //  SX Enable
#define DIR_PIN_03          10 // Direction 
#define STEP_PIN_03         9 // Step 

#define EN_PIN_04           2 // SX Enable
#define DIR_PIN_04          12 // Direction
#define STEP_PIN_04         11 // Step

#define EN_PIN_05           8 // SX Enable
#define DIR_PIN_05          17 // Direction
#define STEP_PIN_05         16 // Step 

#define EN_PIN_06           0 // SX Enable
#define DIR_PIN_06          3 // Direction 
#define STEP_PIN_06         2 // Step 

#define EN_PIN_07           12 // SX Enable
#define DIR_PIN_07          23 // Direction 
#define STEP_PIN_07         22 // Step 

#define EN_PIN_08           14 // SX Enable
#define DIR_PIN_08          27 // Direction 
#define STEP_PIN_08         26 // Step 

#define SERIAL_PORT Serial5
#define DRIVER_ADDRESS 0b00

#define R_SENSE 0.11f


constexpr uint32_t steps_per_mm = 80;

TMC2209Stepper driver(&SERIAL_PORT, R_SENSE, DRIVER_ADDRESS);

AccelStepper steppers[8];

MotorPins motorPins[8];

//
bool shaft0 = true;
bool shaft1 = true;
bool shaft2 = true;
bool shaft3 = true;
bool shaft4 = true;
bool shaft5 = true;
bool shaft6 = true;
bool shaft7 = true;

//mux
SX1509 sx;
#define SX1509_ADDRESS_00  0x3E

//
const int selectMotor[3] = {5, 7, 9};
const int motorEnable = 15;

const int selectLED[3] = {11, 13, 15};
const int LEDEnable = 4;

void setup() {

  Serial.begin(250000);
  delay(1000);

  //pins Def;

  //starting
  int muxCounter = 0;
  Serial.println("starting motors");
  while (muxCounter != 3) {
    if (!sx.begin(SX1509_ADDRESS_00) ) {
      Serial.print("Failed 00 ");
      Serial.print(SX1509_ADDRESS_00);
      Serial.print(" ");
      Serial.println(muxCounter);
      delay(100);
      muxCounter++;
    } else {
      Serial.println("Connected 00");
      Serial.print(SX1509_ADDRESS_00);
      Serial.print(" ");
      Serial.println(muxCounter);
      break;
    }
  }

  setupSXPins();

  Serial.println("Init motors....");

  //SPI.begin();                    // SPI drivers
  SERIAL_PORT.begin(250000);      // HW UART drivers
  //   driver.beginSerial(250000);     // SW UART drivers

  driver.begin();                 //  SPI: Init CS pins and possible SW SPI pins
  // UART: Init SW UART (if selected) with default 115200 baudrate

  enableMotor(0);
  setupMotor(0);

  enableMotor(1);
  setupMotor(1);

  enableMotor(2);
  setupMotor(2);

  enableMotor(3);
  setupMotor(3);

  enableMotor(4);
  setupMotor(4);

  enableMotor(5);
  setupMotor(5);

  enableMotor(6);
  setupMotor(6);

  enableMotor(7);
  setupMotor(7);

}



void loop() {
  // put your main code here, to run repeatedly:
  // Run 5000 steps and switch direction in software
  for (uint16_t i = 500; i > 0; i--) {
    digitalWrite(STEP_PIN_01, HIGH);
    digitalWrite(STEP_PIN_02, HIGH);
    digitalWrite(STEP_PIN_03, HIGH);
    digitalWrite(STEP_PIN_04, HIGH);
    digitalWrite(STEP_PIN_05, HIGH);
    digitalWrite(STEP_PIN_06, HIGH);
    digitalWrite(STEP_PIN_07, HIGH);
    digitalWrite(STEP_PIN_08, HIGH);
    delayMicroseconds(64);

    digitalWrite(STEP_PIN_01, LOW);
    digitalWrite(STEP_PIN_02, LOW);
    digitalWrite(STEP_PIN_03, LOW);
    digitalWrite(STEP_PIN_04, LOW);
    digitalWrite(STEP_PIN_05, LOW);
    digitalWrite(STEP_PIN_06, LOW);
    digitalWrite(STEP_PIN_07, LOW);
    digitalWrite(STEP_PIN_08, LOW);
    delayMicroseconds(64);
  }


  if (Serial.available() > 0) {
    char key = Serial.read();
    if (key == '1') {
      shaft0 = !shaft0;
      enableMotor(0);
      driver.shaft(shaft0);
      testConnection(0);
    }

    if (key == '2') {
      shaft1 = !shaft1;
      enableMotor(1);
      driver.shaft(shaft1);
      testConnection(1);
    }

    if (key == '3') {
      shaft2 = !shaft2;
      enableMotor(2);
      driver.shaft(shaft2);
      testConnection(2);
    }

    if (key == '4') {
      shaft3 = !shaft3;
      enableMotor(3);
      driver.shaft(shaft3);
      testConnection(3);
    }

    if (key == '5') {
      shaft4 = !shaft4;
      enableMotor(4);
      driver.shaft(shaft4);
      testConnection(4);
    }

    if (key == '5') {
      shaft5 = !shaft5;
      enableMotor(5);
      driver.shaft(shaft5);
      testConnection(5);
    }

    if (key == '6') {
      shaft6 = !shaft6;
      enableMotor(6);
      driver.shaft(shaft6);
      testConnection(6);
    }

    if (key == '7') {
      shaft7 = !shaft7;
      enableMotor(7);
      driver.shaft(shaft7);
      testConnection(7);
    }
  }
}

void setupMotor(int id) {
  Serial.print("Setup Motor ");
  Serial.print(id);
  Serial.println("...");

  driver.toff(5);                 // Enables driver in software
  driver.rms_current(500);        // Set motor RMS current
  driver.microsteps(32);          // Set microsteps to 1/16th

  //driver.en_pwm_mode(true);       // Toggle stealthChop on TMC2130/2160/5130/5160
  driver.en_spreadCycle(false);   // Toggle spreadCycle on TMC2208/2209/2224
  driver.pwm_autoscale(true);     // Needed for stealthChop

  Serial.print(F("\nTesting connection... "));
  Serial.println(id);
  uint8_t result = driver.test_connection();
  delay(200);
  if (result) {
    Serial.println(F("failed!"));
    Serial.print(F("Likely cause: "));
    switch (result) {
      case 1: Serial.println(F("loose connection")); break;
      case 2: Serial.println(F("Likely cause: no power")); break;
    }
    Serial.println(F("Fix the problem and reset board."));
    // abort();
  }
  Serial.println(F("OK"));
}

void setupMotors() {

  //fill motor Pins to motor pins array
  motorPins[0].EN_PIN = EN_PIN_01;
  motorPins[0].DIR_PIN = DIR_PIN_01;
  motorPins[0].STEP_PIN = STEP_PIN_01;

  motorPins[1].EN_PIN = EN_PIN_02;
  motorPins[1].DIR_PIN = DIR_PIN_02;
  motorPins[1].STEP_PIN = STEP_PIN_02;

  motorPins[2].EN_PIN = EN_PIN_03;
  motorPins[2].DIR_PIN = DIR_PIN_03;
  motorPins[2].STEP_PIN = STEP_PIN_03;

  motorPins[3].EN_PIN = EN_PIN_04;
  motorPins[3].DIR_PIN = DIR_PIN_04;
  motorPins[3].STEP_PIN = STEP_PIN_04;

  motorPins[4].EN_PIN = EN_PIN_05;
  motorPins[4].DIR_PIN = DIR_PIN_05;
  motorPins[4].STEP_PIN = STEP_PIN_05;

  motorPins[5].EN_PIN = EN_PIN_06;
  motorPins[5].DIR_PIN = DIR_PIN_06;
  motorPins[5].STEP_PIN = STEP_PIN_06;

  motorPins[6].EN_PIN = EN_PIN_07;
  motorPins[6].DIR_PIN = DIR_PIN_07;
  motorPins[6].STEP_PIN = STEP_PIN_07;

  motorPins[7].EN_PIN = EN_PIN_08;
  motorPins[7].DIR_PIN = DIR_PIN_08;
  motorPins[7].STEP_PIN = STEP_PIN_08;

  for (int i = 0; i < 8; i++) {
    steppers[i] = AccelStepper(steppers[i].DRIVER, motorPins[i].STEP_PIN, motorPins[i].DIR_PIN);
  }

  for (int i = 0; i < 8; i++) {
    steppers[i].setMaxSpeed(50 * steps_per_mm); // 100mm/s @ 80 steps/mm
    steppers[i].setAcceleration(1000 * steps_per_mm); // 2000mm/s^2
    steppers[i].setEnablePin(sx, motorPins[i].EN_PIN);
    //steppers[i].setPinsInverted(false, false, true);
    steppers[i].enableOutputs(sx);
  }

}

void testConnection(int id) {
  int result = driver.test_connection();
  Serial.print(F("Conn: "));
  Serial.print(id);
  Serial.print(F(" "));
  Serial.println(result);
}


void testConnection() {
  int result = driver.test_connection();
  Serial.print(F("Con 0 "));
  Serial.println(result);
}

void enableMotor(int id) {
  if (id == 0) {
    sx.digitalWrite(selectMotor[0], LOW);
    sx.digitalWrite(selectMotor[1], LOW);
    sx.digitalWrite(selectMotor[2], LOW);
  } else if ( id == 1) {
    sx.digitalWrite(selectMotor[0], HIGH);
    sx.digitalWrite(selectMotor[1], LOW);
    sx.digitalWrite(selectMotor[2], LOW);
  } else if ( id == 2) {
    sx.digitalWrite(selectMotor[0], LOW);
    sx.digitalWrite(selectMotor[1], HIGH);
    sx.digitalWrite(selectMotor[2], LOW);
  } else if ( id == 3) {
    sx.digitalWrite(selectMotor[0], HIGH);
    sx.digitalWrite(selectMotor[1], HIGH);
    sx.digitalWrite(selectMotor[2], LOW);
  } else if ( id == 4) {
    sx.digitalWrite(selectMotor[0], LOW);
    sx.digitalWrite(selectMotor[1], LOW);
    sx.digitalWrite(selectMotor[2], HIGH);
  } else if ( id == 5) {
    sx.digitalWrite(selectMotor[0], HIGH);
    sx.digitalWrite(selectMotor[1], LOW);
    sx.digitalWrite(selectMotor[2], HIGH);
  } else if ( id == 6) {
    sx.digitalWrite(selectMotor[0], LOW);
    sx.digitalWrite(selectMotor[1], HIGH);
    sx.digitalWrite(selectMotor[2], HIGH);
  } else if ( id == 7) {
    sx.digitalWrite(selectMotor[0], HIGH);
    sx.digitalWrite(selectMotor[1], HIGH);
    sx.digitalWrite(selectMotor[2], HIGH);
  }
}

void setupSXPins() {

  //MOTOR enable
  sx.pinMode(EN_PIN_01, OUTPUT);
  sx.pinMode(EN_PIN_02, OUTPUT);
  sx.pinMode(EN_PIN_03, OUTPUT);
  sx.pinMode(EN_PIN_04, OUTPUT);
  sx.pinMode(EN_PIN_05, OUTPUT);
  sx.pinMode(EN_PIN_06, OUTPUT);
  sx.pinMode(EN_PIN_07, OUTPUT);
  sx.pinMode(EN_PIN_08, OUTPUT);

  sx.digitalWrite(EN_PIN_01, LOW);
  sx.digitalWrite(EN_PIN_02, LOW);
  sx.digitalWrite(EN_PIN_03, LOW);
  sx.digitalWrite(EN_PIN_04, LOW);
  sx.digitalWrite(EN_PIN_05, LOW);
  sx.digitalWrite(EN_PIN_06, LOW);
  sx.digitalWrite(EN_PIN_07, LOW);
  sx.digitalWrite(EN_PIN_08, LOW);

  //MUX
  sx.pinMode(selectMotor[0], OUTPUT);
  sx.pinMode(selectMotor[1], OUTPUT);
  sx.pinMode(selectMotor[2], OUTPUT);

  //LED
  sx.pinMode(selectLED[0], OUTPUT);
  sx.pinMode(selectLED[1], OUTPUT);
  sx.pinMode(selectLED[2], OUTPUT);

  //MUX
  for (int i = 0; i < 3; i++) {
    sx.digitalWrite(selectMotor[i], LOW);
    sx.digitalWrite(selectLED[i], LOW);
  }
}

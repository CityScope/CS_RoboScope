
#include <TMCStepper.h>

#define EN_PIN_01           20 // Enable //14 17
#define DIR_PIN_01          22 // Direction //16 19
#define STEP_PIN_01         21 // Step . //15 18

#define EN_PIN_02           17 // Enable //14 17
#define DIR_PIN_02          19 // Direction //16 19
#define STEP_PIN_02         18 // Step . //15 18

#define EN_PIN_03           14 // Enable //14 17
#define DIR_PIN_03          16 // Direction //16 19
#define STEP_PIN_03         15 // Step . //15 18

#define SERIAL_PORT Serial2
#define DRIVER_ADDRESS 0b00

#define R_SENSE 0.11f

TMC2209Stepper driver(&SERIAL_PORT, R_SENSE, DRIVER_ADDRESS);

//mux
const int selectPins[3] = {5, 6, 7};

void setup() {
  pinMode(EN_PIN_01, OUTPUT);
  pinMode(STEP_PIN_01, OUTPUT);
  pinMode(DIR_PIN_01, OUTPUT);
  digitalWrite(EN_PIN_01, LOW);      // Enable driver in hardware

  pinMode(EN_PIN_02, OUTPUT);
  pinMode(STEP_PIN_02, OUTPUT);
  pinMode(DIR_PIN_02, OUTPUT);
  digitalWrite(EN_PIN_02, LOW);

  pinMode(EN_PIN_03, OUTPUT);
  pinMode(STEP_PIN_03, OUTPUT);
  pinMode(DIR_PIN_03, OUTPUT);
  digitalWrite(EN_PIN_03, LOW);

  for (int i = 0; i < 3; i++)
  {
    pinMode(selectPins[i], OUTPUT);
    digitalWrite(selectPins[i], LOW);
  }

  //SPI.begin();                    // SPI drivers
  SERIAL_PORT.begin(115200);      // HW UART drivers
  // driver.beginSerial(115200);     // SW UART drivers

  driver.begin();                 //  SPI: Init CS pins and possible SW SPI pins
  // UART: Init SW UART (if selected) with default 115200 baudrate
  driver.toff(5);                 // Enables driver in software
  driver.rms_current(300);        // Set motor RMS current
  driver.microsteps(32);          // Set microsteps to 1/16th

  //driver.en_pwm_mode(true);       // Toggle stealthChop on TMC2130/2160/5130/5160
  driver.en_spreadCycle(false);   // Toggle spreadCycle on TMC2208/2209/2224
  driver.pwm_autoscale(true);     // Needed for stealthChop



  digitalWrite(selectPins[0], HIGH);
  digitalWrite(selectPins[1], LOW);
  digitalWrite(selectPins[2], HIGH);

  driver.rms_current(300);        // Set motor RMS current
  driver.microsteps(32);          // Set microsteps to 1/16th

  //driver.en_pwm_mode(true);       // Toggle stealthChop on TMC2130/2160/5130/5160
  driver.en_spreadCycle(false);   // Toggle spreadCycle on TMC2208/2209/2224
  driver.pwm_autoscale(true);     // Needed for stealthChop


  digitalWrite(selectPins[0], HIGH);
  digitalWrite(selectPins[1], HIGH);
  digitalWrite(selectPins[2], HIGH);

  driver.rms_current(300);        // Set motor RMS current
  driver.microsteps(32);          // Set microsteps to 1/16th

  //driver.en_pwm_mode(true);       // Toggle stealthChop on TMC2130/2160/5130/5160
  driver.en_spreadCycle(false);   // Toggle spreadCycle on TMC2208/2209/2224
  driver.pwm_autoscale(true);     // Needed for stealthChop

}

bool shaft0 = true;
bool shaft1 = false;
bool shaft2 = true;
void loop() {
  // put your main code here, to run repeatedly:
  // Run 5000 steps and switch direction in software
  for (uint16_t i = 50000; i > 0; i--) {
    digitalWrite(STEP_PIN_01, HIGH);
    digitalWrite(STEP_PIN_02, HIGH);
    digitalWrite(STEP_PIN_03, HIGH);
    delayMicroseconds(160);
    digitalWrite(STEP_PIN_01, LOW);
    digitalWrite(STEP_PIN_02, LOW);
    digitalWrite(STEP_PIN_03, LOW);
    delayMicroseconds(160);
  }

  shaft0 = !shaft0;
  digitalWrite(selectPins[0], LOW);
  digitalWrite(selectPins[1], LOW);
  digitalWrite(selectPins[2], LOW);
  driver.shaft(shaft0);

  shaft1 = !shaft1;
  digitalWrite(selectPins[0], HIGH);
  digitalWrite(selectPins[1], LOW);
  digitalWrite(selectPins[2], HIGH);
  driver.shaft(shaft1);

  shaft2 = !shaft2;
  digitalWrite(selectPins[0], HIGH);
  digitalWrite(selectPins[1], HIGH);
  digitalWrite(selectPins[2], HIGH);
  driver.shaft(shaft2);


  //


}

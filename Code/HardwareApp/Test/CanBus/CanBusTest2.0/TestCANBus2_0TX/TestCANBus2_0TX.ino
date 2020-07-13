#include <FlexCAN.h>

/*
  https://github.com/collin80/FlexCAN_Library

*/

FlexCAN CANbus(1000000);
static CAN_message_t msg, rxmsg;

#define LED_PIN_01 15
#define LED_PIN_02 16

#define CAN_STBY  5

#define KEY_PIN_01 20
#define KEY_PIN_02 21
#define KEY_PIN_03 22
#define KEY_PIN_04 23


void setup() {


  CANbus.begin();
  delay(2000);

  Serial.begin(9600);
  Serial.println(F("Starting Sending"));
  delay(2000);

  Serial.begin(9600);

  //LED
  pinMode(LED_PIN_01, OUTPUT);
  pinMode(LED_PIN_02, OUTPUT);

  pinMode(KEY_PIN_01, INPUT);
  pinMode(KEY_PIN_02, INPUT);
  pinMode(KEY_PIN_03, INPUT);
  pinMode(KEY_PIN_04, INPUT);

  pinMode(CAN_STBY, OUTPUT);

  digitalWrite(LED_PIN_01, HIGH);
  digitalWrite(LED_PIN_02, HIGH);
  delay(2000);

  digitalWrite(LED_PIN_01, LOW);
  digitalWrite(LED_PIN_02, LOW);


  digitalWrite(CAN_STBY, LOW);

  msg.len = 8;
  msg.id = 0x222;
  for ( int idx = 0; idx < 8; ++idx ) {
    msg.buf[idx] = '0' + idx;
  }
}


void loop() {
  digitalWrite(CAN_STBY, LOW);
  // put your main code here, to run repeatedly:
  int pinkey01 = digitalRead(KEY_PIN_01);
  int pinkey02 = digitalRead(KEY_PIN_02);
  int pinkey03 = digitalRead(KEY_PIN_03);
  int pinkey04 = digitalRead(KEY_PIN_04);

  digitalWrite(LED_PIN_01, pinkey01);
  digitalWrite(LED_PIN_01, pinkey02);

  digitalWrite(LED_PIN_02, pinkey03);
  digitalWrite(LED_PIN_02, pinkey04);

  int x = CANbus.write(msg);
  Serial.write(x + 48);

  delay(1000);

}

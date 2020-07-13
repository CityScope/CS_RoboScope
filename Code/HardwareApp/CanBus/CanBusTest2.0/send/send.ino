
#include <FlexCAN.h>


#define LED_PIN_01 15
#define LED_PIN_02 16


#define KEY_PIN_01 20
#define KEY_PIN_02 21
#define KEY_PIN_03 22
#define KEY_PIN_04 23


FlexCAN CANbus(1000000);

static CAN_message_t msg, rxmsg;

// -------------------------------------------------------------
static void hexDump(uint8_t dumpLen, uint8_t *bytePtr)
{
  uint8_t hex[17] = "0123456789abcdef";
  uint8_t working;
  while (dumpLen--) {
    working = *bytePtr++;
    Serial.write(hex[working >> 4]);
    Serial.write(hex[working & 15]);
  }
  Serial.write('\r');
  Serial.write('\n');
}

// -------------------------------------------------------------
void setup(void)
{
  Serial.begin(9600);
  Serial.println(F("Starting Reading"));
  delay(2000);

  pinMode(LED_PIN_01, OUTPUT);
  pinMode(LED_PIN_02, OUTPUT);

  pinMode(KEY_PIN_01, INPUT);
  pinMode(KEY_PIN_02, INPUT);
  pinMode(KEY_PIN_03, INPUT);
  pinMode(KEY_PIN_04, INPUT);

  CANbus.begin();
  Serial.println(F("Starting Sending"));
}

// -------------------------------------------------------------
void loop(void)
{

  int pinkey01 = digitalRead(KEY_PIN_01);
  int pinkey02 = digitalRead(KEY_PIN_02);
  int pinkey03 = digitalRead(KEY_PIN_03);
  int pinkey04 = digitalRead(KEY_PIN_04);

  digitalWrite(LED_PIN_01, pinkey01);
  digitalWrite(LED_PIN_01, pinkey02);

  digitalWrite(LED_PIN_02, pinkey03);
  digitalWrite(LED_PIN_02, pinkey04);

  msg.len = 8;
  msg.id = 0x222;

  for ( int idx = 0; idx < 8; ++idx ) {
    msg.buf[idx] = 0;
  }

  msg.buf[0] = char(pinkey01);
  msg.buf[1] = char(pinkey02);
  msg.buf[2] = char(pinkey03);
  msg.buf[3] = char(pinkey04);

  CANbus.write(msg);
}

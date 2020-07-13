#include <Metro.h>
#include <FlexCAN.h>

Metro sysTimer = Metro(1);// milliseconds
FlexCAN CANbus(1000000);

CAN_message_t rxmsg;
static uint8_t hex[17] = "0123456789abcdef";

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
  Serial.println(F("Starting Reading"));
  delay(2000);

  //LED
  pinMode(LED_PIN_01, OUTPUT);
  pinMode(LED_PIN_02, OUTPUT);

  pinMode(CAN_STBY, OUTPUT);

  pinMode(KEY_PIN_01, INPUT);
  pinMode(KEY_PIN_02, INPUT);
  pinMode(KEY_PIN_03, INPUT);
  pinMode(KEY_PIN_04, INPUT);

  digitalWrite(LED_PIN_01, HIGH);
  digitalWrite(LED_PIN_02, HIGH);
  delay(2000);

  digitalWrite(LED_PIN_01, LOW);
  digitalWrite(LED_PIN_02, LOW);

  digitalWrite(CAN_STBY, LOW);
}

static void hexDump(uint8_t dumpLen, uint8_t *bytePtr)
{
  uint8_t working;
  while ( dumpLen-- ) {
    working = *bytePtr++;
    Serial.write( hex[ working >> 4 ] );
    Serial.write( hex[ working & 15 ] );
  }
  Serial.write('\r');
  Serial.write('\n');
}

void loop() {
  // put your main code here, to run repeatedly:

  digitalWrite(CAN_STBY, LOW);

  int pinkey01 = digitalRead(KEY_PIN_01);
  int pinkey02 = digitalRead(KEY_PIN_02);
  int pinkey03 = digitalRead(KEY_PIN_03);
  int pinkey04 = digitalRead(KEY_PIN_04);


  digitalWrite(LED_PIN_01, pinkey01);
  digitalWrite(LED_PIN_01, pinkey02);

  digitalWrite(LED_PIN_02, pinkey03);
  digitalWrite(LED_PIN_02, pinkey04);


  CAN_message_t inMsg;
  while (CANbus.available())
  {
    CANbus.read(inMsg);
    Serial.print("CAN bus 0: "); hexDump(8, inMsg.buf);
  }
}

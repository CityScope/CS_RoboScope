
#include <FlexCAN.h>


#define LED_PIN_01 15
#define LED_PIN_02 16

#define KEY_PIN_01 20
#define KEY_PIN_02 21
#define KEY_PIN_03 22
#define KEY_PIN_04 23

//key
int pinkey01Cur = 0;
int pinkey01Prev = 0;

int pinkey02Cur = 0;
int pinkey02Prev = 0;

int pinkey03Cur = 0;
int pinkey03Prev = 0;

int pinkey04Cur = 0;
int pinkey04Prev = 0;

FlexCAN CANbus(1000000);

static CAN_message_t msg;
int changeMSg = true;

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

  pinkey01Prev = pinkey01Cur;
  pinkey02Prev = pinkey02Cur;
  pinkey03Prev = pinkey03Cur;
  pinkey04Prev = pinkey04Cur;

  pinkey01Cur = digitalRead(KEY_PIN_01);
  pinkey02Cur = digitalRead(KEY_PIN_02);
  pinkey03Cur = digitalRead(KEY_PIN_03);
  pinkey04Cur = digitalRead(KEY_PIN_04);


  //rest
  msg.len = 8;
  msg.id = 0x222;
  changeMSg = false;

  for ( int idx = 0; idx < 8; ++idx ) {
    msg.buf[idx] = 0;
  }

  //if thre is a change send a msg
  if (pinkey01Cur != pinkey01Prev) {
    changeMSg = true;
    msg.buf[0] = char(pinkey01Cur);
    Serial.println("change key 1");
  }

  if (pinkey02Cur != pinkey02Prev) {
    changeMSg = true;
    msg.buf[1] = char(pinkey02Cur);
    Serial.println("change key 2");
  }

  if (pinkey03Cur != pinkey03Prev) {
    changeMSg = true;
    msg.buf[2] = char(pinkey03Cur);
    Serial.println("change key 3");
  }

  if (pinkey04Cur != pinkey04Prev) {
    changeMSg = true;
    msg.buf[3] = char(pinkey04Cur);
    Serial.println("change key 4");
  }

  //change msg
  if (changeMSg) {
    CANbus.write(msg);
    Serial.println("Sent");
  }

  
    if (!changeMSg) {
    //RX message
    CAN_message_t rxMsg;
    while (CANbus.available()) {

      CANbus.read(rxMsg);
      Serial.print("CAN BUS: ");
      hexDump(8, rxMsg.buf);

      digitalWrite(LED_PIN_01, int(rxMsg.buf[0]));
      digitalWrite(LED_PIN_01, int(rxMsg.buf[1]));
      digitalWrite(LED_PIN_02, int(rxMsg.buf[2]));
      digitalWrite(LED_PIN_02, int(rxMsg.buf[3]));
    }
    }
 

}

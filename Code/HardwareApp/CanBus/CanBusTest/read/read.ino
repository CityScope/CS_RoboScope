#include <FlexCAN.h>

#define LED_PIN_01 15
#define LED_PIN_02 16


#define KEY_PIN_01 20
#define KEY_PIN_02 21
#define KEY_PIN_03 22
#define KEY_PIN_04 23

FlexCAN CANbus(1000000);

// -------------------------------------------------------------
static int hexDump(uint8_t dumpLen, uint8_t *bytePtr)
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
  CANbus.begin();
  Serial.begin(9600);
  Serial.println(F("Starting Reading"));
  delay(2000);

  pinMode(LED_PIN_01, OUTPUT);
  pinMode(LED_PIN_02, OUTPUT);

  pinMode(KEY_PIN_01, INPUT);
  pinMode(KEY_PIN_02, INPUT);
  pinMode(KEY_PIN_03, INPUT);
  pinMode(KEY_PIN_04, INPUT);

}

// -------------------------------------------------------------
void loop(void)
{
  CAN_message_t rxMsg;
  while (CANbus.available()){
    
    CANbus.read(rxMsg);
    Serial.print("CAN BUS: ");
    hexDump(8, rxMsg.buf);

    digitalWrite(LED_PIN_01, int(rxMsg.buf[0]));
    digitalWrite(LED_PIN_01, int(rxMsg.buf[1]));
    
    digitalWrite(LED_PIN_02, int(rxMsg.buf[2]));
    digitalWrite(LED_PIN_02, int(rxMsg.buf[3]));
    
  }
}

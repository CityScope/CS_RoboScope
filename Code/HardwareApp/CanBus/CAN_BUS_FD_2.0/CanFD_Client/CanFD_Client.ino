/*
   Can Bus Client
   Sending Can Bus FD
   Sending Can TD 0
*/
#include <FlexCAN_T4.h>
#include "SendMsg.h"
#include "message.h"


//CAN BUS FD
FlexCAN_T4FD<CAN3, RX_SIZE_256, TX_SIZE_64>   FD;   // fd port

//CAN BUS 1
FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16>     canBus1;  // can1 port

int local_name = 0;
int msg_array[8] = {1, 2, 3, 4, 32, 6, 7, 8};
int state;
uint32_t timer;

//Key pins
const int KEY_PIN_01 = 6;
const int KEY_PIN_02 = 7;
const int KEY_PIN_03 = 8;
const int KEY_PIN_04 = 9;

const int LED_15 = 22;
const int LED_16 = 23;

//----------------------------------------------------------------
void setup(void) {
  Serial.begin(115200);
  delay(500);
  Serial.println("Client CAN Bus FD and CAN BUS 1");

  //setup CAN 0 BUS
  canBus1.begin();
  canBus1.setBaudRate(500000);     // 500kbps data rate
  canBus1.enableFIFO();
  canBus1.enableFIFOInterrupt();
  canBus1.onReceive(FIFO, canSniff);
  canBus1.mailboxStatus();

  //setup CAN FD bus
  FD.begin();
  CANFD_timings_t config;
  config.clock = CLK_24MHz;
  config.baudrate = 1000000;
  config.baudrateFD = 2000000;
  config.propdelay = 190;
  config.bus_length = 1;
  config.sample = 87.5;
  FD.setRegions(64);
  FD.setBaudRate(config);
  FD.mailboxStatus();

  //Key values
  pinMode(KEY_PIN_01, INPUT);
  pinMode(KEY_PIN_02, INPUT);
  pinMode(KEY_PIN_03, INPUT);
  pinMode(KEY_PIN_04, INPUT);

  //turn on LEDs
  pinMode(LED_15, OUTPUT);
  pinMode(LED_16, OUTPUT);

  digitalWrite(LED_15, HIGH); //CAN Bus FD
  digitalWrite(LED_16, HIGH); //CAN Bus 2.0
  delay(1000);

  digitalWrite(LED_15, LOW);
  digitalWrite(LED_16, LOW);
  
  state = HIGH;
  timer = millis();

  Serial.println("Init client done");
}

//----------------------------------------------------------------
void loop() {
  CANFD_message_t msg;
  if (FD.read(msg)) {
    Serial.println("reading");
    reading(msg);
  }
}

//----------------------------------------------------------------
void canBusSniff(const CAN_message_t &msg) { // global callback
  digitalWrite(LED_15, HIGH); 
  
  Serial.print("T4: ");
  Serial.print("MB "); Serial.print(msg.mb);
  Serial.print(" OVERRUN: "); Serial.print(msg.flags.overrun);
  Serial.print(" BUS "); Serial.print(msg.bus);
  Serial.print(" LEN: "); Serial.print(msg.len);
  Serial.print(" EXT: "); Serial.print(msg.flags.extended);
  Serial.print(" REMOTE: "); Serial.print(msg.flags.remote);
  Serial.print(" TS: "); Serial.print(msg.timestamp);
  Serial.print(" ID: "); Serial.print(msg.id, HEX);
  Serial.print(" IDHIT: "); Serial.print(msg.idhit);
  
  Serial.print(" Buffer: ");
  for ( uint8_t i = 0; i < msg.len; i++ ) {
    Serial.print(msg.buf[i], HEX); Serial.print(" ");
  } Serial.println();

  digitalWrite(LED_15, LOW); //CAN Bus FD
  
}

//----------------------------------------------------------------
void reading(CANFD_message_t msg) {
  if (msg.id <= local_name && msg.id + 16 > local_name) {

    //turn on LED
    digitalWrite(LED_15, HIGH);
    
    //print CAN BUS data
    Serial.print("  LEN: ");
    Serial.print(msg.len);
    Serial.print(" DATA: ");

    for ( uint8_t i = local_name * 4; i < local_name * 4 + 4; i++ ) {
      Serial.print(msg.buf[i]); 
      Serial.print(" ");
    }
    Serial.print("  TS: "); 
    Serial.println(msg.timestamp);

    //turn off LED
    digitalWrite(LED_15, LOW);
    
  }
}

void send

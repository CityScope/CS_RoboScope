#include <FlexCAN_T4.h>
#include <string.h>
#include "message.h"
#include <Bounce2.h>
#include <Keyboard.h>

#define NUM_PIXELS 8
#define MSG_SIZE 64
#define NUM_PANELS 12
#define  STATUS_PIN_SX03 5 

FlexCAN_T4FD<CAN3, RX_SIZE_256, TX_SIZE_32> canBusFD;

//key pins
const int KEY_PIN_01 = 6;
const int KEY_PIN_02 = 7;
const int KEY_PIN_03 = 8;
const int KEY_PIN_04 = 9;

const int LED_15 = 22;
const int LED_16 = 23;

Bounce2::Button buttonKey_01 = Bounce2::Button();
Bounce2::Button buttonKey_02 = Bounce2::Button();
Bounce2::Button buttonKey_03 = Bounce2::Button();
Bounce2::Button buttonKey_04 = Bounce2::Button();

byte tempBuffer[768];  // temp buffer
byte fullBoard[768];   // real buffer
int nodeId = 0;
int pixelId = 0;
int numsTimes = 0;

struct Pixel {
  byte b1;
  byte b2;
  byte motorByte;
  byte modeByte;
  byte density;
  byte colorR;
  byte colorG;
  byte colorB;
};

void setup() {
  Serial.begin(115200);
  establishContact();  // Send byte to establish contact until receiver responds
  pinMode(LED_15, OUTPUT);


  canBusFD.begin();
  CANFD_timings_t config;
  config.clock = CLK_40MHz;  //40 CLK_80MHz
  config.baudrate = 1000000;
  config.baudrateFD = 8000000;
  config.propdelay = 190;
  config.bus_length = 1;
  config.sample = 87.5;
  canBusFD.setRegions(64);
  canBusFD.setBaudRate(config);
  canBusFD.mailboxStatus();

  buttonKey_01.attach(KEY_PIN_01, INPUT_PULLUP);
  buttonKey_02.attach(KEY_PIN_02, INPUT_PULLUP);
  buttonKey_03.attach(KEY_PIN_03, INPUT_PULLUP);
  buttonKey_04.attach(KEY_PIN_04, INPUT_PULLUP);

  // DEBOUNCE INTERVAL IN MILLISECONDS
  buttonKey_01.interval(20);
  buttonKey_02.interval(20);
  buttonKey_03.interval(20);
  buttonKey_04.interval(20);

  // INDICATE THAT THE LOW STATE CORRESPONDS TO PHYSICALLY PRESSING THE BUTTON
  buttonKey_01.setPressedState(LOW);
  buttonKey_02.setPressedState(LOW);
  buttonKey_03.setPressedState(LOW);
  buttonKey_04.setPressedState(LOW);

  Serial.println("Server initialization complete.");
}


void loop() {
  //key
  buttonKey_01.update();
  buttonKey_02.update();
  buttonKey_03.update();
  buttonKey_04.update();

  //key
  int keyPin01 = buttonKey_01.pressed();
  int keyPin02 = buttonKey_02.pressed();
  int keyPin03 = buttonKey_03.pressed();
  int keyPin04 = buttonKey_04.pressed();

  if (Serial.available() > 1) {                   // # of bytes available in the serial port
    int len = Serial.readBytes(tempBuffer, 768);  // if there are bytes avilable to read, read them and store in tempBuffer.
    for (int i = 0; i < 768; i++) {
      if (tempBuffer[i] != fullBoard[i]) {
        memcpy(fullBoard, tempBuffer, 768);
        break;
      }
    }
    
    for (int j = 0; j < 12; j++) {
      sendNode(j, fullBoard);
      Serial.println("Node sent");
    }
    numsTimes++;
  }
  else {
    reset(nodeId, pixelId);
  }
}


//resets node and pixel values after going through one iteration of reading.
void reset(int nodeId, int pixelId) {
  nodeId = 0;
  pixelId = 0;
}

void establishContact() {
  while (Serial.available() <= 0) {
    //Serial.write("Good"); // Prints Good to the serial port.
    delay(1000);
  }
}

// Prints current buffer (one node) to serial.
void CANPrint(const CANFD_message_t &msg) {
  Serial.print("Buffer: "); 
  for (uint8_t i = 0; i < msg.len; i++) {
    Serial.print(msg.buf[i], DEC); Serial.print(" ") ;
  }
  Serial.println();
}

void sendNode(int nodeId, byte* buffer) {
  CANMotorMessage msg = CANMotorMessage(nodeId);
  for (int i = 0; i < NUM_PIXELS; i++) {
    msg.addMessage(i, 0, 0, fullBoard[2 + 64 * nodeId + 8 * i], fullBoard[3 + 64 * nodeId + 8 * i], fullBoard[4 + 64 * nodeId + 8 * i], fullBoard[5 + 64 * nodeId + 8 * i], fullBoard[6 + 64 * nodeId + 8 * i], fullBoard[7 + 64 * nodeId + 8 * i]);
  }

  canBusFD.write(msg.getCANmessage(MSG_SIZE));
  delay(1);
  canBusFD.write(msg.getCANmessage(MSG_SIZE));

}
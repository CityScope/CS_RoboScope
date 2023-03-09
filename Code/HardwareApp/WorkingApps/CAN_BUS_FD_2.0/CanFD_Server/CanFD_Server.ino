#include <FlexCAN_T4.h>
#include <string.h>
#include "message.h"
#include <Bounce2.h>

//NUM 3D PIXELS
#define NUM_3D_PIXELS 8
#define BOARD_ID 0
#define MSG_SIZE 32

FlexCAN_T4FD<CAN3, RX_SIZE_256, TX_SIZE_32> canBusFD;

//key pins
const int KEY_PIN_01 = 6;
const int KEY_PIN_02 = 7;
const int KEY_PIN_03 = 8;
const int KEY_PIN_04 = 9;

const int LED_15 = 22;
const int LED_16 = 23;

char operation;                 // Holds operation (R, W, ...)
int wait_for_transmission = 5;  // Delay in ms in order to receive the serial data

Button buttonKey_01 = Button();
Button buttonKey_02 = Button();
Button buttonKey_03 = Button();
Button buttonKey_04 = Button();

//format for received pixel data
struct Pixel {
  byte inter;
  byte height;
  byte debu;
    byte color0;
  byte color1;
};

//----------------------------------------------------------------
void setup(void) {
  Serial.begin(115200);
  delay(500);

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
  buttonKey_01.interval(50);
  buttonKey_02.interval(50);
  buttonKey_03.interval(50);
  buttonKey_04.interval(50);

  // INDICATE THAT THE LOW STATE CORRESPONDS TO PHYSICALLY PRESSING THE BUTTON
  buttonKey_01.setPressedState(LOW);
  buttonKey_02.setPressedState(LOW);
  buttonKey_03.setPressedState(LOW);
  buttonKey_04.setPressedState(LOW);

  //
  pinMode(LED_15, OUTPUT);
  pinMode(LED_16, OUTPUT);

  Serial.println("Init server done");
}

//----------------------------------------------------------------
//command
//W{node_id},{local_id},{interaction},{height},{color1},{color2}E

void loop() {
  ///Key
  buttonKey_01.update();
  buttonKey_02.update();
  buttonKey_03.update();
  buttonKey_04.update();

  //key
  int keyPin01 = buttonKey_01.pressed();
  int keyPin02 = buttonKey_02.pressed();
  int keyPin03 = buttonKey_03.pressed();
  int keyPin04 = buttonKey_04.pressed();

  digitalWrite(LED_15, keyPin01);
  digitalWrite(LED_16, keyPin04);

  if (Serial.available() > 0) {
    operation = Serial.read();
    if (operation == 'W') {
      process_node();
    } else if (operation == 'T') {
      int len = Serial.parseInt();
      Serial.read();
      for (int k = 0; k < len; k++) {
        process_node();
      }
      Serial.read();
    }
  }

  if (keyPin01 == HIGH) {
    Serial.println("send key 01");
    int nodeId = 0;
    Pixel buf[NUM_3D_PIXELS];
    for (int i = 0; i < NUM_3D_PIXELS; i++) {
      uint8_t interaction = (uint8_t)random(0, 255);
      uint8_t height = (uint8_t)random(255);
      uint8_t color0 = (uint8_t)random(255);
      uint8_t color1 = (uint8_t)random(255);
      uint8_t debug = 0;
      buf[i] = { interaction, height, color0, color1 };
      delay(5);
    }
    translate_pixels(nodeId, buf);
  }

  if (keyPin02 == HIGH) {
    Serial.println("send key 02");
    delay(10);

    //white
    int nodeId = 1;
    test_white(nodeId);

    nodeId = 2;
    test_white(nodeId);
  }

  if (keyPin03 == HIGH) {
    Serial.println("send key 03");
    for (int i = 1; i <= 2; i++) {
      int nodeId = i;
      Pixel buf[NUM_3D_PIXELS];
      for (int j = 0; j < NUM_3D_PIXELS; j++) {
        uint8_t interaction = (uint8_t)random(0, 255);
        uint8_t height = (uint8_t)random(255);
        uint8_t color0 = (uint8_t)random(255);
        uint8_t color1 = (uint8_t)random(255);
        buf[j] = { interaction, height, color0, color1 };
        delay(5);
      }
      translate_pixels(nodeId, buf);
      delay(10);
    }
  }

  if (keyPin04 == HIGH) {
    Serial.println("send key 04");
    delay(10);


    int nodeId = BOARD_ID;
    test_blue(nodeId);

    nodeId = BOARD_ID;
    test_blue(nodeId);
  }
}

//----------------------------------------------------------------
//use this to process an entire node's information coming from Serial
void process_node() {
  Pixel buf[8];
  int node_id = Serial.parseInt();
  Serial.read();

  for (int j = 0; j < NUM_3D_PIXELS; j++) {
    byte temp[4] = { 0 };
    for (int i = 0; i < 4; i++) {
      temp[i] = Serial.parseInt();
      Serial.read();
    }
    buf[j] = { temp[0], temp[1], temp[2], temp[3] };
  }
  translate_pixels(node_id, buf);
}

void process_nodeBytes() {
  Pixel buf[8];
  int node_id = Serial.parseInt();
  Serial.read();


  //temp[i] = Serial.parseInt();

  for (int j = 0; j < NUM_3D_PIXELS; j++) {
    byte temp[4] = { 0 };
    for (int i = 0; i < 4; i++) {
      temp[i] = Serial.parseInt();
      Serial.read();
    }
    buf[j] = { temp[0], temp[1], temp[2], temp[3] };
  }
  translate_pixels(node_id, buf);
}

//use this to send data to the table
void translate_pixels(int node_id, Pixel* buf) {
  CANMotorMessage msg = CANMotorMessage(node_id);
  for (int i = 0; i < NUM_3D_PIXELS; i++) {
    msg.addMessage(i, buf[i].inter, buf[i].height, buf[i].color0, buf[i].color1);
  }
  canBusFD.write(msg.getCANmessage(MSG_SIZE));
}

//----------------------------------------------------------------
void test_white(int node_id) {
  CANMotorMessage msg = CANMotorMessage(node_id);
  for (int i = 0; i < NUM_3D_PIXELS; i++) {
    msg.addMessage(i, 0, 200, 255, 255);
  }
  canBusFD.write(msg.getCANmessage(MSG_SIZE));
}
void test_blue(int node_id) {
  CANMotorMessage msg = CANMotorMessage(node_id);
  for (int i = 0; i < NUM_3D_PIXELS; i++) {
    msg.addMessage(i, 0, 50, 0, 255);
  }
  canBusFD.write(msg.getCANmessage(MSG_SIZE));
}

//----------------------------------------------------------------

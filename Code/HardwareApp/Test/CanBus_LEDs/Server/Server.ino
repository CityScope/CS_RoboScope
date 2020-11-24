#include <FlexCAN_T4.h>
#include <string.h>
#include "message.h"
/* CANBUS_LED Test
 * Serial input: 
 * - a: turn neopixels white
 * - b: turn neopixels blue
 * - c,{int 0-255},{int 0-255}: turn neopixels into given rgb565 color
 * - Example Interface inputs to use:
 *   - W0,0,20,224,195,1,8,27,214,2,20,224,195,3,8,27,214,4,20,224,195,5,20,224,195,6,8,105,243,7,8,105,243E
 *   - W0,0,8,252,211,1,20,224,195,2,8,27,214,3,8,27,214,4,8,27,214,5,8,27,214,6,8,105,243,7,8,105,243E
 */
 
FlexCAN_T4FD<CAN3, RX_SIZE_512, TX_SIZE_512> canBusFD;

//key pins
const int KEY_PIN_01 = 6;
const int KEY_PIN_02 = 7;
const int KEY_PIN_03 = 8;
const int KEY_PIN_04 = 9;

const int LED_15 = 22;
const int LED_16 = 23;

char operation; // Holds operation (R, W, ...)
int wait_for_transmission = 5; // Delay in ms in order to receive the serial data
long primary_timer;
long initial_timer;
int node = 0;
int local = 0;

//format for received pixel data
struct Pixel {
  byte inter;
  byte height;
  byte color1;
  byte color2;
};

//----------------------------------------------------------------
void setup(void) {
  Serial.begin(2000000);
  delay(500);

  canBusFD.begin();
  CANFD_timings_t config;
  config.clock = CLK_40MHz;
  config.baudrate = 1000000;
  config.baudrateFD = 8000000;
  config.propdelay = 190;
  config.bus_length = 1;
  config.sample = 87.5;
  canBusFD.setRegions(64);
  canBusFD.setBaudRate(config);
  canBusFD.mailboxStatus();

  pinMode(KEY_PIN_01, INPUT);
  pinMode(KEY_PIN_02, INPUT);
  pinMode(KEY_PIN_03, INPUT);
  pinMode(KEY_PIN_04, INPUT);

  pinMode(LED_15, OUTPUT);
  pinMode(LED_16, OUTPUT);

  Serial.println("Init server done");
  primary_timer = millis();
  initial_timer = millis();
}

//----------------------------------------------------------------
void loop() {
  digitalWrite(LED_15, digitalRead(KEY_PIN_01));
  digitalWrite(LED_16, digitalRead(KEY_PIN_04));

  if (Serial.available() > 0) {
    operation = Serial.read();
    Serial.println(operation);
    if (operation == 'W') {
      process_node();
    } else if (operation == 'T') {
      int len = Serial.parseInt();
      Serial.read();
      for (int k = 0; k < len; k++) {
        process_node();
      }
      Serial.read();
    } else if (operation == 'a') {
      node = Serial.parseInt();
      //turn neopixels white
      test_white(node);
    } else if (operation == 'b') {
      node = Serial.parseInt();
      //turn neopixels white
      test_blue(node);
    } else if (operation == 'c') {
      //turn neopixels white
      Serial.read();
      int f = Serial.parseInt();
      Serial.read();
      int s = Serial.parseInt();
      Serial.read();
      node = Serial.parseInt();
      test_color(f,s,node);
    }
  }

  CANFD_message_t msg;
  if (canBusFD.read(msg)) {
    send_pixels(msg);
  }
}

//----------------------------------------------------------------
//use this to process an entire node's information coming from Serial
void process_node() {
  Pixel buf[8];
  node = Serial.parseInt();
  Serial.read();

  for (int j = 0; j < 8; j++) {
    byte temp[4] = {0};
    for (int i = 0; i < 4; i++) {
      temp[i] = Serial.parseInt();
      Serial.read();
    }
    buf[j] = {temp[0], temp[1], temp[2], temp[3]};
  }
  translate_pixels(node, buf);
}

//----------------------------------------------------------------
//use this to send data to the table
void translate_pixels(int node, Pixel* buf) {
  CANMotorMessage msg = CANMotorMessage(node);
  for (int i = 0; i < 8; i++) {
    msg.addMessage(i, buf[i].height, buf[i].inter, buf[i].color1, buf[i].color2);
  }
  canBusFD.write(msg.getCANmessage());
}

//----------------------------------------------------------------
//use this to send data to interface
void send_pixels(CANFD_message_t msg) {
  char str[256] = {0};
  char temp[5];
  sprintf(temp, "%d", msg.id);
  memcpy(str, temp, strlen(temp));
  memset (temp, 0, sizeof(temp));

  for ( uint8_t i = 0; i < msg.len; i++ ) {
    sprintf(temp, ",%d", msg.buf[i]);
    memcpy(str + strlen(str), temp, strlen(temp));
    memset (temp, 0, sizeof(temp));
  }
  Serial.println(str);
}

//----------------------------------------------------------------
void test_white(int node) {
  CANMotorMessage msg = CANMotorMessage(node);
  for (int i =0; i < 8; i++) {
    msg.addMessage(i, 10, i, 255, 255);
  }
  canBusFD.write(msg.getCANmessage());
}

//----------------------------------------------------------------
void test_blue(int node) {
  CANMotorMessage msg = CANMotorMessage(node);
  for (int i =0; i < 8; i++) {
    msg.addMessage(i, 10, i, 251, 10);
  }
  canBusFD.write(msg.getCANmessage());
}

//----------------------------------------------------------------
void test_color(int f, int s, int node) {
  CANMotorMessage msg = CANMotorMessage(node);
  for (int i =0; i < 8; i++) {
    msg.addMessage(i, 10, i, f, s);
  }
  canBusFD.write(msg.getCANmessage());
}

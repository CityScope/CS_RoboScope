#include <FlexCAN_T4.h>
#include <string.h>
#include "message.h"

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
    }

    if (operation == 'S') {
      test_pixels();
      Serial.println("Send Test Pixels");
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
void test_pixels() {
  Pixel buf[8];
  for (int j = 0; j < 8; j++) {
    byte temp[4] = {0};
    for (int i = 0; i < 4; i++) {
      temp[i] = int(random(0, 255));
    }
    buf[j] = {temp[0], temp[1], temp[2], temp[3]};
  }
  translate_pixels(node, buf);

}

//----------------------------------------------------------------
void test() {
  if (millis() - initial_timer > 20000) {
    if ((millis() - primary_timer) > 10000) {
      Pixel buf[1] = {0};
      buf[0] = {170, 10, 255, 128};
      if (node == 95) {
        node = 0;
      } else {
        node++;
      }
      primary_timer = millis();
    }
  }
}

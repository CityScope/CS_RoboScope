#include <FlexCAN_T4.h>
#include <string.h>
#include "message.h"

FlexCAN_T4FD<CAN3, RX_SIZE_256, TX_SIZE_32> canBusFD;

//key pins
const int KEY_PIN_01 = 6;
const int KEY_PIN_02 = 7;
const int KEY_PIN_03 = 8;
const int KEY_PIN_04 = 9;

const int LED_15 = 22;
const int LED_16 = 23;

char operation; // Holds operation (R, W, ...)
int wait_for_transmission = 5; // Delay in ms in order to receive the serial data

//format for received pixel data
struct Pixel {
  byte node;
  byte local_id;
  byte inter;
  byte height;
  byte color1;
  byte color2;
};

//----------------------------------------------------------------
void setup(void) {
  Serial.begin(115200);
  delay(500);

  canBusFD.begin();
  CANFD_timings_t config;
  config.clock = CLK_24MHz;
  config.baudrate = 1000000;
  config.baudrateFD = 2000000;
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
}

//----------------------------------------------------------------
//command
//W{node_id},{local_id},{interaction},{height},{color1},{color2}E
void loop() {
  digitalWrite(LED_15, digitalRead(KEY_PIN_01));
  digitalWrite(LED_16, digitalRead(KEY_PIN_04));

  if (Serial.available() > 0) {
    operation = Serial.read();
    if (operation == 'W') {
      byte temp[5] = {0};
      for (int i = 0; i < 6; i++) {
        temp[i] = Serial.parseInt();
        Serial.read();
      }
      Pixel buf = {0, 0, 0, 0, 0, 0};
      buf = {temp[0], temp[1], temp[2], temp[3], temp[4], temp[5]};
      translate_pixels(buf);
    }
    if (operation == 'T') {
      byte temp[5] = {0};
      Pixel buf = {0, 0, 0, 0, 0, 0};
      buf = {(int)random(255), (int)random(255), (int)random(255), (int)random(255), (int)random(255), (int)random(255)};
      translate_pixels(buf);

    }
  }
}

//----------------------------------------------------------------
//use this to send data to the table
void translate_pixels(Pixel buf) {
  byte result = (byte)(buf.node | (buf.local_id << 4));
  CANMotorMessage msg = CANMotorMessage(result);
  msg.addMessage(buf.height, buf.inter, buf.color1, buf.color2);
  canBusFD.write(msg.getCANmessage());
}

//----------------------------------------------------------------
//use this to send data to interface
void send_pixels(Pixel buf[], int buf_size) {
  for (int i = 0; i < buf_size; i++) {
    char str[20];
    sprintf(str, "%d,%d,%s,%d,%X,%X;", buf[i].node, buf[i].local_id, bin(buf[i].inter), buf[i].height, buf[i].color1, buf[i].color2);
    Serial.print(str);
  }
  Serial.println(' ');
}

#pragma once

#include "arduino.h"
#include <FlexCAN_T4.h>

class CANMotorMessage {

  public:

    CANMotorMessage(int nodeId);
    CANFD_message_t getCANmessage(int len);

    // motor message
    void addMessage(int des, int b1, int b2, uint8_t motorByte, uint8_t modeByte, uint8_t density, uint8_t colorR, uint8_t colorG, uint8_t colorB);

  private: 

    int msg_count = 0;

    //node that the message is being sent to.
    int nodeId;

    //size of msg. FD = 8 data packages, CAN Bus is 2 packages.
    int msgSize;

    CANFD_message_t msg;
};

/*
    Serial.print("Node ID: ");
    Serial.print(nodeId);
    Serial.print(", Pixel ID: ");
    Serial.print(pixelId);
    Serial.println();
    Serial.print(currentPixel[0]);
    Serial.print(" ");
    Serial.print(currentPixel[1]);
    Serial.print(" ");
    Serial.print(currentPixel[2]);
    Serial.print(" ");
    Serial.print(currentPixel[3]);
    Serial.print(" ");
    Serial.print(currentPixel[4]);
    Serial.print(" ");
    Serial.print(currentPixel[5]);
    Serial.print(" ");
    Serial.print(currentPixel[6]);
    Serial.print(" ");
    Serial.print(currentPixel[7]);
    Serial.println();
    */
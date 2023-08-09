#include "message.h"

CANMotorMessage::CANMotorMessage(int nodeId) {
  this->msg.id = nodeId; // this-> refers to the object of this class calling this method/member.
}

//-----------------------------------------------------

void CANMotorMessage::addMessage(int des, int b1, int b2, uint8_t motorByte, uint8_t modeByte, uint8_t density, uint8_t colorR, uint8_t colorG, uint8_t colorB) { // des is the pixel we're focusing on
  uint8_t msg[8] = {0, 0, motorByte, modeByte, density, colorR, colorG, colorB}; // one pixel.
  memcpy(this->msg.buf + des * 8, msg, 8); // Takes the 8 bytes of data in msg, and copies it to the address of the buffer of the object + the destination specified times 8. 
}

//-----------------------------------------------------

CANFD_message_t CANMotorMessage::getCANmessage(int len) {
  this->msg.len = len;
  return this->msg;
}

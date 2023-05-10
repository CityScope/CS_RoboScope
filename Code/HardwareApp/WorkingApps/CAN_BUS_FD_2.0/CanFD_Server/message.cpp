#include "message.h"

//board id, can bus type
//----------------------------------------------------------------
CANMotorMessage::CANMotorMessage(int panel_id) {
  panelId = panel_id;
  this->msg.id = panelId;
}

//----------------------------------------------------------------
void CANMotorMessage::addMessage(int des, uint8_t interaction, uint8_t shaff, uint8_t enable, uint8_t disable, uint8_t height, uint8_t colorR, uint8_t colorG, uint8_t colorB) {
  uint8_t msg[8] = {interaction, shaff, enable, disable, height, colorR, colorG, colorB};
  memcpy(this->msg.buf + des * 8, msg, 8);
}

//----------------------------------------------------------------
CANFD_message_t CANMotorMessage::getCANmessage(int len) {
  this->msg.len = len;
  return this->msg;
}

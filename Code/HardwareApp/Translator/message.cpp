#include "message.h"

//board id, can bus type
//----------------------------------------------------------------
CANMotorMessage::CANMotorMessage(int panel_id, int msg_size) {
  panelId = panel_id;
  this->msg.id = panelId;
  this->msgSize = msg_size;
}

//----------------------------------------------------------------
void CANMotorMessage::addMessage(int des, uint8_t steps, uint8_t interaction, uint8_t color1, uint8_t color2) {
  uint8_t msg[4] = {steps, interaction, color1, color2};
  memcpy(this->msg.buf + des * 4, msg, 4);
}

//----------------------------------------------------------------
CANFD_message_t CANMotorMessage::getCANmessage() {
  this->msg.len = 32;
  return this->msg;
}

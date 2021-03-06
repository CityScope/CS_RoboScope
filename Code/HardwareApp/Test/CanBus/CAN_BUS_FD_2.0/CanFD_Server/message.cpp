#include "message.h"

//board id, can bus type
//----------------------------------------------------------------
CANMotorMessage::CANMotorMessage(int panel_id, int msg_size) {
  panelId = panel_id;
  this->msg.id = panelId;
  this->msgSize = msg_size;

  Serial.print("Panel id: ");
  Serial.print(panelId);
  Serial.print(" ");
  Serial.print(msgSize);
  Serial.print(" ");

  if (msgSize == 8) {
    Serial.print("CAN BUS FD ");
  }
  else if ( msgSize == 2) {
    Serial.print("CAN BUS 2.0 ");
  } else {
    Serial.print("NOY SUPPORTED CAN BUS SIZE");
  }

  Serial.println();
}


//----------------------------------------------------------------
void CANMotorMessage::addMessage(uint8_t steps, uint8_t interaction, uint8_t color1, uint8_t color2) {
  uint8_t msg[4] = {steps, interaction, color1, color2};
  memcpy(this->msg.buf, msg, 4);
}

//----------------------------------------------------------------
CANFD_message_t CANMotorMessage::getCANmessage() {
  this->msg.len = 4;
  return this->msg;
}

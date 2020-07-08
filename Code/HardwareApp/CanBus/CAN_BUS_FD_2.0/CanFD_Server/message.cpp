#include "message.h"
#include "functions.h"


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
void CANMotorMessage::addMessage(int des, uint8_t color[], uint8_t steps, uint8_t interaction) {
  uint16_t two_color = (((color[0] & 0b11111000) << 8) + ((color[1] & 0b11111100) << 3) + (color[2] >> 3));
  uint8_t colors[2] = {convertFrom16To8(two_color)};
  uint8_t msg[4] = {colors[0], colors[1], steps, interaction};
  memcpy(this->msg.buf + des * 4, msg, 4);
}

//----------------------------------------------------------------
CANFD_message_t CANMotorMessage::getCANmessage() {
  this->msg.len = 64;
  return this->msg;
}

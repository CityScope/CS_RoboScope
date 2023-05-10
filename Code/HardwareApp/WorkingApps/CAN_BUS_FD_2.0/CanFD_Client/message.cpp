#include "message.h"

//board id, can bus type
//----------------------------------------------------------------
CANMotorMessage::CANMotorMessage(int panel_id, int msgSize) {
  this->panelId = panel_id;
  this->msg.id = msgSize;

  Serial.print("Panel id: ");
  Serial.print(panelId);
  Serial.print(" ");
  Serial.print(msgSize);
  Serial.print(" ");

  if (msgSize >= 16) {
    Serial.print("CAN BUS FD ");
  }
  else if ( msgSize == 8) {
    Serial.print("CAN BUS 2.0 ");
  } else {
    Serial.print("NOY SUPPORTED CAN BUS SIZE");
  }

  Serial.println();

}

//----------------------------------------------------------------
void CANMotorMessage::addMessage(int des, uint8_t interaction, uint8_t steps, uint8_t color0, uint8_t color1) {
  uint8_t msg[4] = {interaction, steps, color0, color1};
  memcpy(this->msg.buf + des * 4, msg, 4);
}

//----------------------------------------------------------------
CANFD_message_t CANMotorMessage::getCANmessage() {
  return this->msg;
}


// create interaction byte msg
uint8_t CANMotorMessage::createInteractionMsg() {

}

//generate the motor byte msg
uint8_t CANMotorMessage::createMotorMsg() {

}

//generate the color byte msg
uint8_t * CANMotorMessage::createColorMsg() {

}

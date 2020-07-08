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

//add motor message
CANMotorMessage::addMessage(int des, uint8_t color[], uint8_t steps, uint8_t interaction) {


}

CANFD_message_t CANMotorMessage::getCANmessage() {

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

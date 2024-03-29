#include "message.h"


//----------------------------------------------------------------
//Convert 2 bytes to a single 16bit number
uint16_t convertFrom8To16(uint8_t dataFirst, uint8_t dataSecond) {
    uint16_t dataBoth = 0x0000;

    dataBoth = dataFirst;
    dataBoth = dataBoth << 8;
    dataBoth |= dataSecond;
    return dataBoth;
}

//----------------------------------------------------------------
//Convert from 16 bit number to two single uint8_t
uint8_t* convertFrom16To8(uint16_t dataAll) {
    static uint8_t arrayData[2] = { 0x00, 0x00 };

    *(arrayData) = (dataAll >> 8) & 0x00FF;
    arrayData[1] = dataAll & 0x00FF;
    return arrayData;
}

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


// create interaction byte msg
uint8_t CANMotorMessage::createInteractionMsg() {

}

//generate the motor byte msg
uint8_t CANMotorMessage::createMotorMsg() {

}

//generate the color byte msg
uint8_t * CANMotorMessage::createColorMsg() {

}

#include "CanBusParser.h"


//--------------------------------------------------
CanBusParser::CanBusParser() {
  canBus = new FlexCAN(SPPED_BUS);
  txMsg.len = 8;
}

//--------------------------------------------------
void CanBusParser::hexDump(uint8_t dumpLen, uint8_t *bytePtr) {
  uint8_t hex[17] = "0123456789abcdef";
  uint8_t working;
  while (dumpLen--) {
    working = *bytePtr++;
    Serial.write(hex[working >> 4]);
    Serial.write(hex[working & 15]);

  }
  Serial.write('\r');
  Serial.write('\n');
}


//--------------------------------------- -----------
// Prints the CAN bus message received in Serial
void CanBusParser::writeMsgToSerial() {
  Serial.print("CAN BUS: ");
  hexDump(8, rxMsg.buf);
}

//--------------------------------------------------
void CanBusParser::sendMsg() {
  canBus->write(txMsg);
}

//--------------------------------------------------
void CanBusParser::readMsg() {
  while (canBus->available()) {
    canBus->read(rxMsg);
    Serial.println("got MSG");
  }
}

//--------------------------------------------------
// Configures the message to send
void CanBusParser::updateMsg(uint8_t message[]) {
  for (int i = 0; i < txMsg.len; i++) {
    txMsg.buf[i] = message[i];
  }
}

//--------------------------------------------------
// Returns an array of values stored in the message
void CanBusParser::getRxMsg(uint8_t msg[]) {
  for (int i = 0; i < rxMsg.len; i ++) {
    msg[i] = rxMsg.buf[i];
  }
}


//--------------------------------------------------
//activate and send msg
void CanBusParser::blockMsg() {

}

//--------------------------------------------------
void CanBusParser::activateMsg() {
  activeMsg = true;
}

//--------------------------------------------------
void CanBusParser::deactivateMsg() {
  activeMsg = false;
}

//--------------------------------------------------
boolean CanBusParser::isActiveMsg() {
  return activeMsg;
}

//--------------------------------------------------
void CanBusParser::printCanMsg(CAN_message_t msg) {
  Serial.print("msg: ");
  Serial.println(msg.id);
  for (int i = 0; i < 8; i++) {
    Serial.println(msg.buf[i]);
  }
}

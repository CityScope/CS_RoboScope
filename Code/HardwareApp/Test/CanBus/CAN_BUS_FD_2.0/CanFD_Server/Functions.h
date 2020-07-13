/*
 * 
 *  Helper functions
 *  
*/
//----------------------------------------------------------------
void sendCanBusMsg(uint8_t input) {
  if (input == HIGH) {
    Serial.print("Writing CAN FD: ");
    CANMotorMessage msg = CANMotorMessage(MOTOR_ID);
    Serial.println(MOTOR_ID);
    Serial.print(" ");
    uint8_t colors[3] = {0, 255, 255};
    msg.addMessage(0, colors, 01111000, 01100011);
    msg.addMessage(2, colors, 01000000, 00001011);
    msg.addMessage(5, colors, 01111110, 01111111);
    msg.addMessage(6, colors, 01111110, 01111111);
    canBus.write(msg.returnCANmessage());
    Serial.println();
  }
}

//----------------------------------------------------------------
void sendFDMsg(uint8_t input) {
  if (input == HIGH) {
    Serial.print("Writing CAN FD: ");
    CANMotorMessage msg = CANMotorMessage(MOTOR_ID);
    Serial.println(MOTOR_ID);
    Serial.print(" ");
    uint8_t colors[3] = {0, 255, 255};
    msg.addMessage(0, colors, 01111000, 01100011);
    msg.addMessage(2, colors, 01000000, 00001011);
    msg.addMessage(5, colors, 01111110, 01111111);
    msg.addMessage(6, colors, 01111110, 01111111);
    canBusFD.write(msg.returnCANmessage());
    Serial.println();
  }
}

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

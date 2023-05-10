

//----------------------------------------------------------------
void sendCanBusMsg(uint8_t input) {
  if (input == HIGH) {
    Serial.print("Writing CAN FD: ");
    CANMotorMessage msg = CANMotorMessage(MOTOR_ID);
    Serial.println(MOTOR_ID);
    Serial.print(" ");
    uint16_t color16 = 1111100000000000;
    uint8_t * color8 = convertFrom16To8(color16);
    msg.addMessage(0, 01111000, 01100011, color8[0], color8[1]);
    msg.addMessage(2, 01000000, 00001011, color8[0], color8[1]);
    msg.addMessage(5, 01111110, 01111111, color8[0], color8[1]);
    msg.addMessage(6, 01111110, 01111111, color8[0], color8[1]);
    canBus.write(msg.getCANmessage());
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
   uint16_t color16 = 1111100000000000;
    uint8_t * color8 = convertFrom16To8(color16);
    msg.addMessage(0, 01111000, 01100011, color8[0], color8[1]);
    msg.addMessage(2, 01000000, 00001011, color8[0], color8[1]);
    msg.addMessage(5, 01111110, 01111111, color8[0], color8[1]);
    msg.addMessage(6, 01111110, 01111111, color8[0], color8[1]);
    FD.write(msg.getCANmessage());
    Serial.println();
  }
}



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
    uint8_t colors[3] = {0, 255, 255};
    msg.addMessage(0, colors, 01111000, 01100011);
    msg.addMessage(2, colors, 01000000, 00001011);
    msg.addMessage(5, colors, 01111110, 01111111);
    msg.addMessage(6, colors, 01111110, 01111111);
    FD.write(msg.getCANmessage());
    Serial.println();
  }
}

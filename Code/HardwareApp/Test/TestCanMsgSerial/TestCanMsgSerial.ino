

void setup() {
  // put your setup code here, to run once:

  Serial.begin(57600);
  Serial1.begin(57600);
  
  delay(2000);
  Serial.println("Starting Serial");
  Serial.println("Test serial CAN MSG");

}

void loop() {
  // put your main code here, to run repeatedly:

  if (Serial.available() > 0) {
    uint8_t buff[6];
    int length = 6;
    Serial.readBytes(buff, length);
    
    uint16_t boardId = buff[0] | buff[1] << 8;
    uint16_t color =  buff[2] | buff[3] << 8;
    uint8_t step = buff[4];
    uint8_t interaction = buff[5];

    Serial.println("got msg:");
    Serial.println(boardId);

    Serial.println(color);
    Serial.println(step);
    Serial.println(interaction);

    //write on the other serial
    Serial1.write(buff, 6);

  }
}

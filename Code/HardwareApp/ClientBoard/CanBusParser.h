/*
   Class that manages CAN Bus Communication protocol
*/
#ifndef CAN_BUS_PARSER_H
#define CAN_BUS_PARSER_H

#include <FlexCAN.h>

#define SPPED_BUS     1000000
#define MSG_LENGTH    8


//---------------------------------------------------------
class CanBusParser {
  public:
    //constructor
    CanBusParser();

    void sendMsg();
    void updateMsg(uint8_t message[]);

    //update msg
    void waitforMsg();

    void getRxMsg(uint8_t msg[]);

    //dump msg
    void hexDump(uint8_t dumpLen, uint8_t *bytePtr);
    void writeMsgToSerial();
    
  private:

    FlexCAN * canBus;

    CAN_message_t txMsg;
    CAN_message_t rxMsg;
};

#endif

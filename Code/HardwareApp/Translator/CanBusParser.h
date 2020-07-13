/*
   Class that manages CAN Bus Communication protocol
*/
#ifndef CAN_BUS_PARSER_H
#define CAN_BUS_PARSER_H

#include <FlexCAN.h>

/*
 * Teensy available speeds:
 * https://www.pjrc.com/teensy/td_uart.html
 * 
 * 2000000
 * 1000000
 * 250000 
 * 115200
 * 38400
 */
 
#define SERIAL_BAU    1000000 
 
#define SPPED_BUS     1000000
#define MSG_LENGTH    8


/*
 * 
typedef struct CAN_message_t {
    uint32_t id;            // board id, this is based on the local DIP Switch configuration
    uint16_t timestamp;     // timestep of the board
    uint8_t len;            // length of data
    uint8_t buf[8];         // buf of the msg
} CAN_message_t;


   * Msg structure buf[8]
   * [0] : motorID -> local motor
   * [1] : motorDir 
   * [2] : motorStep
   * [3] : motorTimeActivation
   * [4] : motorEnable
   * [5] : color
   * [6] : motorSensor0
   * [7] : motorSensor1
   * 
*/

//---------------------------------------------------------
class CanBusParser {
  public:
    //constructor
    CanBusParser();

    //msg
    void sendMsg();
    void readMsg();

    //update msg from an input msg
    void updateMsg(uint8_t message[]);
    void getRxMsg(uint8_t msg[]);

    //dump msg
    void hexDump(uint8_t dumpLen, uint8_t *bytePtr);
    void writeMsgToSerial();

    //activate and send msg
    void blockMsg();

    //activate
    void activateMsg();
    void deactivateMsg();
    boolean isActiveMsg();

    static void printCanMsg(CAN_message_t msg);
    
    
  private:

    FlexCAN * canBus;

    boolean   activeMsg;

    CAN_message_t txMsg;
    CAN_message_t rxMsg;
};

#endif

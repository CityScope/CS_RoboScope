#pragma once

/*


   CAN BUS FD has a 64 msg protocol.
    - 8 motor per msg
   CAN BUS 2.0 has a 8 byte msg protocol
    - 2 motors per msg

   Messsage Protocol:
   4 bytes:
      2 bytes for color
      1 byte for rod position 0 - 255
      1 byte -> 00000000
          8 bits:
            0
            1
            2
            3
            4
            5
            6
            7
*/

#include "arduino.h"
#include <FlexCAN_T4.h>


class CANMotorMessage {
  public:
    //board id, can bus type
    CANMotorMessage(int panel_id, int msg_size = 8);

    //add motor message
    void addMessage(int des, uint8_t steps, uint8_t interaction, uint8_t color1, uint8_t color2);

    //get CAN Bus Msg
    CANFD_message_t getCANmessage();

    // create interaction byte msg
    uint8_t createInteractionMsg();

    //generate the motor byte msg
    uint8_t createMotorMsg();

    //generate the color byte msg
    uint8_t * createColorMsg();

  private:
    int msg_count = 0;

    //panel id that we are sending the msg
    int panelId;

    //size of the msg, fd is 8 data packages, can bus is 2 packages
    int msgSize;

    //CAN Bus FD 
    CANFD_message_t msg;
};

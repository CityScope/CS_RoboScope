## Tes Codes:

- 

## CAN Bus Protocol

Data Structure:
CAN BUS 2
https://github.com/collin80/FlexCAN_Library

CAN BUS FD with Teensy 4.0
https://github.com/tonton81/FlexCAN_T4

CAN Bus FD
MCP2562FD

https://www.microchip.com/wwwproducts/en/MCP2562FD

https://github.com/skpang/Teensy40_triple_CAN_demo/blob/master/Teensy40_triple_CAN_demo.ino


## Message Structure

### Sever Msg

Messages are bi-directional from server and client.

Server will

  - Board id
    - [id] -> uint32_t
    - ````4 bytes -> [0-4294967295] or [0x00000000-0xFFFFFFFF]````
  - Message
    - buf[8] -> uint8_t array length 8
    - ````1 byte -> [0-255] or [0x00-0xFF]````
      - [0] - Motor id [0, 255]
          - Local id depending on the number of motors on the board
      - [1] - Motor dir - up/down
      - [2] - Motor step  - number of steps
      - [3] - Motor time activation
      - [4] - Motor enable/sleep/activation
      - [5] - Motor limit switches and push and pull
          - ```[0000LHDU]```
          - Limit switch up   (U)    ```[0000000U]```
          - Limit switch down (D)    ```[000000D0]```
          - Push Interaction (Push)  ```[00000H00]```
          - Pull Interaction (Pull)  ```[0000L000]```
      - [6] - Motor sensor 0
      - [7] - Motor sensor 1

### Flex CAN CAN_message_t

````
typedef struct CAN_message_t {
    uint32_t id;          // can identifier
    uint16_t timestamp;   // FlexCAN time when message arrived
    struct {
        uint8_t extended:1; // identifier is extended (29-bit)
        uint8_t remote:1;   // remote transmission request packet type
        uint8_t overrun:1;  // message overrun
        uint8_t reserved:5;
    } flags;
    uint8_t len;          // length of data
    uint8_t buf[8];
} CAN_message_t;
````

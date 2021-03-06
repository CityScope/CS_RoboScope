# Hardware Apps

Arduino

## Translator App

Application that manages and translates grid information to the motor modules.

## Module App

Client application for each board.

## Libraries

List of Arduino libraries

- SX1509
- NeoPixel
- CAN Bus FD FlexCAN

## Test Code

- CanBus

- EnableMotor

- NeoPixel

- MotorTest

- InteractionTest

### CAN Bus Protocol


- 11-bit Motor id identifier
- CAN Bus Message - 4 byte Protocol
  - 2 byte RGBW Color
  - 1 byte Motor position
  - 1 byte interaction

![CAN Bus Block](/Docs/Designs/CanBus/25042020_CANBusMsg.png)

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

### Library:

  - CAN Bus FD and CAN Bus 2.0 for  Teensy 4.0
    https://github.com/tonton81/FlexCAN_T4

  - Data Structure:
    CAN Bus 2.0
    https://github.com/collin80/FlexCAN_Library

  - CAN Bus FD - MCP2562FD
    https://www.microchip.com/wwwproducts/en/MCP2562FD

### Refereces:

  https://github.com/skpang/Teensy40_triple_CAN_demo/blob/master/Teensy40_triple_CAN_demo.ino

## List

8/18

Final list of items to buy:
https://docs.google.com/spreadsheets/d/1OweoeCU06feMGqQnCsFGZUhG3TEGoh_JW88oVjHfxHk/edit#gid=0


### Base

- design files of the base
- matrix design files
- Rubber Bushings for base
- screws for motors
- acrylic for top
- aluminum plate for the matrix
- aluminum plate for bottom plate (electronics)
- 3d model of the spacer

Instead of matrix grid approach for the base, Kent suggest we should do horizontal or vertical modules, for easier remove, adjust and plug-play approach.

### Rods/Box

- switches
  - https://www.mouser.com/ProductDetail/653-D2F-01L2-T
- LEDs RGBW
  - NeoPixel:
    https://www.adafruit.com/product/2758
- qwiic cables JST-4 Cable
- 3d Model of the Box
- sensor to detect touch
- Pull/Push interaction

## Energy

Teensy module
  - 4 Amps

## Controls

 - Voltage Control:
   - Arduino ->
   - LEDs
   - Motor
   - Sensors

 - Motor control https://www.sparkfun.com/products/12779
    - STEP    -> GPIO Arduino
    - DIR    -> GPIO Arduino
    - ENABLE -> GPIO Arduino
    - MS1 -> switch GND or 5V
    - MS2 -> switch GND or 5V
    - Voltage IN  -> Power supply range from 6V to 30V.
    - Voltage OUT -> regulated 5V

- Motor Driver:
    - STSPIN220 Low-Voltage Stepper Motor Driver Carrier,  1/256 step https://www.pololu.com/product/2876
    - DRV8834 - 1/32 step https://www.pololu.com/product/2134/specs

 - CAN Bus Protocol (5V, GND)
    - MCP2515  with TJA1050
        - https://harrisonsand.com/static/CAN_module.pdf
    - SCK, MOSI, MISO
    - D13 -> SCK - SCK
    - D12 -> SO  - MISO
    - D11 -> SI  - MOSI
    - D10 -> CS  - ANY
 - swiic cable model
 - voltage input 5V
 - Teensy 3.2 -> CAN BUS, I2C
 - LED for power status
 - switch to turn on/off the motor
 - enumerate each motor and sensor output
      -
 - SAMD21  + 48mhrz
    - https://www.sparkfun.com/products/13664
    - https://www.avdweb.nl/arduino/samd21/samd21-programmer
 - DIP Switch for changing the ID of the motor
    - DIP 10 positions -> 2^9
      https://www.sparkfun.com/products/8034
    - Multiplexer   16 channels
      https://www.sparkfun.com/products/9056
  - CAN Bus Output
  - Switch's Control
      - SparkFun 16 Output I/O Expander Breakout
      - https://www.sparkfun.com/products/13601

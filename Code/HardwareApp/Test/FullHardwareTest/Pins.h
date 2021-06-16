/*
 * MOTOR PINS
 *
 * PREFIX G -> Global
*/

//define address
#define  SX1509_ADDRESS_00         0x3E
#define  SX1509_ADDRESS_01         0x3F // 0x3F
#define  SX1509_ADDRESS_10         0x70   ///0x71
#define  SX1509_ADDRESS_11         0x71

//MOTOR DEFINES
#define SERIAL_PORT Serial5
#define DRIVER_ADDRESS 0b00

#define R_SENSE 0.11f


#define LED_STATUS 19

#define INTERRUPT_PIN_SWITCH_01   32
#define INTERRUPT_PIN_SWITCH_02   33

//MOTOR VALUES
#define GMOTOR_STEPS      200 *321 //200
#define GRPM              120 //120
#define MOTOR_STEPS       256
#define RMS_CURRENT       400



//MOTOR PINS
//MOTOR 1
#define ENABLE_01_SX00     10 // SX Enable
#define DIR_PIN_01          6 // Direction
#define STEP_PIN_01         5 // Step

#define ENABLE_02_SX00      6 //  SX Enable
#define DIR_PIN_02          8 // Direction
#define STEP_PIN_02         7 // Step 

#define ENABLE_03_SX00      4 //  SX Enable
#define DIR_PIN_03          10 // Direction 
#define STEP_PIN_03         9 // Step 

#define ENABLE_04_SX00      2 // SX Enable
#define DIR_PIN_04          12 // Direction
#define STEP_PIN_04         11 // Step

#define ENABLE_05_SX00      8 // SX Enable
#define DIR_PIN_05          17 // Direction
#define STEP_PIN_05         16 // Step 

#define ENABLE_06_SX00      0 // SX Enable
#define DIR_PIN_06          3 // Direction 
#define STEP_PIN_06         2 // Step 

#define ENABLE_07_SX00      12 // SX Enable
#define DIR_PIN_07          23 // Direction 
#define STEP_PIN_07         22 // Step 

#define ENABLE_08_SX00      14 // SX Enable
#define DIR_PIN_08          27 // Direction 
#define STEP_PIN_08         26 // Step 


//NEO PIXELS
#define  NEO_PIN_01        4
#define  NEO_PIN_02       13
#define  NEO_PIN_03       14//2;
#define  NEO_PIN_04       15///3;
#define  NEO_PIN_05       24
#define  NEO_PIN_06       25
#define  NEO_PIN_07       28
#define  NEO_PIN_08       29

//LIMIT BUTTONS
#define SWITCH_01_SX01    8 
#define SWITCH_02_SX01    6 
#define SWITCH_03_SX01    9 
#define SWITCH_04_SX01    7 

#define SWITCH_05_SX02    8 
#define SWITCH_06_SX02    6 
#define SWITCH_07_SX02    9 
#define SWITCH_08_SX02    7 

//DOWN BUTTONS
#define DOWN_01_SX01      12 
#define DOWN_02_SX01      4 
#define DOWN_03_SX01      15 
#define DOWN_04_SX01      5 

#define DOWN_05_SX02      12 
#define DOWN_06_SX02      4 
#define DOWN_07_SX02      15 
#define DOWN_08_SX02      5 


//UP BUTTONS
#define  UP_01_SX01       11 
#define  UP_02_SX01       3 
#define  UP_03_SX01       14 
#define  UP_04_SX01       1 

#define  UP_05_SX02       11 
#define  UP_06_SX02       3
#define  UP_07_SX02       14 
#define  UP_08_SX02       1 

//TOUCH PIN
#define  TOUCH_01_SX01    10 
#define  TOUCH_02_SX01    2 
#define  TOUCH_03_SX01    13 
#define  TOUCH_04_SX01    0 

#define  TOUCH_05_SX02    10 
#define  TOUCH_06_SX02    2 
#define  TOUCH_07_SX02    13
#define  TOUCH_08_SX02    0 


//DIP SWITCH
#define  DIP_01_SX03      6 
#define  DIP_02_SX03      7 
#define  DIP_03_SX03      8 
#define  DIP_04_SX03      9 
#define  DIP_05_SX03      10
#define  DIP_06_SX03      11 
#define  DIP_07_SX03      12 
#define  DIP_08_SX03      13 
#define  DIP_09_SX03      14 
#define  DIP_10_SX03      15

//STATUS PIN
#define  STATUS_PIN_SX03  5 

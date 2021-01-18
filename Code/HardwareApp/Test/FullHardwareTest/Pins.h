/*
 * MOTOR PINS
 *
 * PREFIX G -> Global
*/

//define address
#define  SX1509_ADDRESS_00         0x3E
#define  SX1509_ADDRESS_01         0x3F
#define  SX1509_ADDRESS_10         0x71
#define  SX1509_ADDRESS_11         0x70

#define LED_STATUS 19

#define INTERRUPT_PIN_SWITCH_01   28
#define INTERRUPT_PIN_SWITCH_02   29

//MOTOR VALUES
#define GMOTOR_STEPS      200 *321 //200
#define GRPM              120 //120

//STEPS PISN
#define GM0_PIN           11
#define GM1_PIN           7 //sx

//Energy Pins
#define TRQ1_PIN          32 
#define TRQ2_PIN          33

//MOTOR PINS
//MOTOR 1
#define DIR_PIN_01        6
#define STEP_PIN_01       5
#define ENABLE_01_SX00    10  // ENABLE PIN on the SX1509

// MOTOR 2
#define DIR_PIN_02        8
#define STEP_PIN_02       7
#define ENABLE_02_SX00    6  // ENABLE PIN on the SX1509

// MOTOR 3
#define DIR_PIN_03        10
#define STEP_PIN_03       9
#define ENABLE_03_SX00    4  // ENABLE PIN on the SX1509


// MOTOR 4
#define DIR_PIN_04        12
#define STEP_PIN_04       11
#define ENABLE_04_SX00    2  // ENABLE PIN on the SX1509


// MOTOR 5
#define DIR_PIN_05        21
#define STEP_PIN_05       20
#define ENABLE_05_SX00    8  // ENABLE PIN on the SX1509


// MOTOR 6
#define DIR_PIN_06        3
#define STEP_PIN_06       2
#define ENABLE_06_SX00    0  // ENABLE PIN on the SX1509


// MOTOR 7
#define DIR_PIN_07        23
#define STEP_PIN_07       22
#define ENABLE_07_SX00    12  // ENABLE PIN on the SX1509


// MOTOR 8
#define DIR_PIN_08        27
#define STEP_PIN_08       26
#define ENABLE_08_SX00    14  // ENABLE PIN on the SX1509


//NEO PIXELS
#define  NEO_PIN_01       16
#define  NEO_PIN_02       17
#define  NEO_PIN_03       14//2;
#define  NEO_PIN_04       15///3;
#define  NEO_PIN_05       4
#define  NEO_PIN_06       24
#define  NEO_PIN_07       25
#define  NEO_PIN_08       13

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

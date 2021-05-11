/*
   MOTOR PINS

   PREFIX G -> Global
*/




#define  SX1509_ADDRESS_00         0x3E
#define  SX1509_ADDRESS_01         0x3F // 0x3F
#define  SX1509_ADDRESS_10         0x70   ///0x71
#define  SX1509_ADDRESS_11         0x71

//Number of motors that each panel has
#define MOTORS_PER_PANEL 8
#define PIXELS_PER_URBAN_PIXEL 2


#define MOTOR_STEP_TOLERANCE 20

#define EN_PIN_01           10 // SX Enable
#define DIR_PIN_01          6 // Direction
#define STEP_PIN_01         5 // Step

#define EN_PIN_02           6 //  SX Enable
#define DIR_PIN_02          8 // Direction
#define STEP_PIN_02         7 // Step

#define EN_PIN_03           4 //  SX Enable
#define DIR_PIN_03          10 // Direction
#define STEP_PIN_03         9 // Step

#define EN_PIN_04           2 // SX Enable
#define DIR_PIN_04          12 // Direction
#define STEP_PIN_04         11 // Step

#define EN_PIN_05           8 // SX Enable
#define DIR_PIN_05          17 // Direction
#define STEP_PIN_05         16 // Step

#define EN_PIN_06           0 // SX Enable
#define DIR_PIN_06          3 // Direction
#define STEP_PIN_06         2 // Step

#define EN_PIN_07           12 // SX Enable
#define DIR_PIN_07          23 // Direction
#define STEP_PIN_07         22 // Step

#define EN_PIN_08           14 // SX Enable
#define DIR_PIN_08          27 // Direction
#define STEP_PIN_08         26 // Step

#define SERIAL_PORT Serial5
#define DRIVER_ADDRESS 0b00


#define R_SENSE 0.11f

//DEBUG LED NEO PIXEL
//#define NEO_DEBUG      32

//LED DEBUG TEENSY
#define LED_TEENSY     13

//NEO PIXELS
//#define NEO_PIXEL_PIN   2

//DIP SWITCH
#define MUX_S0         26
#define MUX_S1         27
#define MUX_S2         28
#define MUX_S3         31
#define MUX_SIG       A13

//MUX I2C
#define MUX_SDA       18
#define MUX_SCL       19

#define INTERRUPT_PIN_SWITCH       28

//1
#define SX1509_01_MENABLE        0  // ENABLE PIN on the SX1509
#define SX1509_01_MSLEEP         11 //  SLEEP PIN on the SX1509
//2
#define SX1509_02_MENABLE        2  // ENABLE PIN on the SX1509
#define SX1509_02_MSLEEP         7 //  SLEEP PIN on the SX1509
//3
#define SX1509_03_MENABLE        4  // ENABLE PIN on the SX1509
#define SX1509_03_MSLEEP         5 //  SLEEP PIN on the SX1509
//4
#define SX1509_04_MENABLE        6  // ENABLE PIN on the SX1509
#define SX1509_04_MSLEEP         3 //  SLEEP PIN on the SX1509
//5
#define SX1509_05_MENABLE        8  // ENABLE PIN on the SX1509
#define SX1509_05_MSLEEP         9 //  SLEEP PIN on the SX1509
//6
#define SX1509_06_MENABLE        10  // ENABLE PIN on the SX1509
#define SX1509_06_MSLEEP         1 //  SLEEP PIN on the SX1509
//7
#define SX1509_07_MENABLE        12  // ENABLE PIN on the SX1509
#define SX1509_07_MSLEEP         13 //  SLEEP PIN on the SX1509
//8
#define SX1509_08_MENABLE        14  // ENABLE PIN on the SX1509
#define SX1509_08_MSLEEP         15 //  SLEEP PIN on the SX1509



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

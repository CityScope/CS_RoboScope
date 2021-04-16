/*
   MOTOR PINS

   PREFIX G -> Global
*/


//Number of motors that each panel has
#define MOTORS_PER_PANEL 8
#define PIXELS_PER_URBAN_PIXEL 2

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

#define SX1509_ADDRESS_00 0x3E

//DEBUG LED NEO PIXEL
#define NEO_DEBUG      32

//LED DEBUG TEENSY
#define LED_TEENSY     13

//NEO PIXELS
#define NEO_PIXEL_PIN   2

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



//MUX SWITH AND LED
#define SX1509_01_SWITCH_STOP      15 // LED connected to pin 8
#define SX1509_02_SWITCH_STOP      14 // LED connected to pin 9
#define SX1509_03_SWITCH_STOP      13 // LED connected to pin 10
#define SX1509_04_SWITCH_STOP      12 // LED connected to pin 11
#define SX1509_05_SWITCH_STOP      11 // LED connected to pin 12
#define SX1509_06_SWITCH_STOP      10 // LED connected to pin 13
#define SX1509_07_SWITCH_STOP       9 // LED connected to pin 14
#define SX1509_08_SWITCH_STOP       8 // LED connected to pin 15

#define SX1509_01_SWITCH_LEDBOX       7
#define SX1509_02_SWITCH_LEDBOX       6
#define SX1509_03_SWITCH_LEDBOX       5
#define SX1509_04_SWITCH_LEDBOX       4
#define SX1509_05_SWITCH_LEDBOX       3
#define SX1509_06_SWITCH_LEDBOX       2
#define SX1509_07_SWITCH_LEDBOX       1
#define SX1509_08_SWITCH_LEDBOX       0

#define NEO_PIN_01       16
#define NEO_PIN_02       17
#define NEO_PIN_03       2
#define NEO_PIN_04       3
#define NEO_PIN_05       4
#define NEO_PIN_06       24
#define NEO_PIN_07       25
#define NEO_PIN_08       13

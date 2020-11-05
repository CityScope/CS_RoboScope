/*
   MOTOR PINS

   PREFIX G -> Global
*/


//Number of motors that each panel has
#define MOTORS_PER_PANEL 8

#define PIXELS_PER_URBAN_PIXEL 2

#define NUM_URBAN_PIXELS 2


// MOTOR 1
#define DIR_PIN_01     6
#define STEP_PIN_01    5

// MOTOR 2
#define DIR_PIN_02     8
#define STEP_PIN_02    7

// MOTOR 3
#define DIR_PIN_03     10
#define STEP_PIN_03    9

// MOTOR 4
#define DIR_PIN_04     12
#define STEP_PIN_04    11

// MOTOR 5
#define DIR_PIN_05     21
#define STEP_PIN_05    20

// MOTOR 6
#define DIR_PIN_06     3
#define STEP_PIN_06    2

// MOTOR 7
#define DIR_PIN_07     23
#define STEP_PIN_07    22

// MOTOR 8
#define DIR_PIN_08     27
#define STEP_PIN_08    26

//Global Values
#define GMOTOR_STEPS  200*16  // 200 200 * 16
#define GRPM          120 // 200


#define INTERRUPT_PIN_SWITCH_01       28
#define INTERRUPT_PIN_SWITCH_02       29

#define TRQ1_PIN 32 
#define TRQ2_PIN 33

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

//Sleep and enable

//1
#define SX1509_01_MSLEEP         11 //  SLEEP PIN on the SX1509
#define SX1509_01_MENABLE        10  // ENABLE PIN on the SX1509

//2
#define SX1509_02_MSLEEP         7 //  SLEEP PIN on the SX1509
#define SX1509_02_MENABLE        6  // ENABLE PIN on the SX1509

//3
#define SX1509_03_MSLEEP         5 //  SLEEP PIN on the SX1509
#define SX1509_03_MENABLE        4  // ENABLE PIN on the SX1509

//4
#define SX1509_04_MSLEEP         3 //  SLEEP PIN on the SX1509
#define SX1509_04_MENABLE        2  // ENABLE PIN on the SX1509

//5
#define SX1509_05_MSLEEP         9 //  SLEEP PIN on the SX1509
#define SX1509_05_MENABLE        8  // ENABLE PIN on the SX15092

//6
#define SX1509_06_MSLEEP         1 //  SLEEP PIN on the SX1509
#define SX1509_06_MENABLE        0  // ENABLE PIN on the SX1509

//7
#define SX1509_07_MSLEEP         13 //  SLEEP PIN on the SX1509
#define SX1509_07_MENABLE        12  // ENABLE PIN on the SX1509

//8
#define SX1509_08_MSLEEP         15 //  SLEEP PIN on the SX1509
#define SX1509_08_MENABLE        14  // ENABLE PIN on the SX1509

//
#define SX1509_ADDRESS_00  0x3E
#define SX1509_ADDRESS_01  0x3F
#define SX1509_ADDRESS_10  0x71
#define SX1509_ADDRESS_11  0x70

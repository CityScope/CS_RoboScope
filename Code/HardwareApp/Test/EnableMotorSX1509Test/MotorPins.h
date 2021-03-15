/*
 * MOTOR PINS
 *
 * PREFIX G -> Global
 */

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
#define GMOTOR_STEPS  200 *16 //200

#define GRPM          120 //120

//Global pin values
#define GENABLE_PIN    33
#define GM0_PIN        11
#define GM1_PIN        12


#define TRQ1_PIN 32 
#define TRQ2_PIN 33

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
#define SX1509_05_MENABLE        8  // ENABLE PIN on the SX1509

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


//NEO PIXELS
#define  NEO_PIN_01       16
#define  NEO_PIN_02       17
#define  NEO_PIN_03       14//2;
#define  NEO_PIN_04       15///3;
#define  NEO_PIN_05       4
#define  NEO_PIN_06       24
#define  NEO_PIN_07       25
#define  NEO_PIN_08       13


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
#define DIR_PIN_04     12 //15
#define STEP_PIN_04    11 //14

// MOTOR 5
#define DIR_PIN_05     15 //17
#define STEP_PIN_05    14 //16

// MOTOR 6
#define DIR_PIN_06     21
#define STEP_PIN_06    20

// MOTOR 7
#define DIR_PIN_07     23
#define STEP_PIN_07    22

// MOTOR 8
#define DIR_PIN_08     27 //25
#define STEP_PIN_08    26 //24

//Global Values
#define GMOTOR_STEPS  200*16  // 200 200 * 16
#define GRPM          200 //120

//Amps
#define TRQ1_PIN       32 
#define TRQ2_PIN       33

//NEO PIXELS
#define NEO_PIXEL_PIN   2

//MUX I2C
#define MUX_SDA       18
#define MUX_SCL       19

#define INTERRUPT_PIN_SWITCH       28

#define GENABLE_PIN 28

//1
#define SX1509_01_MENABLE        0  // ENABLE PIN on the SX1509

//2
#define SX1509_02_MENABLE        2  // ENABLE PIN on the SX1509

//3
#define SX1509_03_MENABLE        4  // ENABLE PIN on the SX1509

//4
#define SX1509_04_MENABLE        6  // ENABLE PIN on the SX1509

//5
#define SX1509_05_MENABLE        8  // ENABLE PIN on the SX1509

//6
#define SX1509_06_MENABLE        10  // ENABLE PIN on the SX1509

//7
#define SX1509_07_MENABLE        12  // ENABLE PIN on the SX1509

//8
#define SX1509_08_MENABLE        14  // ENABLE PIN on the SX1509


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

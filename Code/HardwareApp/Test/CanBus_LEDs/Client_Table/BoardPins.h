/*
   MOTOR PINS

   PREFIX G -> Global
*/


//Number of motors that each panel has
#define MOTORS_PER_PANEL 8

#define PIXELS_PER_URBAN_PIXEL 4

#define NUM_URBAN_PIXELS 4

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
#define DIR_PIN_04     15
#define STEP_PIN_04    14

// MOTOR 5
#define DIR_PIN_05     17
#define STEP_PIN_05    16

// MOTOR 6
#define DIR_PIN_06     21
#define STEP_PIN_06    20

// MOTOR 7
#define DIR_PIN_07     23
#define STEP_PIN_07    22

// MOTOR 8
#define DIR_PIN_08     25
#define STEP_PIN_08    24

//Global Values
#define GMOTOR_STEPS  200*16  // 200 200 * 16
#define GRPM          200 //120

//Global pin values
#define GENABLE_PIN    33
#define GM0_PIN        11
#define GM1_PIN        12

//LED DEBUG TEENSY
#define LED_TEENSY     13

//STATUS LED 5_SX3
#define STATUS_LED     5

//DIP SWITCH
#define MUX_S0         26         
#define MUX_S1         27
#define MUX_S2         28
#define MUX_S3         31
#define MUX_SIG       A13

//MUX I2C
#define MUX_SDA       18
#define MUX_SCL       19

#define INTERRUPT_PIN_SWITCH       13

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

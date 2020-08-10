/*
   Can Bus Client
   Sending Can Bus FD
   Sending Can TD 0
*/
#include <FlexCAN_T4.h>
#include "message.h"

#include <MatrixHardware_KitV4T4.h>     // Teensy 4 Wired to SmartLED Shield for Teensy 3 V4
#include <SmartMatrix3.h>

//CAN BUS FD
FlexCAN_T4FD<CAN3, RX_SIZE_256, TX_SIZE_64>   FD;   // fd port

//CAN BUS 1
FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16>     canBus;  // can1 port

#define COLOR_DEPTH 24                  // known working: 24, 48 - If the sketch uses type `rgb24` directly, COLOR_DEPTH must be 24

const uint16_t kMatrixWidth = 32;        // known working: 32, 64, 96, 128, 256
const uint16_t kMatrixHeight = 32;       // known working: 16, 32, 48, 64, 128
const uint8_t kRefreshDepth = 24;       // known working: 24, 36, 48 (on Teensy 4.x: 3, 6, 9, 12, 15, 18, 21, 24, 27, 30, 33, 36, 39, 42, 45, 48)
const uint8_t kDmaBufferRows = 2;       // known working: 2-4, use 2 to save memory, more to keep from dropping frames and automatically lowering refresh rate
const uint8_t kPanelType = SMARTMATRIX_HUB75_32ROW_MOD16SCAN;   // use SMARTMATRIX_HUB75_16ROW_MOD8SCAN for common 16x32 panels
const uint8_t kMatrixOptions = SMARTMATRIX_OPTIONS_NONE;   // see http://docs.pixelmatix.com/SmartMatrix for options
const uint8_t kBackgroundLayerOptions = (SM_BACKGROUND_OPTIONS_NONE);

SMARTMATRIX_ALLOCATE_BUFFERS(matrix, kMatrixWidth, kMatrixHeight, kRefreshDepth, kDmaBufferRows, kPanelType, kMatrixOptions);
SMARTMATRIX_ALLOCATE_BACKGROUND_LAYER(backgroundLayer, kMatrixWidth, kMatrixHeight, COLOR_DEPTH, kBackgroundLayerOptions);

const int defaultBrightness = (100*255)/100;        // full (100%) brightness
const rgb24 defaultBackgroundColor = {255, 255, 255};

int local_name = 0;
int msg_array[8] = {1, 2, 3, 4, 32, 6, 7, 8};

unsigned long timer;
int counter; 
bool change;

const int MOTOR_ID = 9;

//----------------------------------------------------------------
void setup(void) {
  Serial.begin(115200);
  delay(500);
  Serial.println("Client CAN Bus FD and CAN BUS 1");

  //setup CAN 0 BUS
  canBus.begin();
  canBus.setBaudRate(500000);     // 500kbps data rate
  canBus.enableFIFO();
  canBus.enableFIFOInterrupt();
  canBus.onReceive(FIFO, canBusSniff);
  canBus.mailboxStatus();

  //setup CAN FD bus
  FD.begin();
  CANFD_timings_t config;
  config.clock = CLK_24MHz;
  config.baudrate = 1000000;
  config.baudrateFD = 2000000;
  config.propdelay = 190;
  config.bus_length = 1;
  config.sample = 87.5;
  FD.setRegions(64);
  FD.setBaudRate(config);
  FD.mailboxStatus();
  
  Serial.println("Init client done");

  matrix.addLayer(&backgroundLayer); 
  matrix.begin();

  matrix.setBrightness(defaultBrightness);
  backgroundLayer.enableColorCorrection(true);
  for (int i =0; i<8; i++) {
    for (int j=0; j<12; j++) {
      backgroundLayer.drawPixel(i, j, {33, 32, 31});
      backgroundLayer.drawPixel(i+10, j, {33, 32, 31});
    }
  }
  backgroundLayer.swapBuffers();
  timer = millis();
  change = false;
}

//----------------------------------------------------------------
void loop() {
  if (counter==0) {
    timer=millis();
  }
  
  CANFD_message_t msg;
  if (FD.read(msg)) {
    counter++;
    Serial.println(counter);
    reading(msg);
    change=true;
    timer = millis();
  } else if (millis()-timer>600 && change==true) {
    backgroundLayer.swapBuffers();
    Serial.println("changed");
    change=false;
    timer=millis();
  }

  if (Serial.available() > 0) {
    test(Serial.parseInt());
  } 
}

//----------------------------------------------------------------
void canBusSniff(const CAN_message_t &msg) { // global callback
  
  Serial.print("T4: ");
  Serial.print("MB "); Serial.print(msg.mb);
  Serial.print(" OVERRUN: "); Serial.print(msg.flags.overrun);
  Serial.print(" BUS "); Serial.print(msg.bus);
  Serial.print(" LEN: "); Serial.print(msg.len);
  Serial.print(" EXT: "); Serial.print(msg.flags.extended);
  Serial.print(" REMOTE: "); Serial.print(msg.flags.remote);
  Serial.print(" TS: "); Serial.print(msg.timestamp);
  Serial.print(" ID: "); Serial.print(msg.id, HEX);
  Serial.print(" IDHIT: "); Serial.print(msg.idhit);
  
  Serial.print(" Buffer: ");
  for ( uint8_t i = 0; i < msg.len; i++ ) {
    Serial.print(msg.buf[i], HEX); Serial.print(" ");
  } Serial.println();  
}

//----------------------------------------------------------------
void reading(CANFD_message_t msg) {
    Serial.print("  Node: ");
    Serial.print(msg.id);
    Serial.print("  LEN: ");
    Serial.print(msg.len);
    Serial.print(" DATA: ");

    for ( uint8_t i = 0; i < msg.len; i++ ) {
      Serial.print(msg.buf[i]); 
      Serial.print(" ");
    }
    Serial.println(" ");
    Serial.println(" ");
    
    for ( uint8_t i = 0; i < 8; i++ ) {
      char str[15];
      sprintf(str, "%X%X", msg.buf[i*4+2], msg.buf[i*4+3]); 
      grid(msg.id, i, str, msg.buf[i*4]);
    }
}

//----------------------------------------------------------------
void grid(int node, int local, char* str, int height) {
  uint64_t color = (uint64_t) strtoull(str, 0, 16); 
  int r = ((((color >> 11) & 0x1F) * 527) + 23) >> 6;
  int g = ((((color >> 5) & 0x3F) * 259) + 33) >> 6;
  int b = (((color & 0x1F) * 527) + 23) >> 6;

  int x = (node%4)*2;
  int y = int(node/4)*4+int(local/2);
  
  if (local%2 == 1) {
    x = (node%4)*2 + 1;
  }
  backgroundLayer.drawPixel(x, y, {r,g,b});
  backgroundLayer.drawPixel(x+10, y, {height,height,255}); 
}

//----------------------------------------------------------------
void test(int node) {
  CANMotorMessage msg = CANMotorMessage(node);
  for (int i =0; i < 8; i++) {
    msg.addMessage(i, 10, 170, 105, 243);
  }
  FD.write(msg.getCANmessage());
  
  char str[15];
  sprintf(str, "%X%X", 105, 243); 
  for (int i =0; i < 8; i++) {
    grid(node,i,str,10);
  }
  change=true;
  timer = millis();
}

#include <FlexCAN_T4.h>
#include <string.h>
#include "message.h"
#include <Bounce2.h>

//NUM 3D PIXELS
#define NUM_PIXELS 8
#define MSG_SIZE 64
#define NUM_PANELS 12

FlexCAN_T4FD<CAN3, RX_SIZE_256, TX_SIZE_32> canBusFD;

//key pins
const int KEY_PIN_01 = 6;
const int KEY_PIN_02 = 7;
const int KEY_PIN_03 = 8;
const int KEY_PIN_04 = 9;

const int LED_15 = 22;
const int LED_16 = 23;

char operation;                 // Holds operation (R, W, ...)
int wait_for_transmission = 5;  // Delay in ms in order to receive the serial data

int nodesX = 2;
int currentNode = 0;

Bounce2::Button buttonKey_01 = Bounce2::Button();
Bounce2::Button buttonKey_02 = Bounce2::Button();
Bounce2::Button buttonKey_03 = Bounce2::Button();
Bounce2::Button buttonKey_04 = Bounce2::Button();

bool toogleON = false;
bool toogleDir = false;

//format for received pixel data
struct Pixel {
  byte inter;
  byte debug0;
  byte debug1;
  byte debug2;
  byte height;
  byte colorRed;
  byte colorGreen;
  byte colorBlue;
};

struct landType {
  uint8_t height;
  uint8_t red;
  uint8_t blue;
  uint8_t green;
  uint8_t white;
};

landType HR_0;
landType HR_1;
landType HR_2;

landType MR_0;
landType MR_1;

landType Street;
landType Parks;

int HIGHRise_0 = 220;
int HIGHRise_1 = 180;
int HIGHRise_2 = 150;

int MIDRise_0 = 50;
int MIDRise_1 = 25;
int PARKS = 30;
int STREET = 20;

//----------------------------------------------------------------
void setup(void) {
  Serial.begin(115200);
  delay(500);

  canBusFD.begin();
  CANFD_timings_t config;
  config.clock = CLK_40MHz;  //40 CLK_80MHz
  config.baudrate = 1000000;
  config.baudrateFD = 8000000;
  config.propdelay = 190;
  config.bus_length = 1;
  config.sample = 87.5;
  canBusFD.setRegions(64);
  canBusFD.setBaudRate(config);
  canBusFD.mailboxStatus();

  buttonKey_01.attach(KEY_PIN_01, INPUT_PULLUP);
  buttonKey_02.attach(KEY_PIN_02, INPUT_PULLUP);
  buttonKey_03.attach(KEY_PIN_03, INPUT_PULLUP);
  buttonKey_04.attach(KEY_PIN_04, INPUT_PULLUP);

  // DEBOUNCE INTERVAL IN MILLISECONDS
  buttonKey_01.interval(20);
  buttonKey_02.interval(20);
  buttonKey_03.interval(20);
  buttonKey_04.interval(20);

  // INDICATE THAT THE LOW STATE CORRESPONDS TO PHYSICALLY PRESSING THE BUTTON
  buttonKey_01.setPressedState(LOW);
  buttonKey_02.setPressedState(LOW);
  buttonKey_03.setPressedState(LOW);
  buttonKey_04.setPressedState(LOW);

  //
  pinMode(LED_15, OUTPUT);
  pinMode(LED_16, OUTPUT);

  Serial.println("Init server done");

  HR_0.height = 230;
  HR_1.height = 185;
  HR_2.height = 120;

  MR_0.height = 50;
  MR_1.height = 35;

  Street.height = 0;

  //
  HR_0.red = 220;
  HR_0.blue = 255;
  HR_0.green = 0;

  HR_1.red = 200;
  HR_1.blue = 255;
  HR_1.green = 50;

  HR_2.red = 200;
  HR_2.blue = 255;
  HR_2.green = 80;

  Street.red = 180;
  Street.blue = 220;
  Street.green = 235;

  MR_0.red = 180;
  MR_0.blue = 0;
  MR_0.green = 255;

  MR_1.red = 0;
  MR_1.blue = 255;
  MR_1.green = 255;


  Parks.height = 0;
}

//----------------------------------------------------------------
//command
//W{node_id},{local_id},{interaction},{height},{color1},{color2}E

void loop() {
  ///Key
  buttonKey_01.update();
  buttonKey_02.update();
  buttonKey_03.update();
  buttonKey_04.update();

  //key
  int keyPin01 = buttonKey_01.pressed();
  int keyPin02 = buttonKey_02.pressed();
  int keyPin03 = buttonKey_03.pressed();
  int keyPin04 = buttonKey_04.pressed();

  //digitalWrite(LED_15, keyPin01);
  //digitalWrite(LED_16, keyPin04);

  if (Serial.available() > 0) {
    //processSingleNode(currentNode);

    processAllNodes();
    //digitalWrite(LED_16, LOW);
    
  }

  if (keyPin01 == HIGH) {
    Serial.print("send key 01");
    toogleON = !toogleON;

    if (toogleON == true) {
      stop(0);
      stop(1);
      stop(2);
      stop(3);

      stop(6);
      stop(7);
      stop(8);
      stop(9);
      Serial.println(" STOP");
    } else {
     // start(0);
     // start(1);
      //start(2);
     // start(3);

      //start(6);
      //start(7);
      //start(8);
      //start(9);
      Serial.println(" START");
    }
  }

  if (keyPin02 == HIGH) {
    Serial.print("send Dir");

    sendSequence(0);
    delay(5);
    sendSequence(1);
    delay(5);
    sendSequence(2);
    delay(5);
    sendSequence(3);
    delay(5);
    sendSequence(6);
    delay(5);
    sendSequence(7);
    delay(5);
    sendSequence(8);
    delay(5);
    sendSequence(9);
    //[8]
    /*
    toogleDir = !toogleDir;

    if (toogleDir == true) {
      changeDirOn(0);
      changeDirOn(1);
      changeDirOn(2);
      changeDirOn(3);

      changeDirOn(6);
      changeDirOn(7);
      changeDirOn(8);
      changeDirOn(9);
      delay(10);
      Serial.println(" UP");
    } else {
      changeDirOff(0);
      changeDirOff(1);
      changeDirOff(2);
      changeDirOff(3);

      changeDirOff(6);
      changeDirOff(7);
      changeDirOff(8);
      changeDirOff(9);
      delay(10);
      Serial.println(" DOWN");
    }
    */
  }

  if (keyPin03 == HIGH) {
    Serial.println("send key 03");
    sendLandTypes(0);
    delay(5);
    sendLandTypes(1);
    delay(5);
    sendLandTypes(2);
    delay(5);
    sendLandTypes(3);
    delay(5);
    sendLandTypes(6);
    delay(5);
    sendLandTypes(7);
    delay(5);
   sendLandTypes(8);
    delay(5);
    sendLandTypes(9);
    delay(5);
  }

  if (keyPin04 == HIGH) {
    Serial.println("send key 04");
    toogleDir = !toogleDir;
    if (toogleDir == true) {
      changeDirOn(0);
      changeDirOn(1);
      changeDirOn(2);
      changeDirOn(3);

      changeDirOn(6);
      changeDirOn(7);
      changeDirOn(8);
      changeDirOn(9);
      delay(10);
      Serial.println(" UP");
    } else {
      changeDirOff(0);
      changeDirOff(1);
      changeDirOff(2);
      changeDirOff(3);

      changeDirOff(6);
      changeDirOff(7);
      changeDirOff(8);
      changeDirOff(9);
      delay(10);
      Serial.println(" DOWN");
    }


    test_white(0);
    test_white(1);
    test_white(2);
    test_white(3);

    test_white(6);
    test_white(7);
    test_white(8);
    test_white(9);

    /*
    currentNode++;
    if (currentNode >= 12) {
      currentNode = 0;
    }
    */
  }
}

//----------------------------------------------------------------
void processAllNodes() {
  byte buf[12 * 8 * 8 + 1];

  digitalWrite(LED_15, HIGH);
  int len = Serial.readBytes(buf, 12 * 8 * 8 + 1);
  digitalWrite(LED_16, HIGH);

  if (len > 0) {

    //get node value
    if (buf[0] == B00000001) {
      digitalWrite(LED_16, HIGH);
      for (int j = 0; j < 12; j++) {
        Pixel node[8];
        for (int i = 0; i < NUM_PIXELS; i++) {
          node[i].inter = buf[i * 8 + 0 + j * NUM_PIXELS + 1];


          node[i].debug0 = buf[i * 8 + 1 + j * NUM_PIXELS + 1];
          node[i].debug1 = buf[i * 8 + 2 + j * NUM_PIXELS + 1];
          node[i].debug2 = buf[i * 8 + 3 + j * NUM_PIXELS + 1];

          node[i].height = buf[i * 8 + 4 + j * NUM_PIXELS + 1];

          node[i].colorRed = buf[i * 8 + 5 + j * NUM_PIXELS + 1];
          node[i].colorGreen = buf[i * 8 + 6 + j * NUM_PIXELS + 1];
          node[i].colorBlue = buf[i * 8 + 7 + j * NUM_PIXELS + 1];
        }
        sendNode(j, node);
      }
      digitalWrite(LED_16, LOW);
    }
  }

  Serial.flush();
  for (int i = 0; i < 10; i++) {
    char f = Serial.read();
  }
  digitalWrite(LED_15, LOW);
}

//----------------------------------------------------------------
void processSingleNode(int nodeId) {
  byte buf[8 * 12 * 8 + 1];

  int len = Serial.readBytes(buf, 8 * 12 * 8 + 1);
  if (len > 0) {
    //get node value
    if (buf[0] == B00000001) {
      Pixel node[8];
      for (int i = 0; i < NUM_PIXELS; i++) {
        node[i].inter = buf[i * 8 + 0 + nodeId * NUM_PIXELS + 1];


        node[i].debug0 = buf[i * 8 + 1 + nodeId * NUM_PIXELS + 1];
        node[i].debug1 = buf[i * 8 + 2 + nodeId * NUM_PIXELS + 1];
        node[i].debug2 = buf[i * 8 + 3 + nodeId * NUM_PIXELS + 1];

        node[i].height = buf[i * 8 + 4 + nodeId * NUM_PIXELS + 1];

        node[i].colorRed = buf[i * 8 + 5 + nodeId * NUM_PIXELS + 1];
        node[i].colorGreen = buf[i * 8 + 6 + nodeId * NUM_PIXELS + 1];
        node[i].colorBlue = buf[i * 8 + 7 + nodeId * NUM_PIXELS + 1];
      }
      sendNode(nodeId, node);
    }
  }
}

//use this to send data to the table
void sendNode(int node_id, Pixel* buf) {
  CANMotorMessage msg = CANMotorMessage(node_id);
  for (int i = 0; i < NUM_PIXELS; i++) {
    msg.addMessage(i, buf[i].inter, buf[i].debug0, buf[i].debug1, buf[i].debug2, buf[i].height, buf[i].colorRed, buf[i].colorGreen, buf[i].colorBlue);
    //Serial.print()
  }
  canBusFD.write(msg.getCANmessage(MSG_SIZE));
}

//----------------------------------------------------------------
void test_white(int node_id) {
  CANMotorMessage msg = CANMotorMessage(node_id);
  for (int i = 0; i < NUM_PIXELS; i++) {
    msg.addMessage(i, 0, 0, 1, 0, 0, 255, 255, 255);
  }
  canBusFD.write(msg.getCANmessage(MSG_SIZE));
}
void test_blue(int node_id) {
  CANMotorMessage msg = CANMotorMessage(node_id);

  for (int i = 0; i < NUM_PIXELS; i++) {
    msg.addMessage(i, 0, 0, 1, 0, 0, 0, 0, 255);
  }
  canBusFD.write(msg.getCANmessage(MSG_SIZE));
}

//----------------------------------------------------------------
void stop(int node_id) {
  CANMotorMessage msg = CANMotorMessage(node_id);

  for (int i = 0; i < NUM_PIXELS; i++) {
    msg.addMessage(i, 0, 0, 0, 1, 0, 255, 255, 255);
  }
  canBusFD.write(msg.getCANmessage(MSG_SIZE));
}

//----------------------------------------------------------------
void start(int node_id) {
  CANMotorMessage msg = CANMotorMessage(node_id);

  for (int i = 0; i < NUM_PIXELS; i++) {
    msg.addMessage(i, 0, 1, 1, 0, 0, 255, 255, 255);
  }
  canBusFD.write(msg.getCANmessage(MSG_SIZE));
}

void changeDirOn(int node_id) {
  CANMotorMessage msg = CANMotorMessage(node_id);

  for (int i = 0; i < NUM_PIXELS; i++) {
    msg.addMessage(i, 0, 1, 0, 0, 0, 255, 255, 255);
  }
  canBusFD.write(msg.getCANmessage(MSG_SIZE));
}

void changeDirOff(int node_id) {
  CANMotorMessage msg = CANMotorMessage(node_id);

  for (int i = 0; i < NUM_PIXELS; i++) {
    msg.addMessage(i, 0, 0, 0, 0, 0, 255, 255, 255);
  }
  canBusFD.write(msg.getCANmessage(MSG_SIZE));
}

/*


    uint8_t msgs[8][8] = { { 1, 0, 0, 0, 35, 255, 0, 0 },
                           { 1, 0, 0, 0, 130, 0, 255, 0 },
                           { 1, 0, 0, 0, 35, 0, 0, 255 },
                           { 1, 0, 0, 0, 35, 255, 255, 255 },
                           { 1, 0, 0, 0, 0, 255, 0, 0 },
                           { 1, 0, 0, 0, 130, 0, 255, 0 },
                           { 1, 0, 0, 0, 0, 0, 0, 255 },
                           { 1, 0, 0, 0, 35, 255, 255, 255 } };

*/



void sendSequence(int id) {
  if (id == 0) {
    CANMotorMessage msg = CANMotorMessage(id);

    uint8_t msgs[8][8] = { { 1, 0, 0, 0, MR_0.height, MR_0.red, MR_0.green, MR_0.blue},
                           { 1, 0, 0, 0, HR_0.height, HR_0.red, HR_0.green, HR_0.blue  }, 
                           { 1, 0, 0, 0, MR_0.height, MR_0.red, MR_0.green, MR_0.blue },
                           { 1, 0, 0, 0, MR_0.height, MR_0.red, MR_0.green, MR_0.blue },
                           { 1, 0, 0, 0, Street.height, Street.red, Street.green, Street.blue },
                           { 1, 0, 0, 0, HR_0.height, HR_0.red, HR_0.green, HR_0.blue  },
                           { 1, 0, 0, 0, Street.height, Street.red, Street.green, Street.blue },
                           { 1, 0, 0, 0, MR_0.height, MR_0.red, MR_0.green, MR_0.blue } };


    for (int i = 0; i < NUM_PIXELS; i++) {
      msg.addMessage(i, msgs[i][0], msgs[i][1], msgs[i][2], msgs[i][3], msgs[i][4], msgs[i][5], msgs[i][6], msgs[i][7]);
    }
    canBusFD.write(msg.getCANmessage(MSG_SIZE));
  }

  if (id == 1) {
    CANMotorMessage msg = CANMotorMessage(id);

    uint8_t msgs[8][8] = { { 1, 0, 0, 0, MR_0.height, MR_0.red, MR_0.green, MR_0.blue },
                           { 1, 0, 0, 0, HR_0.height, HR_0.red, HR_0.green, HR_0.blue },
                           { 1, 0, 0, 0, MR_0.height, MR_0.red, MR_0.green, MR_0.blue },
                           { 1, 0, 0, 0, MR_0.height, MR_0.red, HR_0.green, HR_0.blue },
                           { 1, 0, 0, 0, MR_0.height, HR_0.red, HR_0.green, HR_0.blue },
                           { 1, 0, 0, 0, HR_0.height - HR_0.red, HR_0.green, HR_0.blue },
                           { 1, 0, 0, 0, Street.height, Street.red, Street.green, Street.blue },
                           { 1, 0, 0, 0, MR_0.height, MR_0.red, MR_0.green, MR_0.blue } };


    for (int i = 0; i < NUM_PIXELS; i++) {
      msg.addMessage(i, msgs[i][0], msgs[i][1], msgs[i][2], msgs[i][3], msgs[i][4], msgs[i][5], msgs[i][6], msgs[i][7]);
    }
    canBusFD.write(msg.getCANmessage(MSG_SIZE));
  }


  if (id == 2) {
    CANMotorMessage msg = CANMotorMessage(id);

    uint8_t msgs[8][8] = { { 1, 0, 0, 0, MR_1.height, MR_1.red, MR_1.green, MR_1.blue },
                           { 1, 0, 0, 0, Street.height, Street.red, Street.green, Street.blue },
                           { 1, 0, 0, 0, Street.height, Street.red, Street.green, Street.blue },
                           { 1, 0, 0, 0, Street.height, Street.red, Street.green, Street.blue },
                           { 1, 0, 0, 0, MR_0.height, MR_0.red, MR_0.green, MR_0.blue},
                           { 1, 0, 0, 0, HR_1.height, HR_1.red, HR_1.green, HR_1.blue },
                           { 1, 0, 0, 0, Street.height, Street.red, Street.green, Street.blue },
                           { 1, 0, 0, 0, Street.height, Street.red, Street.green, Street.blue } };


    for (int i = 0; i < NUM_PIXELS; i++) {
      msg.addMessage(i, msgs[i][0], msgs[i][1], msgs[i][2], msgs[i][3], msgs[i][4], msgs[i][5], msgs[i][6], msgs[i][7]);
    }
    canBusFD.write(msg.getCANmessage(MSG_SIZE));
  }


  if (id == 3) {
    CANMotorMessage msg = CANMotorMessage(id);

    uint8_t msgs[8][8] = { { 1, 0, 0, 0, MR_1.height, MR_1.red, MR_1.green, MR_1.blue },
                           { 1, 0, 0, 0, MR_1.height, MR_1.red, MR_1.green, MR_1.blue },
                           { 1, 0, 0, 0, Street.height, Street.red, Street.green, Street.blue },
                           { 1, 0, 0, 0, Street.height,Street.red, Street.green, Street.blue },
                           { 1, 0, 0, 0, MR_0.height, MR_0.red, MR_0.green, MR_0.blue },
                           { 1, 0, 0, 0, HR_2.height, HR_2.red, HR_2.green, HR_2.blue },
                           { 1, 0, 0, 0, MR_0.height, MR_0.red, MR_0.green, MR_0.blue },
                           { 1, 0, 0, 0, Street.height, Street.red, Street.green, Street.blue } };


    for (int i = 0; i < NUM_PIXELS; i++) {
      msg.addMessage(i, msgs[i][0], msgs[i][1], msgs[i][2], msgs[i][3], msgs[i][4], msgs[i][5], msgs[i][6], msgs[i][7]);
    }
    canBusFD.write(msg.getCANmessage(MSG_SIZE));
  }



  if (id == 6) {
    CANMotorMessage msg = CANMotorMessage(id);

    uint8_t msgs[8][8] = { { 1, 0, 0, 0, Street.height, Street.red, Street.green, Street.blue },
                           { 1, 0, 0, 0, HR_0.height, HR_0.red, HR_0.green, HR_0.blue },
                           { 1, 0, 0, 0, Street.height, Street.red, Street.green, Street.blue },
                           { 1, 0, 0, 0, MR_0.height, MR_0.red, MR_0.green, MR_0.blue },
                           { 1, 0, 0, 0, Street.height, Street.red, Street.green, Street.blue },
                           { 1, 0, 0, 0, HR_0.height, HR_0.red, HR_0.green, HR_0.blue },
                           { 1, 0, 0, 0, Street.height, Street.red, Street.green, Street.blue },
                           { 1, 0, 0, 0, MR_0.height,MR_0.red, MR_0.green, MR_0.blue } };


    for (int i = 0; i < NUM_PIXELS; i++) {
      msg.addMessage(i, msgs[i][0], msgs[i][1], msgs[i][2], msgs[i][3], msgs[i][4], msgs[i][5], msgs[i][6], msgs[i][7]);
    }
    canBusFD.write(msg.getCANmessage(MSG_SIZE));
  }



  if (id == 7) {
    CANMotorMessage msg = CANMotorMessage(id);

    uint8_t msgs[8][8] = { { 1, 0, 0, 0, Street.height, Street.red, Street.green, Street.blue },
                           { 1, 0, 0, 0, HR_1.height, HR_1.red, HR_1.green, HR_1.blue },
                           { 1, 0, 0, 0, Street.height, Street.red, Street.green, Street.blue },
                           { 1, 0, 0, 0, MR_0.height, MR_0.red, MR_0.green, MR_0.blue },
                           { 1, 0, 0, 0, Street.height, Street.red, Street.green, Street.blue },
                           { 1, 0, 0, 0, HR_1.height, HR_1.red, HR_1.green, HR_1.blue },
                           { 1, 0, 0, 0, MR_1.height, MR_1.red, MR_1.green, MR_1.blue },
                           { 1, 0, 0, 0, MR_0.height, MR_0.red, MR_0.green, MR_0.blue } };


    for (int i = 0; i < NUM_PIXELS; i++) {
      msg.addMessage(i, msgs[i][0], msgs[i][1], msgs[i][2], msgs[i][3], msgs[i][4], msgs[i][5], msgs[i][6], msgs[i][7]);
    }
    canBusFD.write(msg.getCANmessage(MSG_SIZE));
  }


  if (id == 8) {
    CANMotorMessage msg = CANMotorMessage(id);

    uint8_t msgs[8][8] = { { 1, 0, 0, 0, MR_1.height, MR_1.red, MR_1.green, MR_1.blue },
                           { 1, 0, 0, 0, MR_1.height, MR_1.red, MR_1.green, MR_1.blue },
                           { 1, 0, 0, 0, MR_1.height, MR_1.red, MR_1.green, MR_1.blue },
                           { 1, 0, 0, 0, MR_1.height, MR_1.red, MR_1.green, MR_1.blue },
                           { 1, 0, 0, 0, MR_1.height, MR_1.red, MR_1.green, MR_1.blue },
                           { 1, 0, 0, 0, MR_1.height, MR_1.red, MR_1.green, MR_1.blue },
                           { 1, 0, 0, 0, Street.height, Street.red, Street.green, Street.blue },
                           { 1, 0, 0, 0, MR_1.height, MR_1.red, MR_1.green, MR_1.blue } };


    for (int i = 0; i < NUM_PIXELS; i++) {
      msg.addMessage(i, msgs[i][0], msgs[i][1], msgs[i][2], msgs[i][3], msgs[i][4], msgs[i][5], msgs[i][6], msgs[i][7]);
    }
    canBusFD.write(msg.getCANmessage(MSG_SIZE));
  }


  if (id == 9) {
    CANMotorMessage msg = CANMotorMessage(id);

    uint8_t msgs[8][8] = { { 1, 0, 0, 0, HR_2.height,HR_2.red, HR_2.green, HR_2.blue },
                           { 1, 0, 0, 0, Street.height, Street.red, Street.green, Street.blue },
                           { 1, 0, 0, 0, MR_0.height, MR_0.red, MR_0.green, MR_0.blue },
                           { 1, 0, 0, 0, Street.height, Street.red, Street.green, Street.blue },
                           { 1, 0, 0, 0, HR_2.height,HR_2.red, HR_2.green, HR_2.blue },
                           { 1, 0, 0, 0, Street.height, Street.red, Street.green, Street.blue },
                           { 1, 0, 0, 0, MR_0.height,MR_0.red, MR_0.green, MR_0.blue},
                           { 1, 0, 0, 0, Street.height, Street.red, Street.green, Street.blue} };


    for (int i = 0; i < NUM_PIXELS; i++) {
      msg.addMessage(i, msgs[i][0], msgs[i][1], msgs[i][2], msgs[i][3], msgs[i][4], msgs[i][5], msgs[i][6], msgs[i][7]);
    }
    canBusFD.write(msg.getCANmessage(MSG_SIZE));
  }
}

void sendLandTypes(int id) {
  if (id == 0) {
    CANMotorMessage msg = CANMotorMessage(id);

    uint8_t msgs[8][8] = { { 0, 0, 0, 0, MR_0.height, MR_0.red, MR_0.green, MR_0.blue },
                           { 0, 0, 0, 0, HR_0.height, HR_0.red, HR_0.green, HR_0.blue },
                           { 0, 0, 0, 0, MR_0.height, MR_0.red, MR_0.green, MR_0.blue },
                           { 0, 0, 0, 0, MR_0.height, MR_0.red, MR_0.green, MR_0.blue },
                           { 0, 0, 0, 0, Street.height, Street.red, Street.green, Street.blue },
                           { 0, 0, 0, 0, HR_0.height, HR_0.red, HR_0.green, HR_0.blue },
                           { 0, 0, 0, 0, Street.height, Street.red, Street.green, Street.blue },
                           { 0, 0, 0, 0, MR_0.height, MR_0.red, MR_0.green, MR_0.blue } };


    for (int i = 0; i < NUM_PIXELS; i++) {
      msg.addMessage(i, msgs[i][0], msgs[i][1], msgs[i][2], msgs[i][3], msgs[i][4], msgs[i][5], msgs[i][6], msgs[i][7]);
    }
    canBusFD.write(msg.getCANmessage(MSG_SIZE));
  }

  if (id == 1) {
    CANMotorMessage msg = CANMotorMessage(id);

    uint8_t msgs[8][8] = { { 0, 0, 0, 0, MR_0.height, MR_0.red, MR_0.green, MR_0.blue },
                           { 0, 0, 0, 0, HR_0.height, HR_0.red, HR_0.green, HR_0.blue },
                           { 0, 0, 0, 0, MR_0.height, MR_0.red, MR_0.green, MR_0.blue },
                           { 0, 0, 0, 0, MR_0.height, MR_0.red, MR_0.green, MR_0.blue },
                           { 0, 0, 0, 0, MR_0.height, MR_0.red, MR_0.green, MR_0.blue },
                           { 0, 0, 0, 0, HR_0.height - 20, HR_0.red, HR_0.green, HR_0.blue },
                           { 0, 0, 0, 0, Street.height, Street.red, Street.green, Street.blue },
                           { 0, 0, 0, 0, MR_0.height, MR_0.red, MR_0.green, MR_0.blue } };


    for (int i = 0; i < NUM_PIXELS; i++) {
      msg.addMessage(i, msgs[i][0], msgs[i][1], msgs[i][2], msgs[i][3], msgs[i][4], msgs[i][5], msgs[i][6], msgs[i][7]);
    }
    canBusFD.write(msg.getCANmessage(MSG_SIZE));
  }


  if (id == 2) {
    CANMotorMessage msg = CANMotorMessage(id);

    uint8_t msgs[8][8] = { { 0, 0, 0, 0, MR_1.height, MR_1.red, MR_1.green, MR_1.blue },
                           { 0, 0, 0, 0, Street.height, Street.red, Street.green, Street.blue },
                           { 0, 0, 0, 0, Street.height, Street.red, Street.green, Street.blue },
                           { 0, 0, 0, 0, Street.height, Street.red, Street.green, Street.blue },
                           { 0, 0, 0, 0, MR_0.height, MR_0.red, MR_0.green, MR_0.blue },
                           { 0, 0, 0, 0, HR_1.height, HR_1.red, HR_1.green, HR_1.blue },
                           { 0, 0, 0, 0, Street.height, Street.red, Street.green, Street.blue },
                           { 0, 0, 0, 0, Street.height, Street.red, Street.green, Street.blue } };


    for (int i = 0; i < NUM_PIXELS; i++) {
      msg.addMessage(i, msgs[i][0], msgs[i][1], msgs[i][2], msgs[i][3], msgs[i][4], msgs[i][5], msgs[i][6], msgs[i][7]);
    }
    canBusFD.write(msg.getCANmessage(MSG_SIZE));
  }


  if (id == 3) {
    CANMotorMessage msg = CANMotorMessage(id);

    uint8_t msgs[8][8] = { { 0, 0, 0, 0, MR_1.height, MR_1.red, MR_1.green, MR_1.blue },
                           { 0, 0, 0, 0, MR_1.height, MR_1.red, MR_1.green, MR_1.blue },
                           { 0, 0, 0, 0, Street.height, Street.red, Street.green, Street.blue },
                           { 0, 0, 0, 0, Street.height, Street.red, Street.green, Street.blue },
                           { 0, 0, 0, 0, MR_0.height, MR_0.red, MR_0.green, MR_0.blue },
                           { 0, 0, 0, 0, HR_2.height, HR_2.red, HR_2.green, HR_2.blue },
                           { 0, 0, 0, 0, MR_0.height, MR_0.red, MR_0.green, MR_0.blue },
                           { 0, 0, 0, 0, Street.height, Street.red, Street.green, Street.blue } };


    for (int i = 0; i < NUM_PIXELS; i++) {
      msg.addMessage(i, msgs[i][0], msgs[i][1], msgs[i][2], msgs[i][3], msgs[i][4], msgs[i][5], msgs[i][6], msgs[i][7]);
    }
    canBusFD.write(msg.getCANmessage(MSG_SIZE));
  }



  if (id == 6) {
    CANMotorMessage msg = CANMotorMessage(id);

    uint8_t msgs[8][8] = { { 0, 0, 0, 0, Street.height, Street.red, Street.green, Street.blue },
                           { 0, 0, 0, 0, HR_0.height, HR_0.red, HR_0.green, HR_0.blue },
                           { 0, 0, 0, 0, Street.height, Street.red, Street.green, Street.blue },
                           { 0, 0, 0, 0, MR_0.height, MR_0.red, MR_0.green, MR_0.blue },
                           { 0, 0, 0, 0, Street.height, Street.red, Street.green, Street.blue },
                           { 0, 0, 0, 0, HR_0.height, HR_0.red, HR_0.green, HR_0.blue },
                           { 0, 0, 0, 0, Street.height, Street.red, Street.green, Street.blue },
                           { 0, 0, 0, 0, MR_0.height, MR_0.red, MR_0.green, MR_0.blue } };


    for (int i = 0; i < NUM_PIXELS; i++) {
      msg.addMessage(i, msgs[i][0], msgs[i][1], msgs[i][2], msgs[i][3], msgs[i][4], msgs[i][5], msgs[i][6], msgs[i][7]);
    }
    canBusFD.write(msg.getCANmessage(MSG_SIZE));
  }



  if (id == 7) {
    CANMotorMessage msg = CANMotorMessage(id);

    uint8_t msgs[8][8] = { { 0, 0, 0, 0, Street.height, Street.red, Street.green, Street.blue },
                           { 0, 0, 0, 0, HR_1.height, HR_1.red, HR_1.green, HR_1.blue },
                           { 0, 0, 0, 0, Street.height, Street.red, Street.green, Street.blue },
                           { 0, 0, 0, 0, MR_0.height, MR_0.red, MR_0.green, MR_0.blue },
                           { 0, 0, 0, 0, Street.height, Street.red, Street.green, Street.blue },
                           { 0, 0, 0, 0, HR_1.height, HR_1.red, HR_1.green, HR_1.blue },
                           { 0, 0, 0, 0, MR_1.height, MR_1.red, MR_1.green, MR_1.blue },
                           { 0, 0, 0, 0, MR_0.height, MR_0.red, MR_0.green, MR_0.blue } };


    for (int i = 0; i < NUM_PIXELS; i++) {
      msg.addMessage(i, msgs[i][0], msgs[i][1], msgs[i][2], msgs[i][3], msgs[i][4], msgs[i][5], msgs[i][6], msgs[i][7]);
    }
    canBusFD.write(msg.getCANmessage(MSG_SIZE));
  }


  if (id == 8) {
    CANMotorMessage msg = CANMotorMessage(id);

    uint8_t msgs[8][8] = { { 0, 0, 0, 0, MR_1.height, MR_1.red, MR_1.green, MR_1.blue },
                           { 0, 0, 0, 0, MR_1.height, MR_1.red, MR_1.green, MR_1.blue },
                           { 0, 0, 0, 0, MR_1.height, MR_1.red, MR_1.green, MR_1.blue },
                           { 0, 0, 0, 0, MR_1.height, MR_1.red, MR_1.green, MR_1.blue },
                           { 0, 0, 0, 0, MR_1.height, MR_1.red, MR_1.green, MR_1.blue },
                           { 0, 0, 0, 0, MR_1.height, MR_1.red, MR_1.green, MR_1.blue },
                           { 0, 0, 0, 0, Street.height, Street.red, Street.green, Street.blue }, //change because error
                           { 0, 0, 0, 0, MR_1.height, MR_1.red, MR_1.green, MR_1.blue } };


    for (int i = 0; i < NUM_PIXELS; i++) {
      msg.addMessage(i, msgs[i][0], msgs[i][1], msgs[i][2], msgs[i][3], msgs[i][4], msgs[i][5], msgs[i][6], msgs[i][7]);
    }
    canBusFD.write(msg.getCANmessage(MSG_SIZE));
  }


  if (id == 9) {
    CANMotorMessage msg = CANMotorMessage(id);

    uint8_t msgs[8][8] = { { 0, 0, 0, 0, HR_2.height, HR_2.red, HR_2.green, HR_2.blue },
                           { 0, 0, 0, 0, Street.height, Street.red, Street.green, Street.blue },
                           { 0, 0, 0, 0, MR_0.height, MR_0.red, MR_0.green, MR_0.blue },
                           { 0, 0, 0, 0, Street.height, Street.red, Street.green, Street.blue },
                           { 0, 0, 0, 0, HR_2.height, HR_2.red, HR_2.green, HR_2.blue },
                           { 0, 0, 0, 0, Street.height, Street.red, Street.green, Street.blue },
                           { 0, 0, 0, 0, MR_0.height, MR_0.red, MR_0.green, MR_0.blue },
                           { 0, 0, 0, 0, Street.height, Street.red, Street.green, Street.blue } };


    for (int i = 0; i < NUM_PIXELS; i++) {
      msg.addMessage(i, msgs[i][0], msgs[i][1], msgs[i][2], msgs[i][3], msgs[i][4], msgs[i][5], msgs[i][6], msgs[i][7]);
    }
    canBusFD.write(msg.getCANmessage(MSG_SIZE));
  }
}

/*
    uint8_t msgs[8][8] = { { 0, 0, 0, 0, HR_2.height, HR_2.red, HR_2.green, HR_2.blue },
                           { 0, 0, 0, 0, Street.height, Street.red, Street.green, Street.blue },
                           { 0, 0, 0, 0, MR_0.height, MR_0.red, MR_0.green, MR_0.blue },
                           { 0, 0, 0, 0, Street.height, Street.red, Street.green, Street.blue },
                           { 0, 0, 0, 0, HR_2.height, HR_2.red, HR_2.green, HR_2.blue },
                           { 0, 0, 0, 0, Street.height, Street.red, Street.green, Street.blue },
                           { 0, 0, 0, 0, MR_0.height, MR_0.red, MR_0.green, MR_0.blue },
                           { 0, 0, 0, 0, Street.height, Street.red, Street.green, Street.blue } };
*/
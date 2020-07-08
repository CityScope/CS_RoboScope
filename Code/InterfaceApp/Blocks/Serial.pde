import processing.serial.*;

Serial canPort01; 

/*
 * Teensy available speeds:
 * https://www.pjrc.com/teensy/td_uart.html
 * 
 * 2000000
 * 1000000
 * 250000 
 * 115200
 * 38400
*/

int bauRate = 1000000;

/*
  Serial manager
*/
void setupSerial() {

  //print serials
  printArray(Serial.list());

  String portName = Serial.list()[0];
  canPort01 = new Serial(this, portName, bauRate);
}

//send test Msg
void sendTestMsg() {
  
}

/*
*  Matrix with height
*  Matrix with color
*/

//send can Msg
void sendCanMsgTest() {

  //write array of bytes[]
  byte [] heights = new byte[ gridX * gridY ];

  //test
  for (int i = 0; i < gridX * gridY; i++) {
    heights[i] = (byte) int(random(0, 1024));
  }

  //heights
  canPort01.write(heights);
}

///send global heights
void sendHeights(){
  
}


void sendColors(){
  
}

void getMaxHeight(){
  
}


//send single heights

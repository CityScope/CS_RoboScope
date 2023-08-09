import processing.serial.*;

Serial myPort; 
PGraphics board;
PGraphics info; 
String val;
boolean firstContact = false; // Checks if we've heard from the microcontroller
String mode = "density"; 
color[] colors = {color(255, 255, 0), color(255, 0, 0), color(160, 32, 240), color(0, 0, 255), color(190, 190, 190), color(47, 79, 79), color(144, 238, 144), color(34, 139, 34), color(255, 255, 255)}; // these colors are not permanent. 
// left to right: yellow, red, purple, blue, gray, dark gray, light green, forest green, white.
color currentColor = color(255, 0, 0);
int color_index = 0;
int currDensity = 0; // will need to grab this data from the node. 
boolean up = false;
boolean motor_on = false; 
//final byte RETURN = 0b00001010;
//final byte READY = 0b00000000;
//final byte SEND = 0b00000001;
//final byte RECEIVE = 0b00000010; 
//byte[] interactionArray = {SEND, RECEIVE, RETURN, READY};
boolean SEND = false;
int interactionIndex = 0; 
byte interaction = 0;
byte modeByte = 0;
byte motorByte = 0;
byte[] buffer = new byte[12 * 8 * 8]; // 12 nodes, 8 3D pixels per node, 8 bytes per 3D pixel. 
byte nodeId = 0; // goes from 0 to 11
byte pixelId = 0; // goes from 0 to 7
int iterations = 0;

//--------------------------------------------------------------------------------------------

float sqX = 0;
float sqY = 0;
float sqZ = 0;
int localId; 
float[][] pixelCoords = new float[96][2]; //one index for each pixel. each index contains a tuple that contains coordinates eventually. 
int coordsIndex = 0;
boolean change;

void setup() {
  fullScreen(P3D);
  background(100);
  board = createGraphics(1080, 720, P3D);
  info = createGraphics(150, 100, P2D);
  String portName = Serial.list()[Serial.list().length - 1];
  myPort = new Serial(this, portName, 115200); // Parent, port name, baud rate
  myPort.bufferUntil('\n'); // reads the buffer until it reaches end of line character.
  
}

void draw() {
  // these are the individual cells. Eventually, we'll want to put all this functionality into a function.
  board.beginDraw();
  board.stroke(255);
  board.textSize(24);
  info.textSize(12); 
  info.textAlign(CENTER);
  board.textAlign(CENTER);
  board.noFill();
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 12; j++) {
      if (i == 0) {
        localId = j;
        pixelCoords[j][0] = sqX;
        pixelCoords[j][1] = sqY;
      } else {
        localId = 12 * i + j;
        pixelCoords[12 * i + j][0] = sqX;
        pixelCoords[12 * i + j][1] = sqY;
      }
      // If mouse is within a square, draw it with the color selected. If not, just draw the box.
      if ((pmouseX - width/2 + 415) >= sqX && (pmouseX - width/2 + 415) < sqX + 60) {
        if ((pmouseY - height/2 + 275) >= sqY && (pmouseY - height/2 + 275) < sqY + 60) {
          if (i == 0) {
            // draw square with fill
            board.fill(currentColor);
            board.square(sqX, sqY, 60);
            board.fill(255);
            board.text(str(localId), sqX + 30, sqY + 30);
            board.noFill();
            
            //-----------------------------------------------------------------------
            // Extracting the node IDs from each of the pixels. 
            if (localId % 12 < 2) {
              nodeId = 0; 
            } else if (localId % 12 < 4) {
              nodeId = 1;
            } else if (localId % 12 < 6) {
              nodeId = 2;
            } else if (localId % 12 < 8) {
              nodeId = 3;
            } else if (localId % 12 < 10) {
              nodeId = 4;           
            } else {
              nodeId = 5;   
            }           
    
            //-----------------------------------------------------------------------
            
          } else { // if i > 1
            // draw square with fill according to the color selector
            board.fill(currentColor);
            board.square(sqX, sqY, 60);
            board.fill(255);
            board.text(str(localId), sqX + 30, sqY + 30);
            board.noFill();
            
            
            //-----------------------------------------------------------------------
            // Extracting the node  IDs from each of the pixels. 
            if (localId % 12 < 2) {
              if (localId < 48) {
                nodeId = 0;               
              } else {
                nodeId = 6;
              }
              
            } else if (localId % 12 < 4) {
              if (localId < 48) {
                nodeId = 1;
              } else {
                nodeId = 7;
              }
            } else if (localId % 12 < 6) {
              if (localId < 48) {
                nodeId = 2;
              } else {
                nodeId = 8;
              }
            } else if (localId % 12 < 8) {
              if (localId < 48) {
                nodeId = 3;
              } else {
                nodeId = 9;
              }
            } else if (localId % 12 < 10) {
              if (localId < 48) {
                nodeId = 4;
              } else {
                nodeId = 10;
              }           
            } else {
              if (localId < 48) {
                nodeId = 5;
              } else {
              nodeId = 11;
              }          
            }
          }
          
          // Getting the pixel IDs of all of the pixels in each node. 
          if (localId < 48) {
              if (localId % 2 == 0) {
                pixelId = byte(2 * i);
              } else {
                pixelId = byte(2 * i + 1);
              }
            } else {
              if (localId % 2 == 0) {
                pixelId = byte(2 * (i - 4));
              } else {
                pixelId = byte(2 * (i - 4) + 1);
              }
            }
          // compare the colors right here...if they are different from the previous values, change them. Also, this could be where we send that new buffer to the board?
          if ((byte(currentColor >> 16 & 0xFF) != buffer[5 + (8 * 8) * nodeId + 8 * pixelId]) || (byte(currentColor >> 8 & 0xFF) != buffer[6 + (8 * 8) * nodeId + 8 * pixelId]) || (byte(currentColor & 0xFF) != buffer[7 + (8 * 8) * nodeId + 8 * pixelId])) {
            buffer[5 + (8 * 8) * nodeId + 8 * pixelId] = byte(currentColor >> 16 & 0xFF);
            buffer[6 + (8 * 8) * nodeId + 8 * pixelId] = byte(currentColor >> 8 & 0xFF);
            buffer[7 + (8 * 8) * nodeId + 8 * pixelId] = byte(currentColor & 0xFF);
            drawBuffer(byte(nodeId), byte(pixelId), byte(0), byte(0), byte(motorByte), byte(modeByte), byte(0), byte(currentColor >> 16 & 0xFF), byte(currentColor >> 8 & 0xFF), byte(currentColor & 0xFF)); 
            change = true;
          }          
          //Need to figure out how to fix the text on the screen. It is just drawing over the previous text instead of replacing it. 
          text("Node ID: " + str(nodeId), 75, 75);
          text("Pixel ID: " + str(pixelId), 75, 100);
          
        }
      } else { // if mouse is not within the box
        // draw square with no fill
        board.square(sqX, sqY, 60);
        board.text(str(localId), sqX + 30, sqY + 30);
      }
      sqX = sqX + 70;
    }
    
    sqX = 0;
    sqY = sqY + 70;
  }
  
  if (change) {
    myPort.write(buffer);
    println("Buffer sent");
    change = false;
  }
  sqX = 0;
  sqY = 0;
  board.endDraw();
  image(board, width/2 - 415, height/2 - 275);
}

//--------------------------------------------------------------------------------------------

void keyPressed() {
  if (key == 48) { // Ascii value of 0
    nodeId++;
    nodeId = byte(nodeId % 12);
    println("Node: " + nodeId + ". Pixel: " + pixelId);
  }
  
  else if(keyCode == SHIFT) {
    pixelId++;
    pixelId = byte(pixelId % 8);
    println("Node: " + nodeId + ". Pixel: " + pixelId);
  }
  
  else if (key == 49) { // 1
    //enable or disable motors
    motor_on = !motor_on; 
    if (motor_on) {
      println("motor on");
      motorByte = 1;
    }
    else {
      println("motor off");
      motorByte = 0;
    }
  } 
  
  else if (key == 50) { // 2
    //Send or no send
    SEND = !SEND;
    if (SEND) {
      interaction = 0b00000001;
      println("sending!");
    } else {
      interaction = 0b00000000;
      println("not sending!");
    }
  } 
  
  else if (key == 51) { // 3
    // load buffer 
    byte density = byte(currDensity & 0xFF);
    byte[] currColor = {byte(currentColor >> 16 & 0xFF), byte(currentColor >> 8 & 0xFF), byte(currentColor & 0xFF)};
   
    //buffer = drawBuffer(nodeId, pixelId, byte(0), byte(0), motorByte, modeByte, density, currColor[0], currColor[1], currColor[2]);
    for (int i = 0; i < 12; i++) {
      for (int j = 0; j < 8; j++) {
        print("Node " + i + ", Pixel " + j + ": " + buffer[(8 * 8) * i + 8 * j] + " ");
        print(buffer[(8 * 8) * i + 8 * j + 1] + " ");
        print(buffer[(8 * 8) * i + 8 * j + 2] + " ");
        print(buffer[(8 * 8) * i + 8 * j + 3] + " ");
        print(buffer[(8 * 8) * i + 8 * j + 4] + " ");
        print(buffer[(8 * 8) * i + 8 * j + 5] + " ");
        print(buffer[(8 * 8) * i + 8 * j + 6] + " ");
        print(buffer[(8 * 8) * i + 8 * j + 7] + " ");
        println();
      }
    }
  }
  
  else if (key == 52) { // 4
    // sets mode. Mode can either be density mode or color mode
    mode = changeMode(mode);
    if (mode == "color") {
      modeByte = 0;
    }
    else {
      modeByte = 1;
    }
  }
  
  else if (key == 53) { // 5
    // depending on the mode, this can either change the color or the density 
    
    // for density...once for up, again for down
    // for color...iterate through the colors.
    if (mode == "color") {
      // change the color
      currentColor = changeColor(currentColor, color_index);
      color_index++;
      color_index %= 9; // wraps around. There are 9 colors to choose from according to the LBCS color codes. *
      //* Thomas tweaked these colors a bit, but these are just placeholders for the time being.
      println((currentColor >> 16) & 0xFF, (currentColor >> 8) & 0xFF, currentColor & 0xFF);
    }
    
    else if (mode == "density") {
      // change the density
      currDensity = changeDensity(currDensity, up);
      println(currDensity);
    }
  }
  
  else if (key == 54) { // 6
  
    if (interaction == 1) {
      myPort.write(buffer);
      println("Buffer sent");
      //print(buffer);
    } else {
      println("Not sending right now/send is set to 0");
    }
  }
  
  else if (key == 55) { // 7
  // load buffer?
  }
  
  else if (key == 56) { // 8
  
  }
  
  else if (key == 57) { // 9 
  
  }

  if (mode == "density" && (key == 'T' || key == 't')) {
      up = !up;
      println(up);
    }
    
}


//--------------------------------------------------------------------------------------------

color changeColor(color currentColor, int color_index) {
  currentColor = colors[color_index];
  return currentColor;
}

//--------------------------------------------------------------------------------------------

int changeDensity(int currDensity, boolean up) {
  if (up) {
    if (currDensity < 255) {
      currDensity++;
    }
  }
  
  else {
    if (currDensity > 0) {
      currDensity--;
    }
  }
  
  return currDensity;
}

//--------------------------------------------------------------------------------------------

String changeMode(String mode) {
  if (mode == "density") {
    mode = "color";
    println("Mode: color.");
  }
  
  else {
    mode = "density";
    println("Mode: density. t for up, t again for down."); // t is just a holder variable. Could be cahnged to something different.
  }
  
  return mode;
}

//--------------------------------------------------------------------------------------------

byte[] drawBuffer(byte nodeId, byte pixelId, byte b1, byte b2, byte motorByte, byte modeByte, byte density, byte R, byte G, byte B) { // node id is not sent with the buffer 
  buffer[0 + (8 * 8) * nodeId + 8 * pixelId] = b1;
  buffer[1 + (8 * 8) * nodeId + 8 * pixelId] = b2;
  buffer[2 + (8 * 8) * nodeId + 8 * pixelId] = motorByte;
  buffer[3 + (8 * 8) * nodeId + 8 * pixelId] = modeByte;
  buffer[4 + (8 * 8) * nodeId + 8 * pixelId] = density;
  buffer[5 + (8 * 8) * nodeId + 8 * pixelId] = R;
  buffer[6 + (8 * 8) * nodeId + 8 * pixelId] = G;
  buffer[7 + (8 * 8) * nodeId + 8 * pixelId] = B;
  
  return buffer;
}

//--------------------------------------------------------------------------------------------



void serialEvent(Serial myPort) {
  val = myPort.readStringUntil('\n'); // Reads the value "Good" that has been printed to the serial port. 
  
  if (val != null) { // make sure data isn't empty
    val = trim(val); 
    println(val); // this is being printed. val = "Good"
    
    // look for "Good" in order to start the handshake.
    // if there, clear buffer and send request for data. 
    if (firstContact == false) {
      if (val == "Good") {
        myPort.clear();
        firstContact = true;
        myPort.write(interaction); // What do we want to do with the data
        println("contact");
      }
    }
    
    else { // if we've already established contact
      println(val);
      keyPressed();   
      myPort.write("Good"); 
    }
  }
}

import processing.serial.*;

final byte RETURN = 0b00001010;
final byte READY = 0b00000000;
final byte SEND = 0b00000001;
final byte RECEIVE = 0b00000010;
final int BAUD = 115200;
final boolean DEBUG = true;

class SerialHandler {

  public Serial port;

  SerialHandler(PApplet p) throws Exception {
    this.connect(p);
  }

  public void connect(PApplet p) throws Exception {
    String[] portList = Serial.list();
    String portName = portList[portList.length - 1];
    try {
      this.port = new Serial(p, portName, BAUD);
    }
    catch (Exception e) {
      println("Error connecting to Serial :" + e);
      throw e;
    }

    println("connected to: " + portName);
  }

  public boolean checkReady() throws Exception {
    while (this.port.available() > 0) {
      byte[] incomming = new byte[]{0, 0, 0};
      incomming = this.port.readBytes(3);
      // printArray(incomming);
      try {
        return incomming[0] == READY && incomming[2] == RETURN;
      }
      catch (Exception e) {
        return false;
      }
    }
    return false;
  }

  public byte getByte() throws Exception {
    while (this.port.available() > 0) {
      byte[] incomming = new byte[]{0, 0, 0};
      incomming = this.port.readBytes(3);
      if (DEBUG) {
        printArray(incomming);
      }

      if (incomming.length < 3) {
        throw new Exception("index out of bounds: message length" + incomming.length);
      }

      if (incomming[0] == RECEIVE && incomming[2] ==RETURN) {
        return (byte)incomming[1];
      } else {
        throw new Exception("message wrong protocol");
      }
    }

    throw new Exception("invalid message");
  }

  // byte []  generateDataHC(color [] col, int [] heights) {
  //byte [] bytes = new byte[];
  //return bytes;
  // }

  byte []  generateDataOnlyCol(color [] col) {
    byte [] bytes = new byte[ col.length*8 +1];
    println("sending bytes "+ col.length*8 +1);
    bytes[0] = SEND;
    for (int i = 0; i < col.length; i++) {
      bytes[i*8 + 0 +1]= 0;
      bytes[i*8 + 1 +1] = byte(255);
      bytes[i*8 + 2 +1] = byte(0); //

     /* if (brightness(col[i]) >255) {
        bytes[i*8 + 2 +1] = byte(1); //enable motor
      } else {
        bytes[i*8 + 2 +1] = byte(0);
      }

      if (brightness(col[i]) == 0) {
        bytes[i*8 + 2 +1] = byte(1); //disable motor
      } else {
        bytes[i*8 + 3 +1] = byte(0);
      }
      */

      bytes[i*8 + 4 +1] = byte(0); //motor height
      bytes[i*8 + 5 +1] = byte(col[i] >> 16 & 0xFF);
      bytes[i*8 + 6 +1] = byte(col[i] >> 8 & 0xFF);
      bytes[i*8 + 7 +1] = byte(col[i] & 0xFF);
    }
    return bytes;
  }

  byte [] stopMotors(int numBlocks, int numPixels) {
    int  numPix = int(numBlocks * numPixels);

    byte [] bytes = new byte[ numPix *8 +1];
    println("sending bytes "+ numPix*8 +1);
    bytes[0] = SEND;
    for (int i = 0; i < numPix; i++) {
      bytes[i*8 + 0 +1]= 0;
      bytes[i*8 + 1 +1] = byte(0); //motor start
      bytes[i*8 + 2 +1] = byte(0); //enable 1
      bytes[i*8 + 3 +1] = byte(1); //stop motor
      bytes[i*8 + 4 +1] = byte(0); //motor height
      bytes[i*8 + 5 +1] = byte(0); //color 0
      bytes[i*8 + 6 +1] = byte(0); //color 1
      bytes[i*8 + 7 +1] = byte(0); //color 2
    }
    return bytes;
  }

  byte [] statMotors(int numBlocks, int numPixels) {
    int  numPix = int(numBlocks * numPixels);

    byte [] bytes = new byte[ numPix *8 +1];
    println("sending bytes "+ numPix*8 +1);
    bytes[0] = SEND;
    for (int i = 0; i < numPix; i++) {
      bytes[i*8 + 0 +1]= 0;
      bytes[i*8 + 1 +1] = byte(0); //motor start
      bytes[i*8 + 2 +1] = byte(1); //enable 1
      bytes[i*8 + 3 +1] = byte(0); //stop motor
      bytes[i*8 + 4 +1] = byte(0); //motor height
      bytes[i*8 + 5 +1] = byte(0); //color 0
      bytes[i*8 + 6 +1] = byte(0); //color 1
      bytes[i*8 + 7 +1] = byte(0); //color 2
    }
    return bytes;
  }

  byte [] startSequence(int numBlocks, int numPixels) {
    int  numPix = int(numBlocks * numPixels);

    byte [] bytes = new byte[ numPix *8 +1];
    println("sending bytes "+ numPix*8 +1);
    bytes[0] = SEND;
    for (int i = 0; i < numPix; i++) {
      bytes[i*8 + 0 +1]= 1; //sequence activator
      bytes[i*8 + 1 +1] = byte(0); //motor start
      bytes[i*8 + 2 +1] = byte(1); //enable 1
      bytes[i*8 + 3 +1] = byte(0); //stop motor
      bytes[i*8 + 4 +1] = byte(0); //motor height
      bytes[i*8 + 5 +1] = byte(0); //color 0
      bytes[i*8 + 6 +1] = byte(0); //color 1
      bytes[i*8 + 7 +1] = byte(0); //color 2
    }
    return bytes;
  }



  byte [] moveMotors(int numBlocks, int numPixels) {
    int  numPix = int(numBlocks * numPixels);

    byte [] bytes = new byte[ numPix *8 +1];
    println("sending bytes "+ numPix*8 +1);
    bytes[0] = SEND;
    for (int i = 0; i < numPix; i++) {
      bytes[i*8 + 0 +1]= 0;
      bytes[i*8 + 1 +1] = byte(255);
      bytes[i*8 + 2 +1] = byte(0); //

      bytes[i*8 + 3 +1] = byte(0); //enable motor
      bytes[i*8 + 4 +1] = byte(0); //motor height
      bytes[i*8 + 5 +1] = byte(0); //color 0
      bytes[i*8 + 6 +1] = byte(0); //color 1
      bytes[i*8 + 7 +1] = byte(0); //color 2
    }
    return bytes;
  }


  public void sendByte(byte [] data) {
    this.port.write(data);
    this.port.clear();
  }
}

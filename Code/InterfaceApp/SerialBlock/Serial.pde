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
    byte [] bytes = new byte[ col.length*4 +1];
    bytes[0] = SEND;
    for (int i = 0; i < col.length; i++) {
      short color16 =  toRGB565(col[i]);
      byte [] bcol16 = covert16_8array(color16);
      bytes[i*4 + 0 +1]= 0;
      bytes[i*4 + 1 +1] = bcol16[0];
      bytes[i*4 + 2 +1] = bcol16[1];
      bytes[i*4 + 3 +1] = byte(255);
    }
    return bytes;
  }


  public void sendByte(byte [] data) {
    this.port.write(data);
    this.port.clear();
  }
  
}

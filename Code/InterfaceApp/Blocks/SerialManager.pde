/*
Class that Manages the serial information
 */
class SerialManager {
  ArrayList< Serial> serverPorts;

  //transfer speed
  int bauRate    = 1000000;

  //number of Servers connected to the host computer
  int numServers = 4;

  //number of available ports
  int availablePorts = 0;

  //initialize the number of ports 
  SerialManager() {
    serverPorts = new ArrayList<Serial>();
    printArray(Serial.list());
    
    //find the number of ports that are available.
    //parallize the matrix output based on the available serial ports
    
  }

  //setup the ports based on how many ports are available
  void setupPorts(PApplet pa) {
    for (int i = 0; i < availablePorts; i++) {
      String portName = Serial.list()[0];
       Serial port = new Serial(pa, portName, bauRate);
      serverPorts.add(port);
    }
  }


  void sendData() {
  
  }
  
}

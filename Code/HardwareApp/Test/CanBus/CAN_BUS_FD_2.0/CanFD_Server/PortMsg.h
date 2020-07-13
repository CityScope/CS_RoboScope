#pragma once
/*
* 
* Class that manages incomming data from the interpeter app
* 
* 
*/
class PortMsg{
  
  public:

  PortMsg();

  //convert input msgs to a temp array that can be sent to as a CAN BUS Msg
  void readSerial();

  uint8_t * getCANBusMsg();
  
  private:


 };

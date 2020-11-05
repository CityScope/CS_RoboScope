# Pixel Matrix Test Code 

This code tests the serial and CAN bus communication pipeline from the CS CityScope Interface App to a Pixel Matrix (substitute for the RoboScope table)

## Translator.ino

-  The first iteration of Translator code and only has serial communication code
-  Information is sent pixel by pixel 
-  Features: button input to send defined data to interface app as a test

## Server 

-  Combination of serial communication code and CAN bus messaging 
-  Will send data received from Digital Controls to connected clients through CAN bus msgs
    + One CAN bus msg per node
        * ID: node ID
        * Buffer: [height, interaction, color1, color2] x8
- Will send data received from clients to Digital Controls through serial communication
    + Char array in the form:
        * "nodeID,height,interaction,color1,color2,height..."
    
## Client

-  Will read and process data from CANFD_Server
-  Displays information on pixel matrix using SmartMatrix3 library
-  Sends back CANbus msg (on serial input) to test Digital Controls receiving capabilities

For more information about the CANFD Server and Client, see the  check out the [CANBUS Test folder](https://github.com/CityScope/CS_RoboScope/tree/master/Code/HardwareApp/Test/CanBus/CAN_BUS_FD_2.0). 

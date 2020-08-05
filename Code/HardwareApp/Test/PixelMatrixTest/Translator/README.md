# Translator

This translator is the physical Teensy that connects the table to the interface app through serial communication and CAN bus messages.

## Translator.ino

-  The first iteration of Translator code and only has serial communication code
-  Information is sent pixel by pixel 
-  Features: button input to send defined data to interface app as a test

## CANFD_Server (The Actual Translator)

-  Combination of serial communication code and CAN bus messaging 
-  Will send data read from interface App to connected clients
-  CAN bus msg in the form 
    * ID: node ID x pixel ID
    * Buffer: [height, interaction, color1, color2]
 
## CANFD_Client

-  Will read data from CANFD_Server
-  Used for node control boards
-  Does not have sending capabilities yet 

For more information about the CANFD Server and Client, check out the [documentation](https://github.com/CityScope/CS_RoboScope/tree/master/Code/HardwareApp/Test/CanBus/CAN_BUS_FD_2.0) in the CS_Roboscope Repo

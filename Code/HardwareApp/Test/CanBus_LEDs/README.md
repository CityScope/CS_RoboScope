# CanBus + Interaction Board LEDs Test Code 

This code tests the serial and CAN bus communication pipeline from the CS CityScope Interface App to the Interaction Board of the table

## Server

-  Combination of serial communication code and CAN bus messaging 
-  Will send data received from Digital Controls to connected clients through CAN bus msgs
    + One CAN bus msg per node
        * ID: node ID
        * Buffer: [height, interaction, color1, color2] x8
- Will send data received from clients to Digital Controls through serial communication
    + Char array in the form:
        * "nodeID,height,interaction,color1,color2,height..."
    
## CANFD_Client

-  Will read and process data from CANFD_Server
-  Displays information through colors on NeoPixels

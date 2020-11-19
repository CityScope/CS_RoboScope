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
- Serial Input Tests:
    + a: turn neopixels white
    + b: turn neopixels blue
    + c,{int 0-255},{int 0-255}: turn neopixels into given rgb565 color
    + Example Interface inputs to use:
        * W0,0,20,224,195,1,8,27,214,2,20,224,195,3,8,27,214,4,20,224,195,5,20,224,195,6,8,105,243,7,8,105,243E
        * W0,0,8,252,211,1,20,224,195,2,8,27,214,3,8,27,214,4,8,27,214,5,8,27,214,6,8,105,243,7,8,105,243E
    
## CANFD_Client
-  Will read and process data from CANFD_Server
-  Displays information through colors on NeoPixels

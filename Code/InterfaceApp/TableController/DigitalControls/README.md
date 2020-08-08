# Web Socket Server

This server is the layer of communication between the Interface App and the physical Roboscope table. The server uses web sockets to send and receive data to and from the Interface App and uses serial communication with the Translator Teensy. Current functionality allows grid updates from the JS Interface while 'brushing' edit events occur to be sent to Translator and continuous reading of data from Translator. 

## websocketServer.py 

This main server file sets up communication with the Translator Teensy and runs the sockets that connect to the Interface App. For more information about sockets and socket.io, check out the documentation for the [python-socketio library](https://python-socketio.readthedocs.io/en/latest/)

## serial_lib.py 

This file defines a Translator class, that can initialize and close serial communication to a port, and can read and write messages from and to the port. 

## grid_handling.py

This file is for all computation and data processing methods needed to be able to send the proper data to either the InterfaceApp or the physical table. 

## Install & run

-   run `$ npm install`

-   install Flask 

-   Change port in `serial_lib.py` to correct Teensy/Arduino port for your computer
    
    * Use CANFD_Server for this  
 
-   Run the server with `python websocketServer.py`
  

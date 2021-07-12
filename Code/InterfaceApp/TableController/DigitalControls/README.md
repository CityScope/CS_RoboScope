# Web Socket Server

This server is the layer of communication between the Interface App and the physical Roboscope table. The server uses web sockets to send and receive data to and from the Interface App and uses serial communication with the Translator Teensy. Current functionality allows grid updates from the JS Interface while 'brushing' edit events occur to be sent to Translator and continuous reading of data from Translator.

## websocketServer.py

This main server file sets up communication with and runs the sockets that connect to the Interface App. For more information about sockets and socket.io, check out the documentation for the [python-socketio library](https://python-socketio.readthedocs.io/en/latest/)

## grid_handling.py

This file is for all computation and data processing methods needed to be able to send the proper data to either the InterfaceApp or the physical table.

## Install & run

-   run `$ npm install`

-   pyhon 3.3, install Flask, socketio

    `$ pip install flask`

    `$ pip install socketio, python-socketio`

    `$ pip install eventlet, npm, colour`

-   Run the server with `python websocketServer.py`

Connect to roboscope:

http://localhost:3000/cityscope=roboscope

### Troubleshooting

- Kill Process:

Find the process using port 8080:

`sudo lsof -i:8080`

Kill the process on that port:

`kill $PID`

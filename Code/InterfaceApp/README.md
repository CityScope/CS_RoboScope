
## CS CityScope JS

Application in js that allows users to examine different urban-design alternatives and observe their impact through different KPIs and matrices.

### Instructions

##### Running CityScope JS
To run the web application, you can go to the [CityScope JS](https://cityscope.media.mit.edu/CS_cityscopeJS/) website or download/clone the repo here and run the app locally.

To run the app locally, run `npm install` inside the folder to install all the packages and then run `npm start`. For more documentation, go to [CityScope ](https://cityscope.media.mit.edu) website.

##### Roboscope path

`http://localhost:3000/cityscope=roboscope`

##### Setting Up Table Controller
1. Download/clone repo and `cd` into DigitalControls in Terminal
2. Install packages
    1. python-socketio version 4.5.1
    2. flask-socketio 4.3.0
      `pip install python-socketio==4.5.1`
      `pip install flask-socketio==4.3.0`
    2. eventlet version 0.25.2
    3. Flask
3. Connect Teensy table server
4. In `grid_handling.py` change `com_port` to port connected to table server
5. Run `python3 websocketServer.py`

## Virtual Serial Port

Create a virtual port to test communication between the WebSocket and the serial port.

### Mac

`pip3 install PyVirtualSerialPorts`

## Create a Virtual Port

`python3 -m virtualserialports 1`


## Blocks App

Application in Processing that simulates the physical table


## MovingLegosServer App


Application in openFrameworks that simulates the physical table

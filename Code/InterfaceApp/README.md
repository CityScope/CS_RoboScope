
## CS CityScope JS

Application in js that allows users to examine different urban-design alternatives and observe their impact through different KPIs and matrices.

### Instructions

##### Running CityScope JS
To run the web application, you can go to the [CityScope JS](https://cityscope.media.mit.edu/CS_cityscopeJS/) website or download/clone the repo here and run the app locally.

To run the app locally, run `npm install` inside the folder to install all the packages and then run `npm start`. For more documentation, go to [CityScope ](https://cityscope.media.mit.edu) website.

##### Setting Up Table Controller
1) Download/clone repo and `cd` into DigitalControls in Terminal
1) Install packages
    * python-socketio version 4.5.1
    * eventlet version 0.25.2
    * Flask
2) Connect Teensy table server
3) In `grid_handling.py` change `com_port` to port connected to table server
4) Run `python3 websocketServer.py` 


## Blocks App

Application in Processing that simulates the physical table

![Block App](../../Docs/imgs/blocks_processing.png)


## MovingLegosServer App


Application in openFrameworks that simulates the physical table

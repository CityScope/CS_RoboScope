## A working implementation of the CAN Bus Protocol 

This implementation of the CAN bus protocol has two parts: server and client. Each utilizes a Message class that was created with the data protocol designed for the Roboscope table. 

### Message Class

The message class provides a seamless way to customize and format CAN bus messages before they are sent to the client or to the server. The following outlines the functions of the class and their uses:
 
- `CANMotorMessage(panel_id, msg_size = 8)`: initializes a CAN bus message with a give panel (node) ID, the msg_size determines whether to send a CAN_FD msg or CAN 2.0 msg
- `addMessage(des, color[], steps, interaction)`: adds 4 bytes of data (color, motor steps, and interaction byte) to its appropriate spot in the message buffer array as determined by the specific motor ID the data is referring to
- `getCANmessage()`: returns a CAN bus message that can then be written to a client

### CANFD_Server

The server writes CAN bus message to the client. The current program sets up CAN bus FD and 2.0, initializes input (button) and output (LED) features, and sends a hardcoded CAN bus message to the client on a button press. 

### CANFD_Client

The client reads CAN bus message from the server. The current program sets up CAN bus FD and 2.0, initializes input (button) and output (LED) features, and prints out all CAN bus messages when received from the server
## A working implementation of the CAN Bus protocal

Was made for the custom pcb that takes in a teensy board and has 4 buttons to test communications with.

Pressing buttons on the Sending board (TX) should cause the LEDs to turn on and off. Specifically with the Teensy oriented towards the top, the right buttons should turn the top blue LED off, and the left buttons should turn the bottom blue LED off. (The circuit was not designed for this two button controlling one LED behavior, so only one of the buttons in each respective side may work)

The Receiving board (RX) should print out the button states for each of the buttons on the Sending board in the Serial Monitor, as well as having the LEDs mirror the behavior of the (TX) board.

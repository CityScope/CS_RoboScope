/*
 * 
 * Teensy 4.0
 * 
 * Max memory
 * 
 * 1024K RAM (memory when running app)
 * 2048K Flash
 * 
 * 
 * Incomming arrays
 * 
 * Color 
 * Heights
 * Enable/Disable
 * Interactions
 * 
 * Matrix grid 12 x 8 
 *              w * h
 * 
 * Packed as 4 bytes same as CAN Bus Msg
 * 4 bytes x (12 * 8) =  384 bytes
 * 
 * Packed individually 
 * 
 * 
 * 12 byte per motor 
 * 
 * 12 * ( 12 * 8 ) = 96 * 12  = 1152 bytes 1.1kbytes 
 * 
 * 
 * 
 * 
 */

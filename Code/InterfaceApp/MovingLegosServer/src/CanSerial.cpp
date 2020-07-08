//
//  CanSerial.cpp
//  MovingLegosServer
//
//  Created by Thomas on 2/20/20.
//

#include <stdio.h>
#include "CanSerial.h"



/*
 Simple msg:
 
 2 bytes board id
 2 bytes color
 1 byte step
 1 byte interaction
 
 2 bytes -> 0 to 65,535
 
 */

void CanSerial::sendSimpleMsg(uint16_t boardId, ofColor col, uint8_t step, uint8_t inter){
    uint8_t buffer[6];
    
    uint16_t col16 = convertColor24to16(col);
    
    buffer[0] = boardId >> 8 & 0xFF; //upper byte first
    buffer[1] = boardId & 0xFF;
    buffer[2] = col16 >> 8 & 0xFF; //upper byte first
    buffer[3] = col16 & 0xFF; //Send the lower byte
    buffer[4] = step;
    buffer[5] = inter;
    
    
    mSerial.writeBytes(buffer, 6);
    ofLog(OF_LOG_NOTICE)<<"send MSG";
    ofLog(OF_LOG_NOTICE)<<"board: "<< boardId<<" color: "<<col16<<" step: "<<unsigned(step)<<" iter: "<<unsigned(inter);
}

/*
 convert color 24 bits to 16 bits color
 RRRRR-GGGGGG-BBBBB (5-6-5)
*/

uint16_t CanSerial::convertColor24to16(ofColor col){
    uint32_t rgb;
    uint8_t arrayBytes[ ] = {col.r, col.g, col.b, 0};
    std::memcpy(&rgb, arrayBytes, 4);
    uint16_t newRGB = (((rgb&0xf80000)>>8)|((rgb&0xfc00)>>5)|((rgb&0xf8)>>3));
    return newRGB;
}

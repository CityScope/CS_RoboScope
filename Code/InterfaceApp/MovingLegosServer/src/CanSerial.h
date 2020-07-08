/*
 class that manages serial communication using the Can Bus protocol.
 */

#pragma once

#include "ofMain.h"

//baurate of the serial
#define baurate 57600

//memory
class CanSerial;
typedef std::shared_ptr<CanSerial> CanSerialRef;

class CanSerial{
    
public:
    
    //setup serial
    CanSerial(){
        mSerialId =0;
        try{
            mSerial.setup(mSerialId, baurate);
        }catch(exception& e){
            cout << e.what()<<std::endl;
        }
    }

    //delete serial
    ~CanSerial(){
        
    }
    
    //create memory
    static CanSerialRef create() {
        return std::make_shared<CanSerial>();
    }
    
    //send msg
    void printSerialDevice(){
        mSerial.listDevices();
    }
    
    //set Id
    void setId(int serial){
        mSerialId = serial;
    }
    
    //send can Msg Protocol
    void sendCanMsg();
    
    //send simple msg
    void sendMsg();
    
    void sendSimpleMsg(uint16_t boardId, ofColor col, uint8_t step, uint8_t inter);
    
    
    //convert color 24 bits to 16 bits color;
    uint16_t convertColor24to16(ofColor col);
    
private:
    int         mSerialId;
    ofSerial	mSerial;
    bool        mSerialMessage;
    
    std::string mMsg;
    
};

#pragma once

/*
 
 */
#include "ofMain.h"


#include <iostream>
#include <memory>
#include <thread>
#include <chrono>
#include <mutex>

//block size
#define BLOCK_SIZE 20

//memory
class UrbanBlock;
typedef std::shared_ptr<UrbanBlock> UrbanBlockRef;

///////////////////
class UrbanBlock{
public:
    
    UrbanBlock();
    
    ~UrbanBlock(){
        
    }
    
    //create memory
    static UrbanBlockRef create() {
        return std::make_shared<UrbanBlock>();
    }
    
    void createBlockMesh();
    
    //draw
    void draw();
    void update();
    
    //set Position
    void setPos(glm::vec3 pos){mPos = pos;}
    void setSize(glm::vec3 size){mSize = size;}
    void setColor(ofColor col){mColor = col;}
    
    void animatePosition();
    
    glm::vec3 getVertex(int i);
    
private:
    //position
    glm::vec3  mPos;
    glm::vec2  mCenter;
    
    float mHeight;
    float mDepth;
    
    //rod
    glm::vec3 mSize;
    
    //unique id
    int mId;
    
    float mWidth;
    
    //color
    ofColor mColor;
    
    //block mesh
    ofVbo mVboBlock;
    ofMesh mMeshBlock;
    int mNumVertices;
    
    //animation
};

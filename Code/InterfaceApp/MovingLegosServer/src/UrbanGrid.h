#pragma once

/*
 
 */

#include "ofMain.h"
#include "UrbanBlock.h"

//Grid texture
class UrbanGrid;
typedef std::shared_ptr<UrbanGrid> UrbanGridRef;

class UrbanGrid{
public:
    
    //create a grid with w and h dimentions
    UrbanGrid(int w, int h);
    
    ~UrbanGrid(){
        mGrid.clear();
    }
    
    //create memory
    static UrbanGridRef create(int w, int h) {
        return std::make_shared<UrbanGrid>(w, h);
    }
    
    void draw();
    void update();
    
    //base
    void createBase();
    void drawBottomBase();
    void drawTopBase();
    
    glm::vec2 getCenter(){return mCenter;}
    glm::vec3 getDims(){return mDims;}
    

    
private:
    std::vector<UrbanBlockRef> mGrid;
    glm::vec3 mDims;
    
    glm::vec2 mCenter;
    
    //use urbanBlock class for the base
    UrbanBlockRef mTopBase;
    UrbanBlockRef mBottomBase;
};

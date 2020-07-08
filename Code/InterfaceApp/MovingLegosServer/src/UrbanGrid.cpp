#include "UrbanGrid.h"

//---------------------------------------------
UrbanGrid::UrbanGrid(int w, int h){
    mDims = glm::vec3(w, h, 350);
    
    glm::vec2 UrbanSquare(40, 40);
    glm::vec3 rodDim(32.0/2.0, 32.0/2.0, 350/2.0);
    std::cout<<"dims: "<<w<<" "<<h<<std::endl;
    for(int i = 0; i < w ; i++){
        for(int j  = 0; j < h; j++){
            UrbanBlockRef block = UrbanBlock::create();
            float x  =  i * UrbanSquare.x;
            float y  =  j * UrbanSquare.y;
            block->setPos(glm::vec3(x, y, 0));
            
            //block size 32 x 32 with lenght 350
            block->setSize(glm::vec3(rodDim.x, rodDim.y, rodDim.z));
            mGrid.push_back(block);
        }
    }
    
    mDims = glm::vec3(w * UrbanSquare.x, h * UrbanSquare.y, rodDim.z);
    mCenter = glm::vec2(mDims.x/2.0, mDims.y/2.0);
    
    createBase();
    
    cout<<"Grid size: "<<mGrid.size()<<std::endl;
    cout<<"Grid dims"<<mDims.x<<" "<<mDims.y<<" "<<mDims.z<<std::endl;
}

//---------------------------------------------
void UrbanGrid::draw(){
    for(auto & block : mGrid){
        block->draw();
    }
    
    //draw table structure
    
    drawBottomBase();
    drawTopBase();
}


//---------------------------------------------
void UrbanGrid::update(){


}

//----------------------------------------
void UrbanGrid::createBase(){
    mTopBase  = UrbanBlock::create();
    mTopBase->setSize(glm::vec3(mDims.x/1.5 - 4, mDims.y/1.5 - 4, 5));
    mTopBase->setPos(glm::vec3(mDims.x/1.5 - 16 -4 - mDims.x/6.0, mDims.y/1.5 - 16 -4 - mDims.y/6.0, mDims.z - 5*2));
    
    mBottomBase  = UrbanBlock::create();
    mBottomBase->setSize(glm::vec3(mDims.x/1.5 - 4, mDims.y/1.5 - 4, 5));
    mBottomBase->setPos(glm::vec3(mDims.x/1.5 - 16 -4 - mDims.x/6.0, mDims.y/1.5 - 16 -4 - mDims.y/6.0, -mDims.z + 5*2));
    

}

//----------------------------------------
void UrbanGrid::drawBottomBase(){
    mBottomBase->draw();
}
//----------------------------------------
void UrbanGrid::drawTopBase(){
    mTopBase->draw();
}

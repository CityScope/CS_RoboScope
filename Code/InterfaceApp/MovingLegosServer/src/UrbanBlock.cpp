#include "UrbanBlock.h"

//----------------------------------------
UrbanBlock::UrbanBlock(){
    createBlockMesh();
    //default values
    mSize = glm::vec3(5, 5, 50);
    mPos  = glm::vec3(0, 0, 0);
}

//----------------------------------------
void UrbanBlock::createBlockMesh(){
    char c[] = "011111101001111110100101110010000100"
    "010011001000010110111011000100101001";
    
    for (int i=0,x,y,z; i<72; i+=3) {
        
        x = c[i+0]-'0';
        y = c[i+1]-'0';
        z = c[i+2]-'0';
        
        mMeshBlock.addColor(ofFloatColor(x,y,z));
        mMeshBlock.addVertex(ofVec3f(x*2-1,y*2-1,z*2-1));
    }
    
    mNumVertices = 24;
    
   mMeshBlock.setMode(OF_PRIMITIVE_LINE_LOOP);
    
    //mVboBlock.setMesh(mesh, GL_STATIC_DRAW);
}

glm::vec3 UrbanBlock::getVertex(int i){
   // if(i >=0 && i < mNumVertices)
    //    return mVboBlock.getVert
    return glm::vec3(-1);
}

//----------------------------------------
void UrbanBlock::draw(){
    
    ofPushMatrix();
    ofTranslate(mPos);
    ofScale(mSize);
    mMeshBlock.draw();
    //mVboBlock.draw(GL_QUADS, 0, mNumVertices);
    ofPopMatrix();
}

//----------------------------------------
void UrbanBlock::update(){
    
}

void UrbanBlock::animatePosition(){
    
}


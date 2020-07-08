#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofBackground(0, 0, 0);
    ofSetFrameRate(60);
    ofSetVerticalSync(false);
    ofEnableSmoothing();
    ofSetCircleResolution(64);

    // this uses depth information for occlusion
    // rather than always drawing things on top of each other
    ofEnableDepthTest();


    //serial test
    mSerialTest = CanSerial::create();

    //dynamic grid
    mUrbanGrid = UrbanGrid::create(WIDTH_PINS, HEIGHT_PINS);

    //GUI
    setupGUI();
    
    setupCams();
}

//--------------------------------------------------------------
void ofApp::update(){

    //update
    updateGUI();
}
//--------------------------------------------------------------
void ofApp::setupCams(){
    
    iMainCamera = 0;
    bCamParent = false;
    
    mEasyCam.setDistance(100);
    mEasyCam.setNearClip(10);
    mEasyCam.setFarClip(10000);

    camTop.tiltDeg(-90);
    camLeft.panDeg(-90);

    cameras[0] = &mEasyCam;
    cameras[1] = &camFront;
    cameras[2] = &camTop;
    cameras[3] = &camLeft;

    for (size_t i = 1; i != N_CAMERAS; ++i) {
        cameras[i]->enableOrtho();
        cameras[i]->setNearClip(0.1);
        cameras[i]->setFarClip(100000);
    }

    //--
    // Define viewports
    setupViewports();
}

//--------------------------------------------------------------
void ofApp::setupViewports(){
    //call here whenever we resize the window


    //--
    // Define viewports

    float xOffset = ofGetWidth() / 3;
    float yOffset = ofGetHeight() / N_CAMERAS;

    viewMain.x = xOffset;
    viewMain.y = 0;
    viewMain.width = xOffset * 2;
    viewMain.height = ofGetHeight();

    for(int i = 0; i < N_CAMERAS; i++){

        viewGrid[i].x = 0;
        viewGrid[i].y = yOffset * i;
        viewGrid[i].width = xOffset;
        viewGrid[i].height = yOffset;
    }
}

void ofApp::drawUrbanGrid(){
    
    ofPushStyle();
    ofSetColor(ofColor::white);
    ofEnableDepthTest();
    
    ofPushMatrix();
    ofTranslate(-mUrbanGrid->getCenter().x, -mUrbanGrid->getCenter().y, 0 );
    mUrbanGrid->draw();
    ofPopMatrix();
    ofPopStyle();
}

void ofApp::drawScene(int cameraIndex){
    
    drawUrbanGrid();
    //--
    // Draw frustum preview for ofEasyCam camera

    // This code draws our camera in
    //    the scene (reddy/pink lines)
    //
    // The pyramid-like shape defined
    //    by the cameras view is called
    //    a 'frustum'.
    //
    // Often we refer to the volume
    //    which can be seen by the
    //    camera as 'the view frustum'.


    // First check if we're already drawing the view through the easycam
    // If so, don't draw the frustum preview.
    if(cameraIndex != 0){

        ofPushStyle();
        ofPushMatrix();

        //--
        // Create transform for box->frustum

        // In 'camera space' the bounds of
        //  the view are defined by a box
        //  with bounds -1->1 in each axis
        //
        // To convert from camera to world
        //  space, we multiply by the inverse
        //  camera matrix of the camera, i.e
        //  inverse of the ViewProjection
        //  matrix.
        //
        // By applying this transformation
        //  our box in camera space is
        //  transformed into a frustum in
        //  world space.
        //

        // The camera's matricies are dependant on
        //  the aspect ratio of the viewport.
        //  (Which is why we use the viewport as
        //  an argument when we begin the camera.
        //
        // If this camera is fullscreen we'll use
        //   viewMain, else we'll use viewGrid[0]
        ofRectangle boundsToUse;
        if(iMainCamera == 0){
            boundsToUse = viewMain;
        }
        else{
            boundsToUse = viewGrid[0];
        }

        // Now lets get the inverse ViewProjection
        //  for the camera
        glm::mat4 inverseCameraMatrix;
        inverseCameraMatrix = glm::inverse(mEasyCam.getModelViewProjectionMatrix(boundsToUse));

        // By default, we can say
        //    'we are drawing in world space'
        //
        // The camera matrix performs
        //    world->camera
        //
        // The inverse camera matrix performs
        //    camera->world
        //
        // Our box is in camera space, if we
        //    want to draw that into world space
        //    we have to apply the camera->world
        //    transformation.
        //

        // This syntax is a little messy.
        // What it's saying is, send the data
        //  from the inverseCameraMatrix object
        //  to OpenGL, and apply that matrix to
        //  the current OpenGL transform
        ofMultMatrix( inverseCameraMatrix );

        //
        //--



        //--
        // Draw box in camera space
        // (i.e. frustum in world space)

        ofNoFill();
        // i.e. a box -1, -1, -1 to +1, +1, +1
        ofDrawBox(0, 0, 0, 2.0f);
        //
        //--

        ofPopMatrix();

        // Alternatively to the above, you may call the built-in method of
        // ofCamera to draw its frustum:
        //
        // camEasyCam.drawFrustum(boundsToUse);

        ofPopStyle();
    }

    //
    //--



    //--
    // Draw mouse ray

    // Draw the ray if ofEasyCam is in main view,
    //  and we're not currently drawing in that view
    if(iMainCamera == 0 && cameraIndex != 0){
        ofPushStyle();
        ofSetColor(100, 100, 255);
        ofDrawLine(mMouseRay[0], mMouseRay[1]);
        ofPopStyle();
    }

    //
    //--
}

//--------------------------------------------------------------
void ofApp::draw(){

    ofPushStyle();
    ofDisableDepthTest();
    ofSetColor(100, 100, 100);
    ofDrawRectangle(viewGrid[iMainCamera]);
    
    
    ofSetColor(ofColor::white);
    ofEnableDepthTest();
    
    // draw main viewport
    cameras[iMainCamera]->begin(viewMain);
    drawScene(iMainCamera);

    // calculate mouse ray whilst this camera is active
    updateMouseRay();

    cameras[iMainCamera]->end();

    // draw side viewports
    for(int i = 0; i < N_CAMERAS; i++){
        cameras[i]->begin(viewGrid[i]);
        drawScene(i);
        cameras[i]->end();
    }
    ofPopStyle();
    
    
    ofPushStyle();
    ofDisableDepthTest();

    // draw some labels
    ofSetColor(255, 255, 255);
    ofDrawBitmapString("Press keys 1-4 to select a camera for main view", viewMain.x + 20, 30);
    ofDrawBitmapString("Camera selected: " + ofToString(iMainCamera + 1), viewMain.x + 20, 50);
    ofDrawBitmapString("Press 'f' to toggle fullscreen", viewMain.x + 20, 70);
    ofDrawBitmapString("Press 'p' to toggle parents on OrthoCamera's", viewMain.x + 20, 90);

    ofDrawBitmapString("EasyCam",   viewGrid[0].x + 20, viewGrid[0].y + 30);
    ofDrawBitmapString("Front",     viewGrid[1].x + 20, viewGrid[1].y + 30);
    ofDrawBitmapString("Top",       viewGrid[2].x + 20, viewGrid[2].y + 30);
    ofDrawBitmapString("Left",      viewGrid[3].x + 20, viewGrid[3].y + 30);

    // draw outlines on views
    ofSetLineWidth(5);
    ofNoFill();
    ofSetColor(255, 255, 255);
    //
    for(int i = 0; i < N_CAMERAS; i++){
        ofDrawRectangle(viewGrid[i]);
    }
    //
    ofDrawRectangle(viewMain);

    // restore the GL depth function
    
    ofPopStyle();

    
    

    //draw
    ofPushStyle();
    ofDisableDepthTest();
    drawGUI();
    ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::setupGUI(){
    picker = new ofxDatGuiColorPicker("COLOR PICKER", ofColor::fromHex(0xCECECE));
    picker->setAnchor(ofxDatGuiAnchor::TOP_LEFT);

    // and register a callback to listen for change events //
    picker->onColorPickerEvent(this, &ofApp::onColorPickerEvent);
}

//--------------------------------------------------------------
void ofApp::updateGUI(){
    picker->update();
}

//--------------------------------------------------------------
void ofApp::drawGUI(){
    picker->draw();
}

//--------------------------------------------------------------
void ofApp::updateMouseRay(){
    // Define ray in screen space
    mMouseRay[0] = glm::vec3(ofGetMouseX(), ofGetMouseY(), -1);
    mMouseRay[1] = glm::vec3(ofGetMouseX(), ofGetMouseY(), 1);

    // Transform ray into world space
    mMouseRay[0] = cameras[iMainCamera]->screenToWorld(mMouseRay[0], viewMain);
    mMouseRay[1] = cameras[iMainCamera]->screenToWorld(mMouseRay[1], viewMain);
}


//--------------------------------------------------------------
//Events    ------
void ofApp::onColorPickerEvent(ofxDatGuiColorPickerEvent e){

}

/*bit 3 is actually the fourth digit from the right
//01000
http://www.cplusplus.com/forum/general/97378/
https://stackoverflow.com/questions/8461126/how-to-create-a-byte-out-of-8-bool-values-and-vice-versa
 */
//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if(key >= '1' && key <= '4'){
        iMainCamera = key - '1';
    }

    if(key == 'f'){
        ofToggleFullscreen();
    }

    if(key == 'p'){
        if(bCamParent){
            camFront.clearParent();
            camTop.clearParent();
            camLeft.clearParent();

            bCamParent = false;
        }
        else{
 
            bCamParent = true;
        }
    }
    
    if(key == 'm'){
        uint16_t boardId = 5;
        ofColor col(255, 255, 255);

        uint8_t step = 100;

        bool stateIteraction[8] = {false, false, false, false, false, false, false, false};
        uint8_t iter = 0;

        for(int i = 0; i < 8; i++){
            iter |= ((unsigned char) stateIteraction[i]) << i;
        }
        cout<<iter<<", "<< unsigned(iter)<<", "<<int(iter)<<std::endl;

        mSerialTest->sendSimpleMsg(boardId, col, step, iter);
    }


}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    setupViewports();
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}

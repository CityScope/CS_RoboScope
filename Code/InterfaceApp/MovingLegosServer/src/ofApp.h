#pragma once

/*
 
 */
#include "ofMain.h"

#include "CanSerial.h"
#include "UrbanGrid.h"
#include "UrbanBlock.h"

#include "ofxDatGui.h"

//define dimenstions for the grid
#define WIDTH_PINS  12
#define HEIGHT_PINS  8

#define N_CAMERAS 4

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    //collection of usb ports
    std::vector<CanSerialRef> mSerials;
    
    //test port
    CanSerialRef mSerialTest;
    
    //camera movement
    ofEasyCam mEasyCam;
    ofCamera camFront;
    ofCamera camTop;
    ofCamera camLeft;
    
    //cameras have parent?
    bool bCamParent;
    
    //camera pointers
    ofCamera * cameras[N_CAMERAS];
    int iMainCamera;

    //viewports
    ofRectangle viewMain;
    ofRectangle viewGrid[N_CAMERAS];
    
    //setup scene
    void setupCams();
    void setupViewports();
    void drawScene(int cameraIndex);
    
    //ray drawn under mouse cursor [start,end]
    glm::vec3 mMouseRay[2];
    void updateMouseRay();
    
    //grid
    UrbanGridRef mUrbanGrid;
    void drawUrbanGrid();
    
    //GUI
    ofxDatGuiColorPicker    * picker;
    ofxDatGuiButton         * button;
    ofxDatGuiButton         * toggle;
    
    void    setupGUI();
    void    updateGUI();
    void    drawGUI();
    
    //events
    void onColorPickerEvent(ofxDatGuiColorPickerEvent e);
    
    
};

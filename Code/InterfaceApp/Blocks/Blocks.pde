/*
  Program the manages the interface, communication and interaction between the physical table and data. 
 
 
 Thomas Sanchez Lengeling 
 City Science Group
 2019
 
 */
import controlP5.*;
import peasy.*;
import peasy.PeasyCam;
import picking.*;

//picker
Picker picker3d;

//cam
PeasyCam cam;
CameraState state;

PGraphics pg;

//orientation
boolean drawOrientation = false;
PImage imgOrientation;

//background color
int bkgColor = 50;

//grid class
Grid  tableGrid;

//grid size
int gridX = 32;
int gridY = 48;

//block size  32mm legos
int blockSize = 32*2;

//sapce between the blocks
int gridSpace = 4*2;

//location of the grid 
int gridStartX = 400;
int gridStartY = 00;

//gui 
ControlP5 cp5;
Accordion accordionGrid;
Accordion accordionIds;

// id of the road
int currentIdRoad = 0;

//enables
boolean enableDraw     = false;
boolean enableMap      =  false;
boolean enableColorMap = false;
boolean enableGrouping = false;

//group blocks
int groupCounter = 0;
int groupMax = 4;

//activate camera animation
boolean activeAnimation = false;
boolean recording = false;

//map rendering 
Map map;

//initialization
void setup() {
  size(1920, 1080, P3D);
  smooth(8);

  imgOrientation = loadImage("orientation.png");

  //picking 3d objects
  picker3d = new Picker(this);

  //camera 
  cam = new PeasyCam(this, 500);
  state = cam.getState();

  //gui
  cp5 = new ControlP5(this);
  cp5.setAutoDraw(false);

  // group number 1, contains 2 bangs
  Group g1 = cp5.addGroup("Gird")
    .setBackgroundColor(color(0, 64))
    .setBackgroundHeight(150);

  // group number 1, contains 2 bangs
  Group g2 = cp5.addGroup("ids")
    .setBackgroundColor(color(0, 64))
    .setBackgroundHeight(150);

  // group number 1, contains 2 bangs
  Group g3 = cp5.addGroup("groups")
    .setBackgroundColor(color(0, 64))
    .setBackgroundHeight(150);

  cp5.addSlider2D("gridSize")
    .setPosition(10, 10)
    .setSize(75, 75)
    .setMinMax(1, 1, 80, 45)
    .setValue(float(gridX)+0.1, float(gridY))
    .moveTo(g1)
    ;

  cp5.addSlider2D("gridSpace")
    .setPosition(100, 10)
    .setSize(75, 75)
    .setMinMax(0, 0, 20, 20)
    .setValue(gridSpace, gridSpace)
    .moveTo(g1)
    ;

  cp5.addSlider("blockSize")
    .setPosition(10, 120)
    .setSize(90, 15)
    .setValue(blockSize)
    .setRange(10, 100)
    .setGroup(g1)
    ;

  //ids


  cp5.addTextlabel("idGrid")
    .setText("id: ")
    .setPosition(100, 50)
    .setFont(createFont("Georgia", 12))
    .setGroup(g2);

  cp5.addSlider("density")
    .setPosition(90, 10)
    .setSize(90, 15)
    .setGroup(g2)
    ;

  cp5.addToggle("id")
    .setPosition(150, 50)
    .setSize(20, 20)
    .setGroup(g2)
    ;


  cp5.addRadioButton("amenities")
    .setPosition(10, 10)
    .setSize(20, 20)
    .addItem("Office", 0)
    .addItem("Residential", 1)
    .addItem("Parks", 2)
    .addItem("Roads", 3)
    .addItem("Map", 4)
    .setGroup(g2)
    ;

  accordionGrid = cp5.addAccordion("Master")
    .setPosition(10, 20)
    .setWidth(230)
    .addItem(g1)
    .addItem(g3)
    .addItem(g2);

  cp5.addRadioButton("Groups")
    .setPosition(10, 20)
    .setSize(20, 20)
    .addItem("Group_1", 0)
    .addItem("Group_2", 1)
    .addItem("Gorup_3", 2)
    .setGroup(g3)
    ;

  accordionGrid.open(0, 1, 2);
  accordionGrid.setCollapseMode(Accordion.MULTI);

  //real map
  map = new Map();

  {
    float [] gridSize = cp5.getController("gridSize").getArrayValue();
    float [] gridSpace = cp5.getController("gridSpace").getArrayValue();
    int blockSize = int(cp5.getController("blockSize").getValue());

    tableGrid = new Grid(gridStartX, gridStartY, (int)gridSize[0], (int)gridSize[1], blockSize, (int)gridSize[0]);
  }
}


void draw() {
  picker3d.stop();

  background(bkgColor); //190

  pushMatrix();

  // lights();
  // ambient(250, 250, 250);
  // pointLight(255, 255, 255, 0, 0, 200);

  translate(-650, -350, 0);
  // 

  //draw table
  drawTable();
  drawBase();

  //draw grid and rods
  tableGrid.draw();
  //tableGrid.drawContour();

  Block currBl = null;
  tableGrid.updatePicker(picker3d);
  int idPicker = picker3d.get(mouseX, mouseY);
  println(idPicker);
  if (idPicker >= 0 && idPicker < gridX * gridY) {
    currBl = tableGrid.getBlock(idPicker);
    tableGrid.updateCurrentBlock(currBl);
  }

  picker3d.stop();
  popMatrix();

  if (!enableDraw) {
    float [] gridSize = cp5.getController("gridSize").getArrayValue();
    tableGrid.setGridDim(gridSize);

    float [] gridSpace = cp5.getController("gridSpace").getArrayValue();
    tableGrid.setSetSpace(gridSpace);
  }

  //Block bl = tableGrid.getCurrentBlock(mouseX, mouseY);
  if (currBl != null) {
    cp5.getController("idGrid").setStringValue("id: "+currBl.getId());
  }


  //density
  float scaleX = 50.0;
  float scaleY = 50.0;
  float scaleZ = 10.0;

  float scale = cp5.getController("density").getValue();
  //createCube(scaleX, scaleY + scale, scaleZ );

  int wTable = tableGrid.getEndX();
  int hTable = tableGrid.getEndY();
  //real map

  if (enableMap) {
     //map.draw(gridStartX, gridStartY, wTable, hTable);
  }

  if (!recording) {
    drawGUI();
  }
  //set Colr dimentions

  if (recording) {
    saveFrame("output/frames-######.png");
  }
}

void drawGUI() {

  hint(DISABLE_DEPTH_TEST);

  cam.beginHUD();

  fill(0);
  text(frameRate, 10, 10);

  if (drawOrientation) {
    pushStyle();
    tint(255, 50);
    image(imgOrientation, 0, 0, width, height);
    popStyle();
  }

  cp5.draw();
  cam.endHUD();



  hint(ENABLE_DEPTH_TEST);
}


void mousePressed() {
  Block bl = tableGrid.getCurrentBlock(mouseX, mouseY);

  int Office = int(cp5.getController("Office").getValue());
  int Residential = int(cp5.getController("Residential").getValue());
  int Parks = int(cp5.getController("Parks").getValue());
  int Map = int(cp5.getController("Map").getValue());

  if (bl != null) {
    if (Office == 1) {
      bl.setType(1);
    } else if (Residential == 1) {
      bl.setType(2);
    } else if (Parks == 1) {
      bl.setType(3);
    }

    tableGrid.updateBlockType(bl);
  }

  if (Map == 1) {
    tableGrid.setGridColors(map.getImgDensity(), map.getImgDensity());//map.getImgColor());
  }

  //grouping, create group

  int groupBuildings = int(cp5.getController("Group_1").getValue());
}

void keyPressed() {

  if (key == '1') { 
    state = cam.getState();
    println("pos");
    printArray(cam.getPosition());
    println("rot");
    printArray(cam.getRotations());
  }

  //test can 
  if (key == '2') {
    //sendCanMsgTest();
  }

  if (key == '2') { 
    cam.setState(state, 18000);
  }

  if (key == 'a') {
    enableMap = !enableMap;
  }

  if (key == 'b') {
    enableColorMap = !enableColorMap;
  }

  if (key == 'z') {
    activeAnimation = !activeAnimation;
  }

  if (key == 'r' || key == 'R') {
    recording = !recording;
  }

  if ( key == 'p') {
    saveFrame("line-######.png");
  }

  if (key == 'o') {
    drawOrientation = !drawOrientation;
  }
}

void blockSize(float bsize) {
  tableGrid.setBlockSize(int(bsize));
}

void id() {
  enableDraw = !enableDraw;

  if (!enableDraw) {
    float [] gridSize = cp5.getController("gridSize").getArrayValue();
    tableGrid.setGridDim(gridSize);

    float [] gridSpace = cp5.getController("gridSpace").getArrayValue();
    tableGrid.setSetSpace(gridSpace);
  }
}

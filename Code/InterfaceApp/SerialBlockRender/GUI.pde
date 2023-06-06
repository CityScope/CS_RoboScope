import controlP5.*;

ControlP5 cp5;

float near;
float far;


void createGUI() {

  cp5 = new ControlP5(this);
  cp5.setPosition(0, 0);

  cp5.setAutoDraw(false);

  // add a horizontal sliders, the value of this slider will be linked
  // to variable 'sliderValue'
  cp5.addSlider("far")
    .setPosition(50, 50)
    .setSize(300, 20)
    .setRange(0, 1000)
    .setValue(750)
    ;

  // create another slider with tick marks, now without
  // default value, the initial value will be set according to
  // the value of variable sliderTicks2 then.
  cp5.addSlider("near")
    .setPosition(50, 80)
    .setRange(0, 1000)
    .setSize(300, 20)
    .setValue(0)
    ;

  cp5.addButton("saveDepth")
    .setValue(0)
    .setPosition(50, 120)
    .setSize(50, 50)
    ;
}

void far(float v) {
  far =v;
  clip_z_far = v;
}
void near(float v) {
  near = v;
}

void saveDepth() {
  int type = 1; // 512, type  = 2

  println("save depth");
  calculateDepth();
  PImage depth1024 = pg_aa;//.save("data/depths/depth.png");
  PImage depthCut =  depth1024.get((int)depthROI.x, (int)depthROI.y, 1024, 1024);
  //depthCut.resize(512, 512);
  depthCut.save("data/depths/depth_1024.png");

  //color
  println("save color");
  PImage color1024 = pg_color;//.save("data/depths/depth.png");
  PImage colorCut =  color1024.get((int)depthROI.x, (int)depthROI.y, 1024, 1024);
  // colorCut.resize(512, 512);
  colorCut.save("data/depths/color_1024.png");
}

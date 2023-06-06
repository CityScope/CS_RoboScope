import java.util.ArrayList;
import processing.core.PImage;
import processing.core.PVector;

import peasy.*;
import peasy.PeasyCam;
import peasy.org.apache.commons.math.*;
import peasy.org.apache.commons.math.geometry.*;
import java.io.ByteArrayOutputStream;
import java.nio.ByteBuffer;

PeasyCam cam;

BlockManager blockManager;

SerialHandler serial;
boolean enablePort = false;

PGraphics pg;

int numPanels = 2;
int nodesX   = 2;
int nodesY   = 4;

int moduleX = 3 * numPanels * nodesX;
int moduleY = 2 * nodesY;

int nodesPerNode = 8;

int blockSize = 10;

RendererScreen rc;

PFont font;

int currentNode = 0;

boolean depthFullScreen = false;
boolean debugView = true;

public void settings() {
  size(viewport_w, viewport_h, P3D);
  smooth(0);
}

void setup() {
  //size(1280, 720, P2D);
  pg = createGraphics(width, height, P3D);

  cam = new PeasyCam(this, 1000);
  cam.setMinimumDistance(0);
  cam.setMaximumDistance(1000);
  cam.setDistance(160);

  blockManager = new BlockManager(moduleX, moduleY, nodesPerNode, blockSize );

  font =  createFont("Georgia", 12);
  textFont(font);


  //serial
  try {
    serial = new SerialHandler(this);
    enablePort = false;
  }
  catch(Exception e) {
    println("cannot load Serial");
  }

  rc = new RendererScreen(moduleX, moduleY);

  pg.beginDraw();
  pg.ellipseMode(CENTER);
  // pg.textMode(SHAPE);
  pg.endDraw();

  //hint(ENABLE_DEPTH_SORT);

  //GUI
  createGUI();
  createGUIMatrix();

  //
  perspective(60 * PI/180, width/(float)height, 1, 7000);

  PShape ps = blockManager.getShape();

  // record list of vertices of the given shape
  DwVertexRecorder vertex_recorder = new DwVertexRecorder(this, ps);

  // compute scene bounding-sphere
  DwBoundingSphere scene_bs = new DwBoundingSphere();
  scene_bs.compute(vertex_recorder.verts, vertex_recorder.verts_count);

  // used for centering and re-scaling the scene
  PMatrix3D mat_scene_bounds = scene_bs.getUnitSphereMatrix();

  // callback for rendering the scene
  DwSceneDisplay scene_display = new DwSceneDisplay() {
    @Override
      public void display(PGraphics3D canvas) {
      displayScene(canvas);
    }
  };

  // library context
  context = new DwPixelFlow(this);
  context.print();
  context.printGL();
  context.printGL_MemoryInfo();

  // Depth of Field
  dof = new DepthOfField(context);
  depthZ = new DepthField(context);
  geombuffer = new DwScreenSpaceGeometryBuffer(context, scene_display);


  // init skylight renderer
  skylight = new DwSkyLight(context, scene_display, mat_scene_bounds);

  // parameters for sky-light
  skylight.sky.param.iterations     = 70;
  skylight.sky.param.solar_azimuth  = 0;
  skylight.sky.param.solar_zenith   = 0;
  skylight.sky.param.sample_focus   = 1; // full sphere sampling
  skylight.sky.param.intensity      = 3.0f;
  skylight.sky.param.rgb            = new float[]{1, 1, 1};
  skylight.sky.param.shadowmap_size = 256; // quality vs. performance

  // parameters for sun-light
  skylight.sun.param.iterations     = 50;
  skylight.sun.param.solar_azimuth  = 45;
  skylight.sun.param.solar_zenith   = 55;
  skylight.sun.param.sample_focus   = 0.05f;
  skylight.sun.param.intensity      = 1.2f;
  skylight.sun.param.rgb            = new float[]{1, 1, 155.0/255.0};
  skylight.sun.param.shadowmap_size = 512;

  smaa = new SMAA(context);
}


void draw() {

  background(0);

  resizeScene();

  updateCamActiveStatus();
  // if (CAM_ACTIVE) {
  skylight.reset();
  // }

  // update renderer
  skylight.update();

  // apply AntiAliasing
  smaa.apply(skylight.renderer.pg_render, pg_aa);

  // apply DoF
  renderDOF();

  DwUtils.beginScreen2D(g);
  cam.beginHUD();
  // display result

  pg_color = skylight.renderer.pg_render;


  renderScene();

  image(pg_color, 0, 0);



  if (depthFullScreen) {
    image(pg_aa, 0, 0);
  } else {
    image(pg_color, 0, 0);
  }

  if (debugView) {
    image(pg_aa, 0, 0, 400, 400);
    cp5.draw();

    text(frameRate, 50, 300);
  }

  image(pg, 0, 0);

  drawDepthROI();

  DwUtils.endScreen2D(g);

  // cam.beginHUD();
  // display result
  //
  //    image(skylight.sky.getSrc(), 0, 0);








  //




  int[][] motorsOn = getCells();
  rc.updateRenderScreen(motorsOn);
  rc.drawOfScreen();

  rc.draw();
  blockManager.setImage(rc.getRenderImg());

  text(frameRate, width-50, 10);
  fill(255);
  // PShape number = font.getShape('a');
  //shape(number, 200, 400);
  cam.endHUD();

  if (enablePort) {
    //if (frameCount % 15  == 0 ) {
    //color [] colors = blockManager.getColors();
    color []  colors  = blockManager.getAllNodeColors();
    byte [] data = serial.generateDataOnlyCol(colors);

    println("sending: "+moduleX * moduleY);
    serial.sendByte(data);

    //getNodeColors(currentNode);

    enablePort = false;
    //}
  }
}

void renderScene() {

  cam.getState().apply(pg);

  blockManager.update();

  pg.beginDraw();
  pg.background(0);
  pg.pushMatrix();
  ///pg.translate(width/2.0, height/2);
  pg.translate( -(moduleX*0.5 )*blockSize, -(moduleY*0.5)*blockSize);

  blockManager.update();
  blockManager.draw(pg);
  blockManager.drawBase(pg);
  pg.popMatrix();
  pg.endDraw();
}


public void keyReleased() {

  if (key == 'm') {
    enablePort = !enablePort;
  }

  if (key == 'a') {
    currentNode++;
    if (currentNode >=12) {
      currentNode =0;
    }
  }

  if (key == 's') {
    blockManager.getNodeColors(currentNode);
  }




  if (key == '1') {
    //state = cam.getState();

    float pos [] = cam.getPosition();
    float rot[]  = cam.getRotations();
    double dist  = cam.getDistance();
    float lookAt [] = cam.getLookAt();

    int[] view =  cam.getViewport();

    println("pos: "+pos[0]+" "+pos[1]+" "+pos[2]);
    println("rot: "+rot[0]+" "+rot[1]+" "+rot[2]);
    println("lookAt: "+lookAt[0]+" "+lookAt[1]+" "+lookAt[2]);
    println("dist: "+dist);
    println("view: "+view[0]+" "+view[1]+" "+view[2]+" "+view[3]);
  }

  if (key == '2') {

    //lookAt: 154.92358 -85.61942 151.10127
    Rotation rotn = new Rotation(RotationOrder.XYZ, -1.7617512, 0.96755165, 1.7974268);
    Vector3D npos = new Vector3D(  403.2791, 141.26372, 0.32108656);
    float dist = 555.4389675441226;
    CameraState state = new CameraState(rotn, npos, dist);
    cam.setState(state, 5000);
    cam.lookAt(-90.811424, -70.0859, 41.17713);
  }
}

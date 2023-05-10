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

void setup() {
  size(1280, 720, P2D);
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

  createMatrix();
}


void draw() {

  background(0);

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

  cam.getState().apply(pg);

  image(pg, 0, 0);

  cam.beginHUD();


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

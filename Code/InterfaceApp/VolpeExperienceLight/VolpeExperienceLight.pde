import java.util.ArrayList;
import processing.core.PImage;
import processing.core.PVector;
import peasy.*;
import peasy.PeasyCam;
import peasy.org.apache.commons.math.*;
import peasy.org.apache.commons.math.geometry.*;

float ry;

PeasyCam cam;

PShape volpeMapped;
PShape volpeOnly;
PShape VolpeBase;
PShape volepeOnlyBase;


boolean depthFullScreen = false;
boolean debugView = true;

//PShader depthShader;

int drawMode = 0;

public void settings() {
  size(viewport_w, viewport_h, P3D);
  smooth(0);
}

public void setup() {

  surface.setLocation(viewport_x, viewport_y);

  loadModels();
  //PShape  volpe = loadShape("volpe_clean.obj");
  //volpeMapped  = computeScale(volpe);

  cam = new PeasyCam(this, 30000);
  cam.setMinimumDistance(-3000);
  cam.setMaximumDistance(7000);

  Rotation rotn = new Rotation(RotationOrder.XYZ, -2.69932962, 0.7210216, -0.28469923);
  Vector3D npos = new Vector3D(  1857.1611, 353.98395, -1434.1498);
  float dist = 1512.9476534152527;
  CameraState state = new CameraState(rotn, npos, dist);
  cam.setState(state, 5000);
  cam.lookAt(858.3852, -132.38905, -407.06732);

  createGUI();

  perspective(60 * PI/180, width/(float)height, 1, 7000);

  // record list of vertices of the given shape
  DwVertexRecorder vertex_recorder = new DwVertexRecorder(this, volpeMapped);

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

  //depthShader = loadShader("frag.glsl", "vert.glsl");

  frameRate(1000);
  
  loadGenerative();


  //println(volpeMapped.getVertexCount()+" "+volpeMapped.getChildCount());
}

public void draw() {
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

  drawDepthROI();
  
  displayGenerative();
  DwUtils.endScreen2D(g);

  // cam.beginHUD();
  // display result
  //
  //    image(skylight.sky.getSrc(), 0, 0);

  cam.endHUD();
}

public void keyReleased() {
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
    Rotation rotn = new Rotation(RotationOrder.XYZ, -2.69932962, 0.7210216, -0.28469923);
    Vector3D npos = new Vector3D(  1857.1611, 353.98395, -1434.1498);
    float dist = 1512.9476534152527;
    CameraState state = new CameraState(rotn, npos, dist);
    cam.setState(state, 5000);
    cam.lookAt(858.3852, -132.38905, -407.06732);
  }

  if (key == '3') {

    //lookAt: 154.92358 -85.61942 151.10127
    Rotation rotn = new Rotation(RotationOrder.XYZ, 2.0954323, 1.465100, 1.0838872);
    Vector3D npos = new Vector3D( 1473.415, -203.3111, -617.7035);
    float dist = 18.5920658935149;
    CameraState state = new CameraState(rotn, npos, dist);
    cam.setState(state, 5000);
    cam.lookAt(1473.415, -203.3111, -617.7035, dist);
  }

  if (key == '4') {

    //lookAt: 154.92358 -85.61942 151.10127
    Rotation rotn = new Rotation(RotationOrder.XYZ, -0.9589054, 1.0257818, -2.1504865);
    Vector3D npos = new Vector3D(  1420.9451, 1.4322454, -477.4362);
    float dist =  594.03109491632471;
    CameraState state = new CameraState(rotn, npos, dist);
    cam.setState(state, 5000);
    cam.lookAt( 913.6671, -250.31334, -654.09546, dist);
  }


  if (key == 'c') {
    cam.setYawRotationMode(); //x
    //cam.setPitchRotationMode();
  }

  if (key == 'v') {
    cam.setPitchRotationMode();
  }

  if (key == 'b') {
    cam.setRollRotationMode();
  }
  if (key == 'n') {
    cam.setFreeRotationMode();
  }

  if (key == 'd') {
    if (applyDof) {
      depthZ.reloadShader();
    }
    applyDof = !applyDof;
  }
  if (key == 'f') {
    depthFullScreen = !depthFullScreen;
  }
  if (key == 's') {
    debugView = ! debugView;
  }
  if (key == 'z') {
    depthROIMouse = !depthROIMouse;
  }
  
  if(key == 'g'){
    enableGenerative = !enableGenerative;
  }

  if (key == 'z') {
    drawMode++;
    if (drawMode >=5) {
      drawMode = 0;
    }
    updateCamActiveStatus();
    CAM_ACTIVE = true;
    background(0);
  }

  if (key == 'x') {
    drawMode--;
    if (drawMode <0) {
      drawMode = 0;
    }
    updateCamActiveStatus();
    CAM_ACTIVE = true;
    background(0);
  }
}

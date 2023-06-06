import java.util.Locale;

import com.jogamp.opengl.GL2;
import com.thomasdiewald.pixelflow.java.DwPixelFlow;
import com.thomasdiewald.pixelflow.java.antialiasing.SMAA.SMAA;
import com.thomasdiewald.pixelflow.java.imageprocessing.filter.DepthOfField;
import com.thomasdiewald.pixelflow.java.imageprocessing.filter.DwFilter;
import com.thomasdiewald.pixelflow.java.render.skylight.DwSceneDisplay;
import com.thomasdiewald.pixelflow.java.render.skylight.DwScreenSpaceGeometryBuffer;
import com.thomasdiewald.pixelflow.java.render.skylight.DwSkyLight;
import com.thomasdiewald.pixelflow.java.render.skylight.DwSkyLightRenderer;
import com.thomasdiewald.pixelflow.java.render.skylight.DwSkyLightShader;
import com.thomasdiewald.pixelflow.java.utils.DwBoundingSphere;
import com.thomasdiewald.pixelflow.java.utils.DwColorPicker;
import com.thomasdiewald.pixelflow.java.utils.DwUtils;
import com.thomasdiewald.pixelflow.java.utils.DwVertexRecorder;

import controlP5.Accordion;
import controlP5.CColor;
import controlP5.CallbackEvent;
import controlP5.CallbackListener;
import controlP5.ControlP5;
import controlP5.Controller;
import controlP5.ControllerView;
import controlP5.Group;
import controlP5.Pointer;

import processing.core.PApplet;
import processing.core.PGraphics;
import processing.core.PMatrix3D;
import processing.core.PShape;
import processing.opengl.PGL;
import processing.opengl.PGraphics3D;
import processing.opengl.PGraphicsOpenGL;

int viewport_w = 1920;
int viewport_h = 1080;
int viewport_x = 0;
int viewport_y = 0;

public float clip_z_far = viewport_x;

float[] cam_pos = new float[3];

// renderer
DwSkyLight skylight;
DwPixelFlow context;

SMAA smaa;

PGraphics3D pg_tmp;

PGraphics3D pg_aa;
PGraphics3D pg_render;
PGraphics3D pg_depth;
PGraphics3D pg_color;

boolean CAM_ACTIVE = false;

//dof
DepthOfField dof;
DepthField   depthZ;

DwScreenSpaceGeometryBuffer geombuffer;
boolean applyDof = false;

boolean depthROIMouse = false;
PVector depthROI = new PVector(520, 20);

public void displayScene(PGraphicsOpenGL canvas) {
  if (canvas == skylight.renderer.pg_render) {
    canvas.background(0);
  }

  if (canvas == geombuffer.pg_geom) {
    canvas.pgl.clearDepth(1.0f);
    canvas.pgl.clearColor(1, 1, 1, clip_z_far);
    canvas.pgl.clear(PGL.COLOR_BUFFER_BIT | PGL.DEPTH_BUFFER_BIT);
  }

  PShape ps = blockManager.getShape();
  canvas.shape(ps);
}

public void updateCamActiveStatus() {
  float[] cam_pos_curr = cam.getPosition();
  CAM_ACTIVE = false;
  CAM_ACTIVE |= cam_pos_curr[0] != cam_pos[0];
  CAM_ACTIVE |= cam_pos_curr[1] != cam_pos[1];
  CAM_ACTIVE |= cam_pos_curr[2] != cam_pos[2];
  cam_pos = cam_pos_curr;
}


public boolean resizeScene() {

  viewport_w = width;
  viewport_h = height;

  boolean[] RESIZED = {false};

  pg_aa     = DwUtils.changeTextureSize(this, pg_aa, width, height, 0, RESIZED);
  pg_render = DwUtils.changeTextureSize(this, pg_render, width, height, 0, RESIZED);
  pg_depth  = DwUtils.changeTextureSize(this, pg_render, width, height, 0, RESIZED);
  pg_color  = DwUtils.changeTextureSize(this, pg_render, width, height, 0, RESIZED);
  pg_tmp    = DwUtils.changeTextureSize(this, pg_tmp, width, height, 0, RESIZED, GL2.GL_RGBA16F, GL2.GL_RGBA, GL2.GL_FLOAT);

  skylight.resize(width, height);

  if (RESIZED[0]) {
    // nothing here
  }
  cam.feed();
  perspective(60 * PI/180, width/(float)height, 1, 7000);
  return RESIZED[0];
}


void renderDOF() {
  if (applyDof) {
    int mult_blur = 15;

    geombuffer.update(skylight.renderer.pg_render);
    DwFilter filter = DwFilter.get(context);
    filter.gaussblur.apply(geombuffer.pg_geom, geombuffer.pg_geom, pg_tmp, 3);

    depthZ.param.focus_pos = new float[]{0.5f, 0.5f};
    //      dof.param.focus_pos[0] = map(mouseX, 0, width , 0, 1);
    //      dof.param.focus_pos[1] = map(mouseY, 0, height, 1, 0);
    depthZ.param.mult_blur = mult_blur;
    depthZ.param.clip_z_far = clip_z_far;
    depthZ.apply(pg_aa, pg_render, geombuffer);
    filter.copy.apply(pg_render, pg_depth);
    filter.copy.apply(pg_render, pg_aa);
  }
}

///depth functions
void calculateDepth() {

  geombuffer.update(skylight.renderer.pg_render);
  DwFilter filter = DwFilter.get(context);
  filter.gaussblur.apply(geombuffer.pg_geom, geombuffer.pg_geom, pg_tmp, 3);

  depthZ.param.clip_z_far = clip_z_far;
  depthZ.apply(pg_aa, pg_render, geombuffer);
  filter.copy.apply(pg_render, pg_aa);
}

void drawDepthROI() {
  if (depthROIMouse) {
    depthROI.x = mouseX;
    depthROI.y = mouseY;
  }
  pushStyle();
  noFill();
  stroke(0, 100, 50);
  strokeWeight(5);
  text( depthROI.x+", "+depthROI.y, depthROI.x-50, depthROI.y);
  rect(depthROI.x, depthROI.y, 1024, 1024);
  popStyle();
}

import peasy.*;
import peasy.PeasyCam;
import peasy.org.apache.commons.math.*;
import peasy.org.apache.commons.math.geometry.*;
import java.io.ByteArrayOutputStream;
import java.nio.ByteBuffer;
import ch.bildspur.postfx.builder.*;
import ch.bildspur.postfx.pass.*;
import ch.bildspur.postfx.*;

PostFX fx;
PeasyCam cam;

//BlockManager blockManager; 

//SerialHandler serial;
boolean enablePort = false;

PGraphics pg;

int numPanels = 2;
int nodesX = 2;
int nodesY = 4;

int moduleX = 12; // 3 * 2 * 2
int moduleY = 8; // 2 * 4 

int pixelsPerNode = 8;
int blockSize = 10; 

//RendererScreen rc;

PFont font;

int currentNode = 0;

float bri = 0.0;

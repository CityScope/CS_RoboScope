class Block {

  float motorheight;
  byte interaction;
  int id;
  color col;

  PVector loc;
  PVector size;

  float minHeight = 0;
  float maxHeight = 255;
  boolean animDone = false;
  float timeStep = 0.0;

  float currHeight = 0;
  float newHeight = 0;
  float prevheight = 0;

  int localId;
  int moduleId;
  int nodeId;

  PShape texImg;
  //8 x 12 =96
  
  PShape box;


  Block(int id, int tam) {
    this.localId = id;
    int posX = id % moduleX;
    int posY = (id / moduleX);

    int lid = posX + moduleX*posY;
    moduleId = posY;
   // nodeId = posX;

    println("index: "+moduleId+" "+nodeId+" "+lid);
    size = new PVector(tam, tam, 1);
    loc = new PVector(posX*size.x, posY*size.y);

    col = color(0, 100, 200, 150);

    texImg = createShape();
    
    box = createShape(BOX, size.x*0.8, size.y*0.8, size.z * currHeight/4);
  }
  
  void setNodeId(int id){
    nodeId = id;
  }
 


  //gets
  int getCurrHeight() {
    return int(currHeight);
  }
  color getCurrColor() {
    return col;
  }

  void setColor(color col) {
    this.col = col;
  }
  void draw(PGraphics pg) {
    drawRect(pg);
  }

  void setHeight(color col) {
    float bri = brightness(col);
    prevheight = currHeight;
    newHeight = map(bri, 0, 255, minHeight, maxHeight);
    if (newHeight != prevheight) {
      animDone = true;
      timeStep = 0;
    }
  }

  void update() {
    if (animDone) {
      currHeight =  lerp(prevheight, newHeight, timeStep);
      timeStep += 0.01;
    }
  }

  void drawRect(PGraphics pg) {

    //draw the complete rod
    pg.pushMatrix();
    //float zVal = map(frameCount%200, 0, 255, minHeight, maxHeight);
    pg.scale(1, 1, size.z * currHeight/4);
    pg.translate(loc.x, loc.y, currHeight/8.0);
    pg.noFill();
    pg.stroke(255, 50);

    pg.shape(box);//(size.x*0.8, size.y*0.8, size.z );

    pg.fill(255, 0, 0);
    pg.textSize(8);
    pg.text(localId, -3, 2);
    pg.popMatrix();

    //draw only the top
    pg.pushMatrix();
    pg.translate(loc.x, loc.y, currHeight/4 + 1/2);
    pg.fill(col, 150);
    pg.noStroke();
    pg.box(size.x*0.8, size.y*0.8, 1 );
    pg.popMatrix();
    // pg.fill(200, 100);
    //pg.translate(loc.x, loc.y, 0);
    //pg.ellipse(0, 0, 10, 10);
  }
}

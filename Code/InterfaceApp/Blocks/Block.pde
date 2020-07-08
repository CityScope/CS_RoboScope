/*
  Individual Block or pin
 */
class Block {

  //corner position
  int posx;
  int posy;

  //center position
  int centerPosX;
  int centerPosY;

  //building height
  int maxheight = 500;
  int midHeight = int(maxheight/2.0);
  float bHeight = midHeight;

  //animation
  float finalHeight = midHeight;
  boolean stopAnim  = false;
  float animCounter = 0.0;

  //size of the block
  int tam = 10; //default size

  //unique id
  int id;

  //modfied type of block
  int typeId = -1;

  //selected
  boolean selected = false;

  //color type 
  color colorMap;

  //constructor for the block
  Block(int posx, int posy, int tam) {
    this.posx = posx;
    this.posy = posy;
    this.tam  = tam;

    //update center
    centerPosX = posx + int((float)tam/2.0);
    centerPosY = posy + int((float)tam/2.0);

    colorMap = color(0);
  }

  Block() {
  }

  //update the hight of the rod
  void updateHeight(int value) {
    finalHeight = (int)map(value, 50, 255, midHeight, 20);
    //finalHeight = bHeight;
  }

  //draw the Blcok
  void drawBlock() {
    pushMatrix();
    translate(centerPosX, centerPosY, -(bHeight - midHeight) ); //max = max_height/2.0
    //println(   bHeight - midHeight);
    noFill();
    stroke(50);
    fill(250);
    box(tam, tam, maxheight);
    popMatrix();

    pushMatrix();
    translate(0, 0, -(bHeight - maxheight)  + 1);
    drawLego();
    drawContour();
    popMatrix();

    //animate Pins
  }

  //animate the moving block or pin
  void animateBlock() {
    bHeight = midHeight + (finalHeight - midHeight)*(animCounter);

    animCounter+=0.0025;
    if (animCounter>= 1.0) {
      animCounter =1.0;
    }
  }

  //draw Lego piece on top
  void drawLego() {

    noStroke();
    fill(0);
    rect(posx, posy, tam, tam);

    stroke(200);
    noFill();
    int eTam = 2;
    stroke(150);
    line(posx + tam/2.0, posy, posx + tam/2.0, posy + tam);
    line(posx, posy + tam/2.0, posx + tam, posy + tam/2.0); 

    ellipse(posx + tam/4.0, posy + tam/4.0, eTam, eTam);
    ellipse(posx + 3*(tam/4.0), posy + tam/4.0, eTam, eTam);

    ellipse(posx + (tam/4.0), posy + 3*(tam/4.0), eTam, eTam);
    ellipse(posx + 3*(tam/4.0), posy + 3*(tam/4.0), eTam, eTam);
  }

  //draw the color of the block
  void drawContour() {
    noFill();
    stroke(255);
    rect(posx, posy, tam, tam);

    if (selected) {
      fill(255);
      rect(posx, posy, tam+1, tam+1);
    }

    if (typeId == 0) {
      fill(255, 100, 55, 150);
      rect(posx, posy, tam, tam);
    }
    if (typeId == 1) {
      fill(200, 0, 255, 150);
      rect(posx, posy, tam, tam);
    }

    if (typeId == 2) {
      fill(0, 200, 255, 150);
      rect(posx, posy, tam, tam);
    }

    if (typeId == 3) {
      fill(180, 150);
      rect(posx, posy, tam, tam);
    }

    if (typeId == 4) {
      fill(colorMap);
      rect(posx, posy, tam, tam);
    }
  }

  void setColorMap(color m) {
    colorMap = m;
  }

  int getCenterX() {
    return  centerPosX;
  }

  int getCenterY() {
    return centerPosY;
  }

  void enableSelected() {
    selected = true;
  }

  void disableSelection() {
    selected = false;
  }

  void setBlockSize(int tam) {
    this.tam = tam;

    //update center
    centerPosX = posx + int((float)tam/2.0);
    centerPosY = posy + int((float)tam/2.0);
  }

  void setId(int id) {
    this.id = id;
  }

  int getId() {
    return id;
  }

  float getBlockSize() {
    return tam;
  }

  int getType() {
    return  typeId;
  }

  void setType(int type) {
    typeId = type;
  }
}

class RendererScreen {

  PGraphics pg;

  float incX = 0.0;
  float dir = 1;


  RendererScreen(int sw, int sh) {
    pg = createGraphics(sw, sh, P2D);
    println("render img: "+pg.width+" "+pg.height);
    pg.beginDraw();

    pg.endDraw();
  }


  void drawOfScreen() {
    pg.beginDraw();
    pg.background(0);

    pg.noStroke();
    pg.fill(255);
    pg.ellipse(pg.width/2, pg.height/2, incX, incX);

    incX += 0.006 * dir;
    if (incX > pg.width) {
      incX =0;
    }
    pg.endDraw();
  }
  
  PImage getRenderImg(){
    return pg;
  }

  void draw() {
    stroke(255);
    rect(0, 0, 100, 100);
    image(pg, 0, 0, 100, 100);
  }
}

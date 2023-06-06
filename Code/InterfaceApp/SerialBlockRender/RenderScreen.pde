class RendererScreen {

  PGraphics pg;

  float incX = 0.0;
  float dir = 1;

  int HIGHRise_0 = 170;
  int HIGHRise_1 =  140;
  int HIGHRise_2 =  120;
  int HIGHRise_3 =  95;

  int MIDRise_0 =  40;
  int MIDRIse_1 =  30;
  int MIDRIse_3 =  20;

  int volpeModel [] [] = {
    {130, 35, 130, 35, 0, 20, 20, 20, 0, 0, 0, 0},
    {35, 35, 35, 35, 0, 0, 0, 0, 0, 0, 0, 0},
    {130, 0, 130, 35, 90, 35, 90, 35, 0, 0, 0, 0},
    {35, 0, 35, 0, 0, 0, 0, 35, 0, 0, 0, 0},
    {130, 0, 95, 0, 20, 20, 0, 90, 0, 0, 0, 0},
    {35, 0, 35, 0, 20, 20, 0, 35, 0, 0, 0, 0},
    {130, 0, 95, 0, 20, 20, 0, 90, 0, 0, 0, 0},
    {35, 0, 35, 0, 20, 20, 0, 35, 0, 0, 0, 0}
  };


  RendererScreen(int sw, int sh) {
    pg = createGraphics(sw, sh, P2D);
    println("render img: "+pg.width+" "+pg.height);
    pg.beginDraw();

    pg.endDraw();

    pg.loadPixels();
  }

  void updateRenderScreen(int values [][]) {
    pg.beginDraw();
    pg.loadPixels();

    for (int x = 0; x < pg.width; x++) {
      for (int y = 0; y < pg.height; y++) {
        int loc = x + y*pg.width;
        // if(values[x][y] > 0){
        // pg.pixels[loc] = 255;//values[x][y];
        if (volpeModel[y][x] > 0) {
          pg.pixels[loc] = volpeModel[y][x];
          //println("255");
        } else {
          pg.pixels[loc] = color(0);
        }
      }
    }
    pg.updatePixels();
    pg.endDraw();
  }


  void drawOfScreen() {

    // pg.background(0);
    /*
    pg.noStroke();
     pg.fill(255, 0, 0);
     pg.ellipse(pg.width/2, pg.height/2, incX*20, incX*20);
     
     incX += 0.01 * dir;
     if (incX > 1.0) {
     incX =0;
     }
     */
    // pg.endDraw();
  }

  PImage getRenderImg() {

    return pg;
  }

  void draw() {
    stroke(255);
    rect(0, 0, 100, 100);
    image(pg, 0, 0, 100, 100);
  }
}

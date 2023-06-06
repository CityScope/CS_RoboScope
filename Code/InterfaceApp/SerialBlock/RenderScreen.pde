class RendererScreen {

  PGraphics pg;
  PGraphics colorPg;

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
    {130, 35, 130, 35, 10, 20, 20, 20, 0, 0, 0, 0},
    {35, 35, 35, 35, 10, 10, 10, 10, 0, 0, 0, 0},
    {130, 10, 130, 35, 90, 35, 90, 35, 0, 0, 0, 0},
    {35, 10, 35, 10, 10, 10, 10, 35, 0, 0, 0, 0},
    {130, 10, 95, 10, 20, 20, 10, 90, 0, 0, 0, 0},
    {35, 10, 35, 10, 20, 20, 10, 35, 0, 0, 0, 0},
    {130, 10, 95, 10, 20, 20, 10, 90, 0, 0, 0, 0},
    {35, 10, 35, 10, 20, 20, 10, 35, 0, 0, 0, 0}
  };

  int modelClean [] [] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
  };

  int modelTemp [] [];



  color colorMode(int colType) {
    color col = color(255);
    switch(colType) {
    case 130:
      col = color(215, 30, 252);
      break;
    case 35:
      col = color(193, 163, 56);
      break;
    case 95:
      col = color(106, 10, 170);
      break;
    case 10:
      col = color(180);
      break;
    case 20:
      col = color(10, 122, 190);
      break;
    case 90:
      col = color(85, 10, 188);
      break;
    }
    return col;
  }


  RendererScreen(int sw, int sh) {
    pg = createGraphics(sw, sh, P2D);
    println("render img: "+pg.width+" "+pg.height);
    pg.beginDraw();
    pg.endDraw();

    pg.loadPixels();

    colorPg = createGraphics(sw, sh, P2D);
    println("render img: "+pg.width+" "+pg.height);
    colorPg.beginDraw();
    colorPg.endDraw();

    colorPg.loadPixels();

    modelTemp= modelClean;
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
          pg.pixels[loc] = modelTemp[y][x];
          //println("255");
        } else {
          pg.pixels[loc] = color(0);
        }
      }
    }
    pg.updatePixels();
    pg.endDraw();

    colorPg.beginDraw();
    colorPg.loadPixels();

    for (int x = 0; x < colorPg.width; x++) {
      for (int y = 0; y < colorPg.height; y++) {
        int loc = x + y*colorPg.width;
        // if(values[x][y] > 0){
        // pg.pixels[loc] = 255;//values[x][y];
        if (volpeModel[y][x] > 0) {
          colorPg.pixels[loc] = colorMode(modelTemp[y][x]);
          //println("255");
        } else {
          colorPg.pixels[loc] = color(0);
        }
      }
    }
    colorPg.updatePixels();
    colorPg.endDraw();
  }


  void drawOfScreen() {
    image(colorPg, 0, 0);
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

  PImage getRenderColorImg() {
    return colorPg;
  }

  void cleanModel() {
    modelTemp = modelClean;
  }

  void animate() {
    modelTemp = volpeModel;
  }



  void draw() {
    stroke(255);
    rect(0, 0, 100, 100);
    image(pg, 0, 0, 100, 100);
  }
}

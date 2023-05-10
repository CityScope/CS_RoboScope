import controlP5.*;

ControlP5 cp5;



void createMatrix() {
  cp5 = new ControlP5(this);

  cp5.addMatrix("myMatrix")
    .setPosition(20, 150)
    .setSize(200, 200)
    .setGrid(moduleX, moduleY)
    .setGap(5, 5)
    .setColorBackground(color(120))
    .setBackground(color(40))
    .setMode(ControlP5.MULTIPLES)
    ;

  cp5.getController("myMatrix").getCaptionLabel().alignX(CENTER);

  cp5.get(Matrix.class, "myMatrix").pause();
}

int[][] getCells() {
  return cp5.get(Matrix.class, "myMatrix").getCells();
}

void myMatrix(int theX, int theY) {
  println("got it: "+theX+", "+theY);
}

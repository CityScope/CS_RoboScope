/*
Draw table 
 
 */

void drawTable() {
  pushMatrix();
  float heightPins = 174; //174
  float widthTable = 5;
  translate(gridStartX - blockSize*2, gridStartY - blockSize*2, heightPins);
  float x =  (gridX) * (blockSize) + (gridX - 1)*gridSpace + blockSize*4;
  float y =  (gridY) * (blockSize) + (gridY - 1)*gridSpace + blockSize*4;


  createCube(x, y, 75);

  popMatrix();
}


void drawBase() {
  pushMatrix();
  translate(0, 0, -435);
  float heightPins = 174;
  float widthTable = 10;
  
  translate(gridStartX - blockSize*2, gridStartY - blockSize*2, heightPins);
  float x =  (gridX) * (blockSize) + (gridX - 1)*gridSpace + blockSize*4;
  float y =  (gridY) * (blockSize) + (gridY - 1)*gridSpace + blockSize*4;

  createCube(x, y, 5);

  popMatrix();
}

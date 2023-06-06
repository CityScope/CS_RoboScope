class BlockManager {

  ArrayList<Block> blocks;

  PVector numBlocks;
  int blockSize;
  int numPixels;
  
  PShape psBlocks;

  BlockManager(int x, int y, int numPixels, int size) {
    blocks = new ArrayList<Block>();

    for (int i = 0; i < (x * y); i++) {
      blocks.add(new Block(i, size));
    }
    this.numBlocks = new PVector(x, y);
    this.numPixels = numPixels;
    blockSize = size;

    int bsize = int((x*y)/numPixels);

    println("num nodes id: "+bsize);
    
    psBlocks = createShape(GROUP);
  }

  void draw(PGraphics pg) {
    for (Block bl : blocks) {
      bl.draw(pg);
    }
  }

  void update() {
    for (Block bl : blocks) {
      bl.update();
    }
  }
  
  PShape getShape(){
   return psBlocks; 
  }

  void setImage(PImage renderInput) {
    renderInput.loadPixels();
    for (int i = 0; i < blocks.size(); i++) {
      int px = i % renderInput.width;
      int py = i / renderInput.width;
      color renderColor = renderInput.get(px, py);
      blocks.get(i).setColor(renderColor);
      blocks.get(i).setHeight(renderColor);
    }
  }

  int [] getHeights() {
    int heights [] = new int[ blocks.size()];
    for (int i = 0; i < blocks.size(); i++) {
      heights[i] = blocks.get(i).getCurrHeight();
    }
    return heights;
  }

  color []  getColors() {
    int colors [] = new int[ blocks.size()];
    for (int i = 0; i < blocks.size(); i++) {
      colors[i] = blocks.get(i).getCurrColor();
    }
    return colors;
  }

  color [] getNodeColors(int currentNode) {
    int colors [] = new int[ numPixels];
    println(currentNode+" :");
    for (int i = 0; i < numPixels; i++) {
      int indeX = i % nodesX + (currentNode%6)*2;
      int indeY = i / nodesX + (currentNode/6)*4;
      int index = int(indeX + indeY*numBlocks.x);
      colors[i] = blocks.get(index).getCurrColor();
      println(currentNode+": "+indeX+" "+indeY+" "+index);
    }
    return colors;
  }

  color [] getAllNodeColors() {
    int colors [] = new int[ int(numBlocks.x * numPixels)];
    println(numBlocks.x * numPixels+" :");
    for (int j = 0; j < numBlocks.x; j++) {
      for (int i = 0; i < numPixels; i++) {
        int indeX = i % nodesX + (j%6)*2;
        int indeY = i / nodesX + (j/6)*4;
        int index = int(indeX + indeY*numBlocks.x);
        colors[i +j*numPixels] = blocks.get(index).getCurrColor();
        println(i +j*numPixels+": "+indeX+" "+indeY+" "+index);
      }
    }
    return colors;
  }




  void drawBase(PGraphics pg) {
    pg.pushMatrix();
    //pg.fill(100, 10);
    pg.noFill();
    pg.stroke(0, 200, 150, 100);
    pg.translate(numBlocks.x*blockSize*0.5 - blockSize/2, numBlocks.y*blockSize*0.5 - blockSize/2, -20/2);
    pg.box(numBlocks.x*blockSize*1.2, numBlocks.y*blockSize*1.2, 20);
    pg.popMatrix();
  }
}

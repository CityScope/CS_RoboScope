class BlockManager {

  ArrayList<Block> blocks;

  PVector numBlocks;
  int blockSize;
  int numPixels;

  BlockManager(int x, int y, int numPixels, int size) {
    blocks = new ArrayList<Block>();

    for (int i = 0; i < (x * y); i++) {
      blocks.add(new Block(i, size));
    }
    this.numBlocks = new PVector(x, y);
    this.numPixels = numPixels;
    blockSize = size;
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

  void setImage(PImage renderInput) {
    //renderInput.loadPixels();
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

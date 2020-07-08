/*
  Collection of blocks that form a grid
 */
class Grid {
  ArrayList<Block> grid;
  ArrayList<Road> road;

  int gridX;
  int gridY;

  int startPosX;
  int startPosY;

  int endPosX;
  int endPosY;

  int blockSize = 20;

  //roads
  int stepX = 10;
  int stepY = 10;


  //draw roads
  boolean drawRoads = false;

  //current
  Block currentBlock;


  //build grid
  Grid(int startPosX, int startPosY, int gridX, int gridY, int bSize, int step) {
    this.gridX = gridX;
    this.gridY = gridY;

    this.startPosX = startPosX;
    this.startPosY = startPosY;

    this.stepX = step;
    this.stepY = step;

    this.blockSize = bSize;

    this.endPosX = gridX * blockSize + stepX * (gridX - 1); 
    this.endPosY = gridY * blockSize + stepY * (gridY - 1);

    grid = new ArrayList<Block>();
    road = new ArrayList<Road>();

    //create
    createGrid();
  }

  void draw() {
    for (Block bl : grid) {
      //bl.draw();
      if (currentBlock != null) {
        if (bl.getId() == currentBlock.getId()) {
          bl.enableSelected();
        } else {
          bl.disableSelection();
        }
      }
      

      bl.drawBlock();
      if (activeAnimation) {
        bl.animateBlock();
      }
    }

    //
    if (drawRoads) {
      for (Road rd : road) {
        rd.draw();
      }
    }
  }

  //update 3d picker
  void updatePicker(Picker picker) {
    for (Block bl : grid) {
      picker.start(bl.getId());
    }
  }

  
  void drawContour() {
    noFill();
    stroke(255);
    strokeWeight(1.5);
    rect(startPosX, startPosY, endPosX, endPosY);

    strokeWeight(1);
  }

  int getEndX() {
    return endPosX;
  }

  int getEndY() {
    return endPosY;
  }

  //draw road
  void drawRoad() {
  }

  //create grid
  void createGrid() {

    grid.clear();
    road.clear();

    //fill grid
    for (int i = 0; i < gridX; i++) {
      for (int j = 0; j < gridY; j++) {
        int posx = startPosX + blockSize * i + stepX * i;
        int posy = startPosY + blockSize * j + stepY * j;
        Block block  = new Block(posx, posy, blockSize);
        block.setBlockSize(blockSize);
        block.setId(i + j * gridX);
        grid.add(block);
      }
    }

    //Roads

    for (int i = 0; i < gridX; i++) {
      for (int j = 0; j < gridY; j++) {
        int posx = startPosX + blockSize * i + stepX * i + blockSize;
        int posy = startPosY + blockSize * j + stepY * j;

        int lengthX = 10;
        int lengthY = 20;

        Road rd  = new Road(posx, posy, lengthX, lengthY);
        road.add(rd);
      }
    }
  }

  /*
  Values
   */
  void setGridDim(float  [] dim) {
    gridX = int(dim[0]);
    gridY = int(dim[1]);

    endPosX = gridX * blockSize + stepX * (gridX - 1); 
    endPosY = gridY * blockSize + stepY * (gridY - 1);

    createGrid();
  }

  void setSetSpace(float [] space) {

    stepX = int(space[0]);
    stepY = int(space[1]);

    endPosX = gridX * blockSize + stepX * (gridX - 1); 
    endPosY = gridY * blockSize + stepY * (gridY - 1);

    createGrid();
  }

  Block getCurrentBlock(int posx, int posy) {
    for (Block bl : grid) {
      float ds = dist(posx, posy, bl.posx, bl.posy);
      if (ds < blockSize) {
        currentBlock = bl;
        return bl;
      }
    }

    return null;
  }
  
  //get block 
  Block getBlock(int id){
    return grid.get(id);
  }
  
  void updateCurrentBlock(Block cbl){
    currentBlock = cbl;
  }

  void updateBlockType(Block cbl) {
    for (Block bl : grid) {
      if (cbl != null) {
        if (bl.getId() == cbl.getId()) {
          bl.setType(cbl.getType());
          println("new "+cbl.getType()+" "+cbl.getId());
        }
      }
    }
  }

  //set colors of the blocks based on the map
  void setGridColors(PImage map, PImage cMap) {
    PImage densityMap = map;
    PImage colorMap = cMap; 
    densityMap.resize(1920, 1080+blockSize/2);
    colorMap.resize(1920, 1080+blockSize/2);
    for (Block bl : grid) {
      int cx = bl.getCenterX();
      int cy = bl.getCenterY();


      color mapc = colorMap.get(cx, cy);
      color mapd = densityMap.get(cx, cy);
      //color
      bl.setType(4);
      bl.setColorMap(mapc);
      //height
      int colorHeight = int(red(mapd));
      bl.updateHeight(colorHeight);
    }
  }

  void setBlockSize(int bsize) {

    blockSize = bsize;
    endPosX = gridX * blockSize + stepX * (gridX - 1); 
    endPosY = gridY * blockSize + stepY * (gridY - 1);

    createGrid();
  }
}

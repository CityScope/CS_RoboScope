class Building {
  int maxHeight;
  int currentHeight;

  float currHeightCounter = 0.0;
  float heightCounter= 0.0;

  int id = 0;
  String buildingName;

  ArrayList<PShape> floors  = new ArrayList<PShape> ();

  Building(String name, int maxHeight) {
    this.maxHeight = maxHeight;

    for (int i = 0; i < maxHeight; i++) {
      PShape floor = loadShape(name);
      floor =  computeScale(floor);
      floor.translate(0, 27.5*i, 0);
      floors.add(floor);
    }
  }

  void draw(PGraphicsOpenGL pg) {
    for ( int i = 0; i < currentHeight; i++ ) {
      pg.shape(floors.get(i));
    }
  }

  void animateheight() {
    currHeightCounter += 0.0001;
    currentHeight  = floor(currHeightCounter);
    if (currentHeight >= maxHeight) {
      currentHeight = 0;
      currHeightCounter =0.0;
    }
  }
}

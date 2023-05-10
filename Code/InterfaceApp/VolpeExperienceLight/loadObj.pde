//compute bouding box

PVector screenBounds = new PVector(1920, 1080, 300);
PVector centerModel = new PVector();

PVector maxBounds = new PVector();
PVector minBounds = new PVector();

ArrayList<Building>  buildings = new ArrayList<Building >();

int maxFloors [] = {7, 7, 8, 8, 9, 9, 9, 9, 10, 10, 10, 10};

void loadModels() {
  volpeMapped = loadShape("volpe_clean.obj");
  volpeOnly  = loadShape("volpe_noBase.obj");
  volepeOnlyBase = loadShape("volpe_base.obj");

  //compute scale based on the orignal model
  computerBounds(volpeMapped);

  //use the same scale for all the models
  volpeMapped =  computeScale(volpeMapped);
  volpeOnly =  computeScale(volpeOnly);
  volepeOnlyBase =  computeScale(volepeOnlyBase);

  for (int j = 0; j < 12; j++) {
    Building builds  = new Building("A"+(j+1)+".obj", maxFloors[j]);
    buildings.add(builds);
  }

  //volpeOnly = computeScale(volpe);


  //volpeMapped = computeScale(volpe);
}


void computerBounds( PShape  model) {
  //computer bounds
  float minX = Float.MAX_VALUE;
  float minY = Float.MAX_VALUE;
  float minZ = Float.MAX_VALUE;

  float maxX = -Float.MAX_VALUE;
  float maxY = -Float.MAX_VALUE;
  float maxZ = -Float.MAX_VALUE;

  for (int i = 0; i < model.getChildCount(); i++) {
    PShape v = model.getChild(i);
    for (int j = 0; j <  v.getVertexCount(); j++) {
      PVector ver = v.getVertex(j);

      if ( ver.x < minX ) minX = ver.x;
      if ( ver.x > maxX ) maxX = ver.x;

      if ( ver.y < minY ) minY = ver.y;
      if ( ver.y > maxY ) maxY = ver.y;

      if ( ver.z < minZ ) minZ = ver.z;
      if ( ver.z > maxZ ) maxZ = ver.z;
    }
  }
  maxBounds = new PVector(maxX, maxY, maxZ);
  minBounds = new PVector(minX, minY, minZ);
}

PShape computeScale( PShape  volpe) {
  PShape volpeMapped = createShape(GROUP);

  //generate a new obj with the new mapping
  for (int i = 0; i < volpe.getChildCount(); i++) {
    PShape v = volpe.getChild(i);
    PShape childV  = createShape();
    childV.beginShape();
    fill(255);
    noStroke();

    for (int j = 0; j <  v.getVertexCount(); j++) {
      PVector ver = v.getVertex(j);
      float mx =  map(ver.x, minBounds.x, maxBounds.x, 0, screenBounds.x);
      float my =  map(ver.y, minBounds.y, maxBounds.y, screenBounds.z, 0);
      float mz =  map(ver.z, minBounds.z, maxBounds.z, 0, screenBounds.y);

      childV.vertex(mx, -my, -mz);
    }

    childV.endShape(CLOSE);
    volpeMapped.addChild(childV);
  }
  //centerModel.x =  abs(maxX - minX)/2.0;
  //centerModel.y =  abs(maxY - minY)/2.0;

  return volpeMapped;

  //volpeMapped.translate(-screenBounds.x/2.0, -screenBounds.z/2, -screenBounds.y/2.0);
}

//load image from the render

void loadGenerativeImg() {
}

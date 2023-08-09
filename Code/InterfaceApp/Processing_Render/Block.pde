class Block {
  float motorHeight;
  int id;
  color col;
  
  PVector location;
  PVector size;
  
  float minHeight;
  float maxHeight;
  boolean animDone = false;
  float timestep = 0.0;
  
  float currHeight;
  float newHeight;
  float prevHeight;
  
  int localId;
  int moduleId;
  int nodeId;
  
  PShape textImg; // 8 x 12 = 96
  
  Block(int id, int tam) {
    this.localId = id; 
    int posX = id % moduleX;
  }
  
}

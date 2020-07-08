/*
 create Cube Class
 */
float xmag, ymag = 0;
float newXmag, newYmag = 0; 

void createCube(float scaleX, float scaleY, float scaleZ) {

  noStroke();

  scale(scaleX, scaleY, scaleZ);
  beginShape(QUADS);

  //stroke(255);
  fill(30, 30, 30); 
  vertex(0, 1, 1);
  fill(30, 30, 30); 
  vertex( 1, 1, 1);
  fill(30, 30, 30); 
  vertex( 1, 0, 1);
  fill(30, 30, 30); 
  vertex(0, 0, 1);

  fill(30, 30, 30); 
  vertex( 1, 1, 1);
  fill(30, 30, 30);  
  vertex( 1, 1, 0);
  fill(30, 30, 30); 
  vertex( 1, 0, 0);
  fill(30, 30, 30); 
  vertex( 1, 0, 1);

  fill(30, 30, 30); 
  vertex( 1, 1, 0);
  fill(30, 30, 30); 
  vertex(0, 1, 0);
  fill(30, 30, 30); 
  vertex(0, 0, 0);
  fill(30, 30, 30); 
  vertex( 1, 0, 0);

  fill(30, 30, 30); 
  vertex(0, 1, 0);
  fill(30, 30, 30); 
  vertex(0, 1, 1);
  fill(30, 30, 30); 
  vertex(0, 0, 1);
  fill(30, 30, 30); 
  vertex(0, 0, 0);

  fill(30, 30, 30); 
  vertex(0, 1, 0);
  fill(30, 30, 30); 
  vertex( 1, 1, 0);
  fill(30, 30, 30); 
  vertex( 1, 1, 1);
  fill(30, 30, 30); 
  vertex(0, 1, 1);

  fill(0, 0, 0);  
  vertex(0, 0, 0);
  fill(30, 30, 30); 
  vertex( 1, 0, 0);
  fill(30, 30, 30); 
  vertex( 1, 0, 1);
  fill(30, 30, 30);  
  vertex(0, 0, 1);

  endShape();

  //popMatrix();
}

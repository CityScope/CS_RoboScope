/*
 Road class
*/
class Road{
  int startPosX;
  int startPosY;
  
  int lengthX;
  int lengthY;
  
  //ids
  int id;  
  
  Road(int startPosX, int startPosY, int lengthX, int lengthY){
    this.startPosX = startPosX;
    this.startPosY = startPosY;
    this.lengthX = lengthX;
    this.lengthY = lengthY;
  }
  
  void draw(){
    
    noStroke();
    fill(0, 200, 0);
    rect(startPosX, startPosY, lengthX, lengthY);
  }
  
  
}

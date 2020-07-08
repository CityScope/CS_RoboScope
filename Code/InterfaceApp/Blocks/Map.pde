/*

*/
class Map {
  PImage imgColor;
  PImage imgDensity;

  Map() {
    imgDensity = loadImage("grd_boston.png");
    imgColor = loadImage("grd_boston_color.png");
  }

  PImage getImgColor() {
    return imgColor;
  }
  
    PImage getImgDensity() {
    return imgDensity;
  }


}

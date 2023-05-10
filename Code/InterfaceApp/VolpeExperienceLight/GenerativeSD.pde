PImage generativeSD;
boolean enableGenerative = false;

void loadGenerative() {
  generativeSD = loadImage("output/generative_1024.png");
}

void displayGenerative() {
  if ( enableGenerative) {
    image(generativeSD, 0, 0);
  }
}

/*
 * Class that manages Debug information for the board
 * 
*/
#ifndef DEBUG_H
#define DEBUG_H

class DEBUG_LOG{
  
  public:

  DEBUG_LOG(){
      neopixels = new Adafruit_NeoPixel(1, NEO_DEBUG, NEO_GRBW + NEO_KHZ800);
  }

  //debug colors for activation

  //satus color
  
  
  private:

      Adafruit_NeoPixel * neopixels;
};

#endif

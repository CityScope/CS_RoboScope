public static short toRGB565(color col) {
  if (col == 0) {
    return 0;
  }
  int red   = col >> 16 & 0xFF;
  int green = col >> 8 & 0xFF;
  int blue  = col & 0xFF;
  short rgb = (short) (((red & 0xf8) << 8) | ((green & 0xfc) << 3) | (blue >> 3));

  // 0 is reserved for null -> set green (has highest resolution) to 1
  if (rgb == 0) {
    return 0x20;
  }
  // If the color actually was 0x20 then set it to 0x40 (to the nearest green)
  if (rgb == 0x20) {
    return 0x40;
  }
  return rgb;
}

/*

 
 public static int convert32_16(int rgb) {
 
 char val = (((rgb&0xf80000)>>8) + ((rgb&0xfc00)>>5) + ((rgb&0xf8)>>3));
 return val;
 }
 
 
 //http://www.java2s.com/example/java-utility-method/color-to-rgb-string/torgb565-color-c-69a51.html
 //https://github.com/libgdx/libgdx/blob/master/gdx/src/com/badlogic/gdx/graphics/Color.java
 public static int rgb565 (int col) {
 return ((int)(col * 31) << 11) | ((int)(col.g * 63) << 5) | (int)(col.b * 31);
 }
 
/*
 int a = ((rgb & 0xF0000000) >>> 16);
 int r = ((rgb & 0x00F00000) >>  12);
 int g = ((rgb & 0x0000F000) >>   8);
 int b = ((rgb & 0x000000F0) >>   4);
 return (char)( a | r | g | b);
 
 return (short) (((rgb & 0xf8) >> 3) | ((rgb & 0xf800) >> 6) | ((rgb & 0xf80000) >> 9));
 */

public static int convert16_32(short rgb) {
  int a = ((rgb & 0xF000) << 16);
  int r = ((rgb & 0x0F00) << 12);
  int g = ((rgb & 0x00F0) <<  8);
  int b = ((rgb & 0x000F) <<  4);
  return (a | r | g | b);
}

public static byte [] covert16_8array(short val ) {
  ByteBuffer bb = ByteBuffer.allocate(2);
  bb.putShort(val);
  return bb.array();
}

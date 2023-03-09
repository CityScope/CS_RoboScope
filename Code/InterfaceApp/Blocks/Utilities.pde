static final char argb32_to_rgb16(color c) {
  color r = c>>023 & 0x1f;
  color g = c>>012 & 0x3f;
  color b = c>>3 & 0x1f;

  return (char) (r<<11 | g<<5 | b);
}

static final color rgb16_to_argb32(char c) {
  color r = c>>010 | 7;
  color g = c>>3 & 0xfc | 3;
  color b = c<<3 & 0xf8 | 7;

  return r<<020 | g<<010 | b | #000000;
}

static final char from_rgb16_array(byte... arr) {
  return (char) (arr[0]<<11 | arr[1]<<5 | arr[2]);
}

static final byte[] to_rgb16_array(char c) {
  return to_rgb16_array(c, null);
}

static final byte[] to_rgb16_array(char c, byte... arr) {
  byte r = (byte) (c>>11);
  byte g = (byte) (c>>5 & 0x3f);
  byte b = (byte) (c & 0x1f);

  if (arr == null || arr.length < 3)  return new byte[] {
    r, g, b
  };

  arr[0] = r;
  arr[1] = g;
  arr[2] = b;
  return arr;
}


//----------------------------------------------------------------
//Convert 2 bytes to a single 16bit number
char convertFrom8To16(int dataFirst, int dataSecond) {
    char dataBoth = 0x0000;
    dataBoth = dataFirst;
    dataBoth = dataBoth << 8;
    dataBoth |= dataSecond;
    return dataBoth;
}

//----------------------------------------------------------------
//Convert from 16 bit number to two single uint8_t
int [] convertFrom16To8(char dataAll) {
    int arrayData[] = { 0x00, 0x00 };
    arrayData[0] = (dataAll >> 8) & 0x00FF;
    arrayData[1] = dataAll & 0x00FF;
    return arrayData;
}


void testNumberConversion(){
  
  color coltest = color(0, 255, 255);
  char coltest16 = argb32_to_rgb16(coltest);
  int col16int[] = convertFrom16To8(coltest16);
  
  char col16 = convertFrom8To16(col16int[0], col16int[1]);
  
}

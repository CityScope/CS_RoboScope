char* bin(unsigned int k) {
  char* buffer = malloc(8 * sizeof(char)); /* any number higher than sizeof(unsigned int)*bits_per_byte(8) */
  itoa(k, buffer, 2);
  return buffer;
}

//----------------------------------------------------------------
//Convert 2 bytes to a single 16bit number
uint16_t convertFrom8To16(uint8_t dataFirst, uint8_t dataSecond) {
    uint16_t dataBoth = 0x0000;
    dataBoth = dataFirst;
    dataBoth = dataBoth << 8;
    dataBoth |= dataSecond;
    return dataBoth;
}

//----------------------------------------------------------------
//Convert from 16 bit number to two single uint8_t
uint8_t* convertFrom16To8(uint16_t dataAll) {
    static uint8_t arrayData[2] = { 0x00, 0x00 };
    *(arrayData) = (dataAll >> 8) & 0x00FF;
    arrayData[1] = dataAll & 0x00FF;
    return arrayData;
}

#define SERIAL_BAU 115200

char operation; // Holds operation (R, W, ...)
char endC; // end command
char mode; // Holds the mode (D, A)
int pin_number; // Holds the pin number
int digital_value; // Holds the digital value
int analog_value; // Holds the analog value
int value_to_write; // Holds the value that we want to write
int wait_for_transmission = 5; // Delay in ms in order to receive the serial data

int primary_timer;
int LED = 0;
long counter =0;

//format for received pixel data
struct Pixel {
  byte node;
  byte local_id;
  byte inter;
  byte height;
  byte color1;
  byte color2;
};

char* bin(unsigned int k) {
  char* buffer = malloc(8 * sizeof(char)); /* any number higher than sizeof(unsigned int)*bits_per_byte(8) */
  itoa(k, buffer, 2);
  return buffer;
}

//use this to send data to the table
void translate_pixels(Pixel buf) {
  char str[30];
  sprintf(str, "%d,%d,%s,%d,%X,%X", buf.node, buf.local_id, bin(buf.inter), buf.height, buf.color1, buf.color2);
  LED = !LED;
  digitalWrite(13, LED);
}

void send_pixels(Pixel buf[], int buf_size) {
  for (int i = 0; i < buf_size; i++) {
    char str[20];
    sprintf(str, "%d,%d,%s,%d,%X,%X;", buf[i].node, buf[i].local_id, bin(buf[i].inter), buf[i].height, buf[i].color1, buf[i].color2);
    Serial.print(str);
  }
  Serial.println(' ');
}

void setup() {
  Serial.begin(SERIAL_BAU); // Serial Port at 9600 baud
  pinMode(13, OUTPUT);
  pinMode(8, INPUT_PULLUP);
  primary_timer = millis();
}

void loop() {
  // Check if characters available in the buffer
  if (Serial.available() > 0) {
    operation = Serial.read();
    if (operation == 'W') {
      counter++;
      byte temp[5] = {0};
      for (int i = 0; i < 6; i++) {
        temp[i] = Serial.parseInt();
        Serial.read();
      }
      Pixel buf = {0, 0, 0, 0, 0, 0};
      buf = {temp[0], temp[1], temp[2], temp[3], temp[4], temp[5]};
      translate_pixels(buf);
    }
  }
}

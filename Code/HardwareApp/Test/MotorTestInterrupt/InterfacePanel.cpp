#include "InterfacePanel.h"

//---------------------------------------------------------------------------
// constructor
InterfacePanel::InterfacePanel(int pId) {

  Serial.println("Setting up LEDs");
  id = pId;


  colorTimer = new Timer(2000);

  //push down
  pushPin[0] = SX1509_01_SWITCH_LEDBOX;
  pushPin[1] = SX1509_02_SWITCH_LEDBOX;
  pushPin[2] = SX1509_03_SWITCH_LEDBOX;
  pushPin[3] = SX1509_04_SWITCH_LEDBOX;
  pushPin[4] = SX1509_05_SWITCH_LEDBOX;
  pushPin[5] = SX1509_06_SWITCH_LEDBOX;
  pushPin[6] = SX1509_07_SWITCH_LEDBOX;
  pushPin[7] = SX1509_08_SWITCH_LEDBOX;

  //limit switch
  limitSwithPin[0] = SX1509_01_SWITCH_STOP;
  limitSwithPin[1] = SX1509_02_SWITCH_STOP;
  limitSwithPin[2] = SX1509_03_SWITCH_STOP;
  limitSwithPin[3] = SX1509_04_SWITCH_STOP;
  limitSwithPin[4] = SX1509_05_SWITCH_STOP;
  limitSwithPin[5] = SX1509_06_SWITCH_STOP;
  limitSwithPin[6] = SX1509_07_SWITCH_STOP;
  limitSwithPin[7] = SX1509_08_SWITCH_STOP;

  for (int i = 0; i < MOTORS_PER_PANEL; i++) {
    limitSwithState[i] = false;
    limitSwithStatePrev[i] = false;
  }

  for (int i = 0; i < MOTORS_PER_PANEL; i++) {
    pushSwithState[i] = false;
    pushSwithStatePrev[i] = false;
  }

}

//---------------------------------------------------------------------------
int InterfacePanel::getUrbanPixelCount() {
  return NUM_URBAN_PIXELS;
}

//---------------------------------------------------------------------------
void InterfacePanel::init() {

  Serial.println("Setting up Mux:");

  int muxCounter = 0;
  while (muxCounter != 3) {
    if (!sx00.begin(SX1509_ADDRESS_00) )
    {
      Serial.println(SX1509_ADDRESS_00);
      Serial.println("Failed 00");
      muxCounter++;
    } else {
      Serial.println("Connected 00");
      break;
    }
  }

  //input with internal pullup resistor activated:
  for (int i = 0; i < MOTORS_PER_PANEL; i++) {
    sx00.pinMode(pushPin[i], INPUT_PULLUP);
    sx00.pinMode(limitSwithPin[i], INPUT_PULLUP);
  }

  // FALLING, RISING, or CHANGE. Set it to falling, which will
  // mean the button was pressed:
  for (int i = 0; i < MOTORS_PER_PANEL; i++) {
    sx00.enableInterrupt(pushPin[i], CHANGE  );
    sx00.enableInterrupt(limitSwithPin[i], CHANGE  );
  }

  // The SX1509 has built-in debounce features, so a single
  // button-press doesn't accidentally create multiple ints.
  // Use io.debounceTime(<time_ms>) to set the GLOBAL SX1509
  // debounce time.
  // <time_ms> can be either 0, 1, 2, 4, 8, 16, 32, or 64 ms.
  sx00.debounceTime(4);

  //debouncePin
  for (int i = 0; i < MOTORS_PER_PANEL; i++) {
    sx00.debouncePin(pushPin[i]);
    sx00.debouncePin(limitSwithPin[i]);
  }

  pinMode(INTERRUPT_PIN_SWITCH, INPUT_PULLUP);

  // Attach an Arduino interrupt to the interrupt pin. Call
  // the button function, whenever the pin goes from HIGH to
  // LOW.

  neopixels = new Adafruit_NeoPixel(NUM_URBAN_PIXELS, NEO_PIXEL_PIN, NEO_GRBW + NEO_KHZ800);
  neopixels->begin();
  neopixels->clear();

}

//---------------------------------------------------------------------------
void InterfacePanel::setColorAll(int r, int g, int b) {
  for (int i = 0; i < NUM_URBAN_PIXELS; i ++) {
    neopixels->setPixelColor(i, neopixels->Color(r, g, b));
  }
  neopixels->show();
}

//---------------------------------------------------------------------------
// Sets the color on a per urban pixel basis
void InterfacePanel::setColorUrbanPixel(int urbanPixelId, int r, int g, int b) {
  for (int i = 0; i < NUM_URBAN_PIXELS; i++) {
    neopixels->setPixelColor(i, neopixels->Color(r, g, b));
  }
  neopixels->show();
}


//---------------------------------------------------------------------------
// Sets the color on a sub urban pixel basis
// Each urban pixel may be made up of multiple neopixels, this function gives control over them individually
void InterfacePanel::setColorSubUrbanPixel(int urbanPixelId, int subPixel, int r, int g, int b) {

  neopixels->setPixelColor(0, neopixels->Color(r, g, b));
  neopixels->setPixelColor(1, neopixels->Color(r, g, b));
  neopixels->show();
}

bool InterfacePanel::getLimitSwitchState(int i) {
  if ( !limitSwithStatePrev[i] && limitSwithState[i])
    return true;
  return false;
}

bool InterfacePanel::getLimitState(int i) {
  if ( limitSwithStatePrev[i] && limitSwithState[i])
    return true;
  return false;
}

//---------------------------------------------------------------------------
void InterfacePanel::updateLimitState() {

  unsigned int intStatus = sx00.interruptSource();
  // For debugging handiness, print the intStatus variable.
  // Each bit in intStatus represents a single SX1509 IO.
  //Serial.println("intStatus = " + String(intStatus, BIN));

  // If the bit corresponding to our button IO generated
  // the input:
  Serial.print("limit ");
  for (int i = 1; i <= MOTORS_PER_PANEL; i++) {
    unsigned state =  intStatus & (1 << limitSwithPin[i - 1]);

    limitSwithStatePrev[i - 1] =  limitSwithState[i - 1];
    limitSwithState[i - 1] = state;

    Serial.print(bool(state));
    Serial.print("-");
    Serial.print(limitSwithState[i - 1]);
    Serial.print("-");
    Serial.print(limitSwithStatePrev[i - 1]);
    Serial.print(" ");
    /* if ( state) {
       Serial.print(i - 1);
       Serial.print(": ");
       Serial.println("Button pressed!"); // Print a message.
      }
    */
  }
  Serial.println();

  Serial.print("push ");
  for (int i = 1; i <= MOTORS_PER_PANEL; i++) {
    unsigned state =  intStatus & (1 << pushPin[i - 1]);

    pushSwithStatePrev[i - 1] =  pushSwithState[i - 1];
    pushSwithState[i - 1] = state;

    Serial.print(bool(state));
    Serial.print("-");
    Serial.print(pushSwithState[i - 1]);
    Serial.print("-");
    Serial.print(pushSwithStatePrev[i - 1]);
    Serial.print(" ");
    /* if ( state) {
       Serial.print(i - 1);
       Serial.print(": ");
       Serial.println("Button pressed!"); // Print a message.
      }
    */
  }
  Serial.println();
}

//---------------------------------------------------------------------------
bool InterfacePanel::getPushSwitchState(int i) {
  if ( !pushSwithStatePrev[i] && pushSwithState[i])
    return true;
  return false;
}

bool InterfacePanel::getPushState(int i) {
  if ( pushSwithStatePrev[i] && pushSwithState[i])
    return true;
  return false;
}

void InterfacePanel::updatePushState() {
  unsigned int intStatus = sx00.interruptSource();
  // For debugging handiness, print the intStatus variable.
  // Each bit in intStatus represents a single SX1509 IO.
  //Serial.println("intStatus = " + String(intStatus, BIN));

  // If the bit corresponding to our button IO generated
  // the input:

}

//---------------------------------------------------------------------------
int InterfacePanel::getPushCurrentState(int i) {
  return  sx00.digitalRead(pushPin[i]);
}


//---------------------------------------------------------------------------
int InterfacePanel::getLimitCurrentState(int i) {
  return  sx00.digitalRead(limitSwithPin[i]);
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t InterfacePanel::Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return  neopixels->Color(255 - WheelPos * 3, 0, WheelPos * 3, 0);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return  neopixels->Color(0, WheelPos * 3, 255 - WheelPos * 3, 0);
  }
  WheelPos -= 170;
  return neopixels->Color(WheelPos * 3, 255 - WheelPos * 3, 0, 0);
}

void InterfacePanel::rainbow(unsigned long time) {

  colorTimer->update(time);
  if (colorTimer->isDone()) {
    int randomColorR = random(255);
    int randomColorG = random(255);
    int randomColorB = random(255);
    for (int i = 0; i < neopixels->numPixels(); i++) {
      neopixels->setPixelColor(i, randomColorR, randomColorG, randomColorB, 0);
    }
    neopixels->show();


    colorTimer->reset();
  }
}

void InterfacePanel::changeRandColor() {
  int randomColorR = random(255);
  int randomColorG = random(255);
  int randomColorB = random(255);
  for (int i = 0; i < neopixels->numPixels(); i++) {
    neopixels->setPixelColor(i, randomColorR, randomColorG, randomColorB, 0);
  }
  neopixels->show();
}



void InterfacePanel::limitswitch() {
  //InterfacePanel::limitSwitchPressed = true;
}

#include <Scheduler.h>

int counter = 0;
int counter1 = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  delay(2000);
  
  Scheduler.startLoop(loop1);
}

void loop () {
 analogWrite(9, counter);
 counter++;
 if (counter > 255){
  counter = 0;
 }
 delay(33);
}

void loop1 () {
 if (Serial.available()) {
    char c = Serial.read();
    if (c=='0') {
      digitalWrite(13, LOW);
      Serial.println("Led turned off!");
    }
    if (c=='1') {
      digitalWrite(13,  HIGH);
      Serial.println("Led turned on!");
    }
  }
 yield();
}

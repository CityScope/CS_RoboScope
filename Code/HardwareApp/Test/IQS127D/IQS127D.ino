
int poximityPin  = 12;
int touchPin     = 11;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);

  pinMode(poximityPin, INPUT);
  pinMode(touchPin, INPUT);

  pinMode(13, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:

  //digitalWrite(13, LOW);
  
  if (digitalRead(poximityPin) == LOW) {
    //Serial.println("near");
  //  digitalWrite(13, HIGH);
  }

 
    Serial.println(analogRead(touchPin));

 

}

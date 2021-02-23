
int hallEffectPin = 11;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(13, OUTPUT);
  
  pinMode(hallEffectPin, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  int hallEffect = analogRead(hallEffectPin);
  int valDig  = digitalRead(hallEffectPin);

  Serial.print(hallEffect);
  Serial.print(" ");
  Serial.println(valDig);
  delay(50);


  if (hallEffect > 500) {
    digitalWrite(13, HIGH);
  } else {
    digitalWrite(13, LOW);
  }
}

const int photoRezPin = A0;
const int ledPin = 11;

void setup() {
  // put your setup code here, to run once:
  pinMode(ledPin, OUTPUT);
  pinMode(photoRezPin, INPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  int photoRezVal = analogRead(photoRezPin);
  
  int val = map(photoRezVal,0, 1023, 0, 255);

  
 
  analogWrite(ledPin, val);

}

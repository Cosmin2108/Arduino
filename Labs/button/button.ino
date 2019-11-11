const int buttonPin = 2;
const int ledPin = 3;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {

  int buttonValue = digitalRead(buttonPin);

  if(!buttonValue == 1){
     digitalWrite(ledPin, HIGH);
  }
  else{
     digitalWrite(ledPin, LOW);
  }
  

  Serial.println(buttonValue);
  
}

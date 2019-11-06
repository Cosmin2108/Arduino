const int pinLed = 13;
int currentTime = 0;
int interval = 1000;
int lastTime = 0;
int ledValue = LOW;

void setup() {
    pinMode(pinLed, OUTPUT);
    Serial.begin(9600);
}


void loop() {

  currentTime= millis();
  
  if(currentTime - lastTime > interval)
  {
    ledValue = !ledValue;
    lastTime = currentTime;
    digitalWrite(pinLed, ledValue);
    
  }
  
  Serial.print(currentTime);
  Serial.print("\n");

  
}

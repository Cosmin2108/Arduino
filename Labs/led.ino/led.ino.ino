const int *LED_PIN = new int(13);

void setup() {
  // put your setup code here, to run once:
  pinMode(13, OUTPUT);

}

unsigned int courTime = 0;
int lastTime = 0;
int interv = 800;
int value = LOW;

void loop() {
  
  courTime = millis();

  if(courTime - lastTime > interv)
    {
      value = !value;
      lastTime = courTime;
      digitalWrite(13, value);
    }

    
  
}

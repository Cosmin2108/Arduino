const int buttonPin = 2;
const int ledPin = 3;
int lastButtonValue = 0;
int state = 0;
int debounceDelay = 50;
int lastDebounceTime = 0;

void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {

  int buttonValue = digitalRead(buttonPin);

  if(buttonValue != lastButtonValue)
    lastDebounceTime = millis();

    
  if(buttonValue == 0 ){
    lastButtonValue = 0; 
  }


  
  if(buttonValue == 1){ 
    
    if(lastButtonValue == 0){
      state = !state;
    }
    lastButtonValue = 1; 
  }
  
  digitalWrite(ledPin, state);

  Serial.println(buttonValue);
  
}

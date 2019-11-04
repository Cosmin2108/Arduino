// Pins of the RGB LED.
const int redPin = 11;
const int greenPin = 10;
const int bluePin = 9;
const int soundPin = 7;

// Analog input from the potentiometer
const int potPinRed = A0;
const int potPinGreen = A1;
const int potPinBlue = A2;


// Values for each pin of the RGB LED.
int currentColorValueRed;
int currentColorValueGreen;
int currentColorValueBlue;

// Value used to make delay
int currentTime = 0;
int interval1 = 4000;
int lastTime1 = 0;
int count = 1;
const int noteDuration =2000;

void setup()
{
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(soundPin, OUTPUT);


  pinMode(potPinRed, INPUT);
  pinMode(potPinGreen, INPUT);
  pinMode(potPinBlue, INPUT);
}
 
void loop()
{
  
  int potPinRedValue = map(analogRead(potPinRed), 0, 1024, 0, 255);
  int potPinGreenValue = map(analogRead(potPinGreen), 0, 1024, 0, 255);
  int potPinBlueValue = map(analogRead(potPinBlue), 0, 1024, 0, 255);

   
  analogWrite(redPin, potPinRedValue);
  analogWrite(greenPin, potPinGreenValue);
  analogWrite(bluePin, potPinBlueValue);

   currentTime= millis();
  
  if(currentTime - lastTime1 > interval1 && count == 1)
  {
    noTone(soundPin);
    
    tone(soundPin, potPinRedValue, noteDuration);
    
    count =2;
    
    lastTime1 = currentTime;
        
  }
  
  if(currentTime - lastTime1 > interval1 && count == 2)
  {
    noTone(soundPin);
    
    tone(soundPin, potPinGreenValue, noteDuration);
    
    count =3;
    
    lastTime1 = currentTime;
        
  }
  
  if(currentTime - lastTime1 > interval1 && count == 3)
  {
    noTone(soundPin);
    
    tone(soundPin, potPinBlueValue, noteDuration);
    
    count =1;
    
    lastTime1 = currentTime;
        
  }
  
}

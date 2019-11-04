// Pins of the RGB LED.
const int redPin = 11;
const int greenPin = 10;
const int bluePin = 9;

// Analog input from the potentiometer
const int potPin = A0;

// Values for each pin of the RGB LED.
int currentColorValueRed;
int currentColorValueGreen;
int currentColorValueBlue;
 
void setup()
{
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
}
 
void loop()
{
  // Reading the value of the pot pin (it's always 0 - 1024) 
  // and converting it to a RGB range (0 - 255).
  int potPinValue = map(analogRead(potPin), 0, 1024, 0, 255);
  
  // Explanation of the code can be found on this link or on our blog:
  // https://electronics.stackexchange.com/questions/240185/controlling-color-and-brightness-of-rgb-led-strip-using-one-potentiometer
  // Note: 1/6th of 255 is 42.5
  
  if (potPinValue < 86) {
    currentColorValueRed = potPinValue;
    currentColorValueGreen = 0;
    currentColorValueBlue = 0;
   }
   
   if (potPinValue > 85 && potPinValue < 171) {
    currentColorValueRed = 0;
    currentColorValueGreen = potPinValue;
    currentColorValueBlue = 0;
   }
   
   if (potPinValue > 170) {
    currentColorValueRed = 0;
    currentColorValueGreen = 0;
    currentColorValueBlue = potPinValue;
   }
  
  if (potPinValue = 0) {
    currentColorValueRed = 0;
    currentColorValueGreen = 0;
    currentColorValueBlue = 0;
  }
   
  analogWrite(redPin, currentColorValueRed);
  analogWrite(bluePin, currentColorValueBlue);
  analogWrite(greenPin, currentColorValueGreen);
}

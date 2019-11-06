// Pins of the RGB LED.
const int redPin = 11;
const int greenPin = 10;
const int bluePin = 9;

// Analog input from the potentiometer
const int potPinRed = A0;
const int potPinGreen = A1;
const int potPinBlue = A2;


// Values for each pin of the RGB LED.
int currentColorValueRed;
int currentColorValueGreen;
int currentColorValueBlue;

void setup()
{
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

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

}

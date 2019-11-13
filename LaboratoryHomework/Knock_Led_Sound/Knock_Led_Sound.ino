#include "pitches.h"

// knock
const int knockPin = A0;
const int intensity = 50;
int knockValue = 60;
// LED
const int ledPin = 2;
int ledState = 0;

//melody
const int soundPin = 11;
unsigned long currentTime = 0;
int interval = 5000;
unsigned long lastTime = 0;
int soundVal = 500;
int musicOn = 0;
int ok = 0;

// notes in the melody:
int melody[] = {
  NOTE_E6, NOTE_DS6, 
  NOTE_E6, NOTE_DS6, NOTE_E6, NOTE_B5, NOTE_D6, NOTE_C6, 
  NOTE_A3, NOTE_FS4, NOTE_B4, NOTE_C5, NOTE_E5, NOTE_A5,
  NOTE_E3, NOTE_E4, NOTE_GS4, NOTE_E5, NOTE_GS5, NOTE_B5,
  NOTE_A3, NOTE_FS4, NOTE_B4, NOTE_E5, NOTE_E6, NOTE_DS6
};

// note durations: 250 = quarter note, 125 = eighth note, etc.:
int noteDurations[] = {
  EIGHTHNOTE, EIGHTHNOTE, 
  EIGHTHNOTE, EIGHTHNOTE, EIGHTHNOTE, EIGHTHNOTE, EIGHTHNOTE, EIGHTHNOTE, 
  EIGHTHNOTE, EIGHTHNOTE, EIGHTHNOTE, EIGHTHNOTE, EIGHTHNOTE, EIGHTHNOTE,
  EIGHTHNOTE, EIGHTHNOTE, EIGHTHNOTE, EIGHTHNOTE, EIGHTHNOTE, EIGHTHNOTE,
  EIGHTHNOTE, EIGHTHNOTE, EIGHTHNOTE, EIGHTHNOTE, EIGHTHNOTE, EIGHTHNOTE
};


int pause[] = {0};

int courentNote = 0;
unsigned long currentTimeSound = 0;
unsigned long lastTimeSound = 0;


// button
const int buttonPin = 3;
int buttonState = 0;
int lastButtonState = 0;
int deboundeDelay = 50;
unsigned long lastDebounceTime = 0;


void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(soundPin, OUTPUT);
  pinMode(knockPin, INPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:



  if(knockValue > intensity){
    ledState = !ledState;
    knockValue = 40;
    digitalWrite(ledPin, ledState);
    lastTime = millis();
    Serial.println("ma-ta");

  }

  currentTime = millis();
  currentTimeSound = millis();

  if(currentTime - lastTime > interval && !ok){
    Serial.println("Second");
    ok = 1;
    ledState = 0;
    digitalWrite(ledPin, ledState);
    musicOn = 1;
    lastTimeSound = currentTimeSound;
    tone(soundPin, melody[courentNote], noteDurations[courentNote]);  
  }

  ///simulate melody loop
  if(musicOn && currentTimeSound - lastTimeSound > 10*15*noteDurations[courentNote] + pause[0]){
    Serial.println("tone");

    courentNote += 1;
    if(courentNote == 20)
      courentNote = 0;
    lastTimeSound = currentTimeSound;
    tone(soundPin, melody[courentNote], 10*15*noteDurations[courentNote]);  
  }

  int reading = !digitalRead(buttonPin); //If the button is presed, then the value is 0,  so i'll make it 1 


  if(lastButtonState != reading){
    lastDebounceTime = millis();
  }

  if(millis() - lastDebounceTime > deboundeDelay){

    if(reading != buttonState){
      buttonState = reading;
      
      if(reading == 1){
        ledState = !ledState;
        musicOn = !musicOn;
        digitalWrite(ledPin, ledState);
       }
    }
  }
  
  lastButtonState = reading;

}

/// Joystick pins
const int xAxisPin = A0;
const int yAxisPin = A1;
const int pinSW = 0;

///digits pins
const int pinD1 = 2;
const int pinD2 = 3;
const int pinD3 = 4;
const int pinD4 = 5;

///segments pins
const int pinA = 6;
const int pinB = 7;
const int pinC = 8;
const int pinD = 9;
const int pinE = 10;
const int pinF = 11;
const int pinG = 12;
const int pinDP = 13;

const int segSize = 8;
const int noOfDisplays = 4;
const int noOfDigits = 10;


///Joystick button
int buttonState = 0;
int lastButtonState = 0;
int debounceDelay = 50;
unsigned long lastDebounceTime = 0;

bool swState = 0;
bool locked = 0;
bool lastLocked = 0;
int dpState = LOW;

int displayDP = 0;
bool dp_was_moved = 0;

/// Joystick axis
int xAxis;
int yAxis;
int minThreshold= 300;
int maxThreshold= 700;

// segments array, similar to before
int segments[segSize] = {
pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP
};
// digits array, to switch between them easily
int displays[noOfDisplays] = {
pinD1, pinD2, pinD3, pinD4
};

byte digitMatrix[noOfDigits + 1][segSize - 1] = {
// a b c d e f g
{1, 1, 1, 1, 1, 1, 0}, // 0
{0, 1, 1, 0, 0, 0, 0}, // 1
{1, 1, 0, 1, 1, 0, 1}, // 2
{1, 1, 1, 1, 0, 0, 1}, // 3
{0, 1, 1, 0, 0, 1, 1}, // 4
{1, 0, 1, 1, 0, 1, 1}, // 5
{1, 0, 1, 1, 1, 1, 1}, // 6
{1, 1, 1, 0, 0, 0, 0}, // 7
{1, 1, 1, 1, 1, 1, 1}, // 8
{1, 1, 1, 1, 0, 1, 1}, // 9
{0, 0, 0, 0, 0, 0, 0} // NULL
};


void drow_digit(byte digit, byte DP){
  for(int i = 0; i < segSize-1; i ++){
    digitalWrite(segments[i], digitMatrix[digit][i]);
  }
  digitalWrite(segments[segSize-1], DP);
}


void select_digit(byte display_digit){
  for(int i = 0; i < noOfDisplays; i ++){
    digitalWrite(displays[i], HIGH);
  }
  
   digitalWrite(displays[display_digit], LOW);
}

void setup() {
  for(int i = 0; i < segSize; i ++){
    pinMode(segments[i], OUTPUT);
  }

  for(int i = 0; i < noOfDisplays; i ++){
    pinMode(displays[i], OUTPUT);
  }

  pinMode(pinSW, INPUT_PULLUP );
  pinMode(xAxis, INPUT);
  pinMode(yAxis, INPUT);

  Serial.begin(9600);
}

///the number on the display
int currentNumber = 0;
long long int lastDrow = 0;
int interval = 3;  

///In the locked state
int digit_chosen;
long long int  lastChange = 0;
bool digit_updated = 0;


void loop() {

  /// Here I'm moving the dp if it is not locked and update currentNumber if it was changed
  if(!locked){
    
    if(!digit_updated && lastLocked == 1){
    
      int displayDP_aux= displayDP;
      int p = 1;
      
      while(displayDP_aux > 0){
        p = p * 10;
        displayDP_aux --;
      }
    
      currentNumber = ((currentNumber/(p*10)) * 10 + digit_chosen ) * p + currentNumber % p;  /// update currentNumber
      
      digit_updated = 1;
      digit_chosen = 0;
      Serial.println(currentNumber);
    }

      
    int number = currentNumber;
    int display_no = 0;

    int xAxis = analogRead(xAxisPin);
    
    if(xAxis < minThreshold && !dp_was_moved){
      displayDP --;
      dp_was_moved = 1;
    }
    
    if(xAxis > maxThreshold && !dp_was_moved){
      displayDP ++;
      dp_was_moved = 1;
    }

    
    if(displayDP > 3)
      displayDP = 0;

    if(displayDP < 0)
      displayDP = 3;
     
    
    if(xAxis >= minThreshold && xAxis <= maxThreshold)
      dp_was_moved = 0;


    /// printing number

    if(currentNumber == 0){
      int i = 0;
 
      while(i < noOfDisplays){
      
        if(millis() - lastDrow > interval){        
          select_digit(display_no);
          
          if(displayDP == display_no)     /// if current display is sellected, show DP
            drow_digit(0, HIGH);
          else
            drow_digit(0, LOW);
            
          display_no++;
          i++;
          lastDrow = millis();
          delay(1);
        }
      }
    }
    else{

      int i = 0;
     
      while(number != 0 && i < noOfDisplays){
        
        if(millis() - lastDrow > interval){
          int digit = number % 10;
        
          select_digit(display_no);
          
          if(displayDP == display_no)     /// if current display is sellected, show DP
            drow_digit(digit, HIGH);
          else
            drow_digit(digit, LOW);
            
          display_no ++;
          number = number / 10;
          lastDrow = millis();
          i++;
          delay(1);
          
        }
      }
      
      /// If currentNumber has less then 4 digits, print just DP on the rest of the display
      while(i < noOfDisplays){
         if(millis() - lastDrow > interval){
            select_digit(display_no);
            
            if(displayDP == display_no)
              drow_digit(10, HIGH);
            else
              drow_digit(10, LOW);
              
            display_no ++;
            lastDrow = millis();
            i++;
            delay(1);
         }
      }
    }
  }else{
    
    digit_updated = 0;
    int yAxis = analogRead(yAxisPin);
    
    if(yAxis < minThreshold && !dp_was_moved){         /// reuse dp_was_moved ~ chose_number has already been modified in this movement
      digit_chosen --;
      dp_was_moved = 1;
    }
    
    if(yAxis > maxThreshold && !dp_was_moved){
      digit_chosen ++;
      dp_was_moved = 1;
    }
    
    if(digit_chosen > 9)
      digit_chosen = 0;

    if(digit_chosen < 0)
      digit_chosen = 9;
     
    if(yAxis >= minThreshold && yAxis <= maxThreshold)
      dp_was_moved = 0;

    int number = currentNumber;
    int display_no = 0;
    
    if(currentNumber == 0){
      int i = 0;
 
      while(i < noOfDisplays){
      
        if(millis() - lastDrow > interval){        
          select_digit(display_no);
          
          if(displayDP == display_no)     /// if current display is sellected, show DP
            drow_digit(digit_chosen, dpState);
          else
            drow_digit(0, LOW);
            
          display_no++;
          i++;
          lastDrow = millis();
          delay(1);
        }
      }
    }
    else{

      int i = 0;
     
      while(number != 0 && i < noOfDisplays){
        
        if(millis() - lastDrow > interval){
          int digit = number % 10;
        
          select_digit(display_no);
          
          if(displayDP == display_no)     /// if current display is sellected, show DP
            drow_digit(digit_chosen, dpState);
          else
            drow_digit(digit, LOW);
            
          display_no ++;
          number = number / 10;
          lastDrow = millis();
          i++;
          delay(1);
          
        }
      }
    }
  
    
    select_digit(displayDP);    
    drow_digit(digit_chosen, dpState);

    if(millis() - lastChange > 500){
      dpState = !dpState;
      lastChange = millis();
    }
      
  }

   
  int swState = !digitalRead(pinSW); //If the button is presed, then the value is 0,  so I'll make it 1 

  if(lastButtonState != swState){
    lastDebounceTime = millis();
  }

  if(millis() - lastDebounceTime > debounceDelay){

    if(swState != buttonState){   ///if what i'm reading is different from buttonState
      buttonState = swState;
      
      if(swState == 1){       /// make changes if what I'm reading is 1
        lastLocked = locked;
        locked = !locked;
       }
    }
  }
  
  lastButtonState = swState;
  
}

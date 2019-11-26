#include <LiquidCrystal.h> // includes the LiquidCrystal Library
/// LCD Display pins
const int RS = 12;
const int enable = 11;
const int d4 = 5;
const int d5 = 4;
const int d6 = 3;
const int d7 = 2;
///Joystick pins
const int pinSW = 7;
const int xAxis = A0;
const int yAxis = A1;

LiquidCrystal lcd(RS, enable, d4, d5, d6, d7);


///Joystick axis
const int topLimit = 700;
const int bottomLimit = 300;
bool xmoved = 0;
bool ymoved = 0;
int xVal, yVal;

//Joystik button
int buttonState = 0;
int lastButtonState = 0;
int debounceDelay = 50;
unsigned long lastDebounceTime = 0;
bool swState = 0;

//Menu Start
int levels = 3;
int currentLevel = 1;
int lives = 3;
int score = currentLevel*3;

/// Menu setings
int startingLevel = 1;
char player_name[20];

int current_menu = 0;   /// max is 5. 0 - base menu, 1- highsc, 2- start, 3-setings, 4-set player_name, 5-set startlevel
bool visible[] = {0, 0, 0, 0};

struct Selected{
  int l, c;
};

struct Record{
  char player_name[20];
  int score;
};


Record record = {"Player", 0};

Selected selectedPos[] = { {0, 0}, {0, 0}, {0, 0}, {0, 0} };    /// first =  principal menu, second = Setings


void set_player_name(){
  strcpy(player_name, "Player");
  ///make something that set letters like previous homehork with numbers
  lcd.print("Set name");
  visible[current_menu] = 0; 
  current_menu = 3;
}

void set_start_level(){

  int locked = 1;
  lcd.print(" ");
  lcd.print(startingLevel);
  while(locked){
    yVal = analogRead(yAxis);
    
    if(!ymoved && yVal < bottomLimit){
      startingLevel--;
      ymoved = 1;
    }
  
    if(!ymoved && yVal > topLimit){
      startingLevel++;
      ymoved = 1;
    }

    if(startingLevel > 3){
      startingLevel =1;
    }

    if(startingLevel < 1){
      startingLevel = 3;
    }

    if(ymoved){
      lcd.setCursor(1, 0);
      lcd.print(startingLevel);
    }
    
    if(yVal > bottomLimit && yVal < topLimit)
      ymoved = 0;
    
    int swState = !digitalRead(pinSW); //If the button is presed, then the value is 0,  so I'll make it 1 
  
    if(lastButtonState != swState){
      lastDebounceTime = millis();
    }
  
    if(millis() - lastDebounceTime > debounceDelay){
    
      if(swState != buttonState){   ///if what i'm reading is different from buttonState
        buttonState = swState;
        
        if(swState == 1){       /// make changes if what I'm reading is 1
          locked = 0;
          visible[current_menu] = 0; 
          current_menu = 3;
        }
      }
    }
    lastButtonState = swState;
  }
}

void print_menu(){

  lcd.clear();

  if(current_menu == 0){
    
    lcd.setCursor((16 - strlen("HighScore"))/2, 0);
    lcd.print("HighScore");
    lcd.setCursor(1,1);
    lcd.print("Start");
    lcd.setCursor(strlen("Start") + 3, 1);
    lcd.print("Setings");
    visible[current_menu] = 1;
    lcd.setCursor(selectedPos[current_menu].c, selectedPos[current_menu].l);
    lcd.print(">");
    
  }
  else
  if(current_menu == 1){
     lcd.print(" ");
     lcd.print(record.player_name);
     lcd.print(" ");
     lcd.print(record.score);
     lcd.setCursor(0, 1);
     lcd.print(">Exit");
     selectedPos[current_menu].l = 1;
     selectedPos[current_menu].c = 0;
     visible[current_menu] = 1;
     
  }
   else
  if(current_menu == 2){
    
     lcd.print(" ");
     lcd.print("Score ");
     lcd.print(score);
     
     lcd.setCursor(0,1);
     lcd.print("Lives ");
     lcd.print(lives);

     lcd.setCursor(strlen("Lives ")+3,1);
     lcd.print("Level ");
     lcd.print(startingLevel);

     play_game();
     
  }
  else
  if(current_menu == 3){
    
     lcd.print(" Enter name ");
     lcd.setCursor(0,1);
     lcd.print(" Set_lvl:");
     lcd.print(startingLevel);
     lcd.print("  Exit");
     visible[current_menu] = 1;
     lcd.setCursor(selectedPos[current_menu].c, selectedPos[current_menu].l);
     lcd.print(">");

  }else
  if(current_menu == 4){

    set_player_name();
    
  }
  else{
    
    set_start_level();
    
  }
    
  
  
}

int gameStart;
int currentTime;
int gameDuration = 11000;


void play_game(){

  gameStart = millis();
  currentTime = millis();
  currentLevel = startingLevel;
  
  int i = 1;
  while(currentTime - gameStart < gameDuration){
    currentTime = millis();
    if(currentTime - gameStart > i*5000 && currentLevel < levels){
      i++;
      currentLevel ++;
      score = currentLevel * 3;

      lcd.setCursor(7, 0);
      lcd.print(score);
      lcd.setCursor(6, 1);
      lcd.print(lives);
      lcd.setCursor(15, 1);
      lcd.print(currentLevel);
    }
     
  }

  if(record.score < score){
    record.score = score;
    strcpy(record.player_name, player_name);
  }
  
  lcd.clear();
  lcd.setCursor(16, 0);
  lcd.autoscroll();
  char message[] = "Congratulations, you finished the game. Press button to exit";
  
  for(int j = 0; j< strlen(message); j++){
    lcd.print(message[j]);
    delay(250);
  }

//  for(int k = 0; k <= strlen(message)/16; k++){
//    lcd.clear();
//    lcd.setCursor(16, 0);
//    for(int j = k*16; j<(k+1)*16 && j< strlen(message); j++){
//      lcd.print(message[j]);
//      delay(250);
//    }
//  }
  
  lcd.noAutoscroll();
  
  
  int locked = 1;
  while(locked){
    int swState = !digitalRead(pinSW); //If the button is presed, then the value is 0,  so I'll make it 1 

    if(lastButtonState != swState){
      lastDebounceTime = millis();
    }
  
    if(millis() - lastDebounceTime > debounceDelay){
    
      if(swState != buttonState){   ///if what i'm reading is different from buttonState
        buttonState = swState;
        
        if(swState == 1){       /// make changes if what I'm reading is 1
          locked = 0;
          visible[current_menu] = 0; 
          current_menu = 0;
        }
      }
    }
    lastButtonState = swState;
  }
  score = startingLevel*3;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(pinSW, INPUT_PULLUP );
  pinMode(xAxis, INPUT);
  pinMode(yAxis, INPUT);
  
  lcd.begin(16, 2);

}


void loop() {

  if(!visible[current_menu]){
    print_menu();
  }
  
  xVal = analogRead(xAxis);
  
  if(!xmoved && (xVal < bottomLimit || xVal > topLimit)){

    if(current_menu == 0 && selectedPos[current_menu].l == 1){
      lcd.setCursor(selectedPos[current_menu].c, selectedPos[current_menu].l);
      lcd.print(" ");
      if(selectedPos[current_menu].c == 0){
        selectedPos[current_menu].c = 7;
        selectedPos[current_menu].l = 1;
      }
      else{
        selectedPos[current_menu].c = 0;
        selectedPos[current_menu].l = 1;
      }
      
      xmoved = 1;
    }else
    if(current_menu == 3 && selectedPos[current_menu].l == 1){
      lcd.setCursor(selectedPos[current_menu].c, selectedPos[current_menu].l);
      lcd.print(" ");
      if(selectedPos[current_menu].c == 0){
        selectedPos[current_menu].c = 11;
        selectedPos[current_menu].l = 1;
      }
      else{
        selectedPos[current_menu].c = 0;
        selectedPos[current_menu].l = 1;
      }

      xmoved = 1;
    }
  }

    
  yVal = analogRead(yAxis);
  if(!ymoved && (yVal < bottomLimit || yVal > topLimit)){
   
    if(current_menu == 0 || current_menu == 3){
        lcd.setCursor(selectedPos[current_menu].c, selectedPos[current_menu].l);
        lcd.print(" ");
        if( selectedPos[current_menu].l == 0){
          selectedPos[current_menu].c = 0;
          selectedPos[current_menu].l = 1;
        }
        else{
          selectedPos[current_menu].c = 0;
          selectedPos[current_menu].l = 0;
        }
        ymoved = 1;
    }
    
  }


  if(ymoved || xmoved){
    
    lcd.setCursor(selectedPos[current_menu].c, selectedPos[current_menu].l);
    lcd.print(">");
   
  }

  if(yVal > bottomLimit && yVal < topLimit)
    ymoved = 0;

  if(xVal > bottomLimit && xVal < topLimit)
    xmoved = 0;


  int swState = !digitalRead(pinSW); //If the button is presed, then the value is 0,  so I'll make it 1 

  if(lastButtonState != swState){
    lastDebounceTime = millis();
  }

  if(millis() - lastDebounceTime > debounceDelay){

    if(swState != buttonState){   ///if what i'm reading is different from buttonState
      buttonState = swState;
      
      if(swState == 1){       /// make changes if what I'm reading is 1
        
        if(current_menu == 0)
          if(selectedPos[current_menu].l == 1 && selectedPos[current_menu].c == 0)
            {
              visible[current_menu] = 0;
              current_menu = 2;     ///start     
            }
          else
          if(selectedPos[current_menu].l == 0 && selectedPos[current_menu].c == 0)
            {
              visible[current_menu] = 0;
              current_menu = 1;     ///hightscore    
            }
          else
          if(selectedPos[current_menu].l == 1 && selectedPos[current_menu].c == 7)
            {
              visible[current_menu] = 0;
              
              current_menu = 3;     ///Setings   
            }
            

        if(current_menu == 1 && visible[current_menu] ){
          visible[current_menu] = 0;  
          current_menu = 0;     /// principal
           
        }

        if(current_menu == 3  && visible[current_menu] ){
          visible[current_menu] = 0;  

          if(selectedPos[current_menu].l == 0){
            current_menu = 4;   /// set name
          }else
          if(selectedPos[current_menu].l == 1 && selectedPos[current_menu].c == 11){
             current_menu = 0;
          }
          else{
            current_menu = 5;   /// set startlevel
          }
            
        }

        if((current_menu == 4 || current_menu == 5)  && visible[current_menu] ){
          visible[current_menu] = 0;  
          current_menu = 3;     /// principal 
        }
        
        Serial.println(current_menu);
        
      }
    }
  }
  
  lastButtonState = swState;
  
}

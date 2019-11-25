#include <LiquidCrystal.h> // includes the LiquidCrystal Library
/// LCD Display
const int RS = 12;
const int enable = 11;
const int d4 = 5;
const int d5 = 4;
const int d6 = 3;
const int d7 = 2;
///Joystick
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

//Menu
int levels = 3;
int lives = 3;
int highScore = 0;
int startingLevel = 0;
char name[20];
char ** menu;

int menu_printed = 0;   /// max is 3. 0 - base menu, 1- highsc, 2- start, 3-name & startlvl
bool visible[] = {0, 0, 0, 0};

struct Selected{
  int l, c;
};

Selected selectedPos[] = { {0, 0}, {0, 0}, {0, 0}, {0, 0}};    /// first =  principal menu, second = high score list/menu, third = settings menu


void print_menu(){

  lcd.clear();
  lcd.print(">");

  if(menu_printed == 0){
    lcd.setCursor((16 - strlen(menu[0]))/2, 0);
    lcd.print(menu[0]);
    lcd.setCursor(1,1);
    lcd.print(menu[1]);
    lcd.setCursor(strlen(menu[1]) + 3, 1);
    lcd.print(menu[2]);
  }
  else{
     lcd.print("Menu: ");
     lcd.print(menu_printed);
  }

  visible[menu_printed] = 1;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(pinSW, INPUT_PULLUP );
  pinMode(xAxis, INPUT);
  pinMode(yAxis, INPUT);
  
  lcd.begin(16, 2);

  /// trebuie sa mai adaug si restul meniurilor ca sa le pot printa in fct print_menu
  menu = new char*[3];
  for(int i=0; i<3; i++){
    menu[i] = new char[16];
  }
  strcpy(menu[0], "HighScore");
  strcpy(menu[1], "Start");
  strcpy(menu[2], "Settings");

}


void loop() {

  if(!visible[menu_printed]){
    print_menu();
  }
  
  xVal = analogRead(xAxis);
  
  /// trebuie sa pun conditii de miscare a sagetii pt fiecare meniu
  if(!xmoved && (xVal < bottomLimit || xVal > topLimit) && selectedPos[0].l == 1){

    lcd.setCursor(selectedPos[0].c, selectedPos[0].l);
    lcd.print(" ");
    
    if(menu_printed == 0){
      if(selectedPos[0].c == 0){
        selectedPos[0].c = 7;
        selectedPos[0].l = 1;
      }
      else{
        selectedPos[0].c = 0;
        selectedPos[0].l = 1;
      }
    }
    else
    if(menu_printed == 1)
    {
      ///move in another way
    }
    
    xmoved = 1;
  }

    
  yVal = analogRead(yAxis);
  if(!ymoved && (yVal < bottomLimit || yVal > topLimit) && selectedPos[0].l == 0){
    lcd.setCursor(selectedPos[0].c, selectedPos[0].l);
    lcd.print(" ");
    
    if( selectedPos[0].l == 0){
      selectedPos[0].c = 0;
      selectedPos[0].l = 1;
    }
    else{
      selectedPos[0].c = 0;
      selectedPos[0].l = 0;
    }
    
    ymoved = 1;
  }


  if(ymoved || xmoved){
    
    lcd.setCursor(selectedPos[0].c, selectedPos[0].l);
      
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
          if(selectedPos[menu_printed].l == 1 && selectedPos[menu_printed].c == 0)
            {
              visible[menu_printed] = 0;
              menu_printed = 1;          
            }
        Serial.println(menu_printed);
       }
    }
  }
  
  lastButtonState = swState;
  
}

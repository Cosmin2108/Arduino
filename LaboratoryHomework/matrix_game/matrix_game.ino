 #include <LiquidCrystal.h> // includes the LiquidCrystal Library
#include <EEPROM.h>
#include "game.h"

/// LCD Display pins
const int RS = 8;
const int enable = 13;
const int d4 = 5;
const int d5 = 4;
const int d6 = 3;
const int d7 = 2;
///Joystick pins
const int pinSW = 7;
const int xAxis = A0;
const int yAxis = A1;
const int VO = 9;

LiquidCrystal lcd(RS, enable, d4, d5, d6, d7);


///Joystick axis
const int topLimit = 700;
const int bottomLimit = 300;
bool xmoved = 0;
bool ymoved = 0;
short int xVal, yVal;

//Joystik button
bool buttonState = 0;
bool lastButtonState = 0;
short int debounceDelay = 50;
unsigned long lastDebounceTime = 0;
bool swState = 0;

///Suport for navigate and print menu
short int current_menu = 0;   /// max is 5. 0 - base menu, 1- highsc, 2- start, 3-setings, 4-set player_name, 5-set startlevel
bool visible[] = {0, 0, 0, 0, 0, 0, 0, 0};

struct Selected{
  short int l, c;
};

Selected selectedPos[] = { {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}};    /// 0 =  principal menu, 1= high score, 2 = start,
/// 3 = settings, 4 = Set Name, 5= Set Level, 6 = Game over, 7 = Pause

short int contrast = 360;

void check_btn(void (*f)()){

  int swState = !digitalRead(pinSW); //If the button is presed, then the value is 0,  so I'll make it 1 

  if(lastButtonState != swState){
    lastDebounceTime = millis();
  }

  if(millis() - lastDebounceTime > debounceDelay){

    if(swState != buttonState){   ///if what i'm reading is different from buttonState
      buttonState = swState;
      
      if(swState == 1){       /// make changes if what I'm reading is 1
        (*f)();
      }
    }
  }
  
  lastButtonState = swState;

}

void exit_set_player_name(){
  locked = 0;
  if(strcmp(name_aux, "          ") == 0)
    strcpy(player_name, "Player");
  else
    strcpy(player_name, name_aux);
    
  visible[current_menu] = 0; 
  current_menu = 3;
  lcd.noCursor();
  free(name_aux);
  Serial.println(player_name);
}

void set_player_name(){
  lcd.cursor();

  name_aux = (char*)malloc(11*sizeof(char));
  poz_in_name = 0;
  
  for(int i=0; i<10; i++)
    name_aux[i] = ' ';
    
  name_aux[10] = '\0';
  
  char chr = 96;   /// a-1
  locked = 1;
  int ok = 0;
  
  while(locked){

    yVal = analogRead(yAxis);
    
    if(!ymoved && yVal < bottomLimit){
      chr--;
      ok = 1;
      ymoved = 1;
    }
  
    if(!ymoved && yVal > topLimit){
      chr++;
      ok = 1;
      ymoved = 1;
    }

    if(chr > 'z'){
      chr = 'a';
    }

    if(chr < 'a'){
      chr = 'z';
    }

    if(ymoved){
      lcd.setCursor(poz_in_name, 0);
      lcd.print(chr);
      lcd.setCursor(poz_in_name, 0);
    }
    
    if(yVal > bottomLimit && yVal < topLimit){
      if(ok)
        name_aux[poz_in_name] = (char)chr;
      else
        if(name_aux[poz_in_name] == ' ')
          name_aux[poz_in_name] = ' ';
        
      Serial.println(name_aux);
      ymoved = 0;

    }
    
    xVal = analogRead(xAxis);
    
    if(!xmoved && xVal < bottomLimit){
      poz_in_name--;
      ok = 0;
      xmoved = 1;
    }
  
    if(!xmoved && xVal > topLimit){
      poz_in_name++;
      ok = 0;
      xmoved = 1;
    }

    if(poz_in_name > 9){
      poz_in_name = 9;
    }

    if(poz_in_name < 0){
      poz_in_name = 0;
    }

    if(xmoved){
      lcd.setCursor(poz_in_name, 0);
      chr = 96;   /// a-1
      
    }
    
    if(xVal > bottomLimit && xVal < topLimit){
      xmoved = 0;
    }

    check_btn(exit_set_player_name);
      
  }
}


void exit_start_lvl(){
  locked = 0;
  visible[current_menu] = 0; 
  current_menu = 3;
}


void set_start_level(){

  locked = 1;
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

    /// check btn with fct for exit from set_start_lvl as parameter
    check_btn(exit_start_lvl);

  }
}

void print_menu(){

  lcd.clear();
  lcd.setCursor(0, 0);
  Serial.println(current_menu);
  if(current_menu == 0){
    
    lcd.setCursor((16 - strlen("HighScore"))/2, 0);
    lcd.print("HighScore");
    lcd.setCursor(1,1);
    lcd.print("Start");
    lcd.setCursor(strlen("Start") + 3, 1);
    lcd.print("Settings");
    visible[current_menu] = 1;
    lcd.setCursor(selectedPos[current_menu].c, selectedPos[current_menu].l);
    lcd.print(">");
    
  }else
  if(current_menu == 1){
     selectedPos[current_menu].c = 0;
     selectedPos[current_menu].l = 1;
     lcd.print(" ");
     lcd.print(record[0].player_name);
     lcd.print(" ");
     lcd.print(record[0].score);
     lcd.setCursor(0, 1);
     lcd.print(" Reset");
     lcd.print("  Exit");
     lcd.setCursor(selectedPos[current_menu].c, selectedPos[current_menu].l);
     lcd.print(">");
     visible[current_menu] = 1;
     
  }else
  if(current_menu == 2){
         lcd.print(" Score ");
     lcd.print(score);
     
     lcd.setCursor(0,1);
     lcd.print("Lives ");
     lcd.print(lives);

     lcd.setCursor(strlen("Lives ")+3,1);
     lcd.print("Level ");
     lcd.print(startingLevel);
     visible[current_menu] = 1; 
          
  }else
  if(current_menu == 3){
    
     lcd.print(" Name:");
     lcd.print(player_name);
     lcd.setCursor(0,1);
     lcd.print(" Level:");
     lcd.print(startingLevel);
     lcd.print("  Exit");
     visible[current_menu] = 1;
     lcd.setCursor(selectedPos[current_menu].c, selectedPos[current_menu].l);
     lcd.print(">");

  }else
  if(current_menu == 4){
    set_player_name();
  }else
  if(current_menu == 5){
    
    set_start_level();
    
  }else
  if(current_menu == 6){    
    selectedPos[current_menu].c = 0;
    selectedPos[current_menu].l = 1;
    lcd.setCursor(2, 0);
    lcd.print("Game over!");
    lcd.setCursor(1, 1);
    lcd.print("Exit");
    lcd.setCursor(7, 1);
    lcd.print("Try Again");
    lcd.setCursor(selectedPos[current_menu].c, selectedPos[current_menu].l);
    lcd.print(">");
    visible[current_menu] = 1;
  }else
  if(current_menu == 7){
    selectedPos[current_menu].c = 0;
    selectedPos[current_menu].l = 1;
    lcd.setCursor(1, 0);
    lcd.print("Exit & Save");
    lcd.setCursor(1, 1);
    lcd.print("Replay");
    lcd.setCursor(9, 1);
    lcd.print("Reset");
    visible[current_menu] = 1;
    lcd.setCursor(selectedPos[current_menu].c, selectedPos[current_menu].l);
    lcd.print(">");
    visible[current_menu] = 1;
    
  }
    
}

void stop_game(){
  locked = 0;
  last_road_scroll = 0;
  score = 0;
  game_speed = 800;
  lc.clearDisplay(0);// clear screen
}

void reset_game(){
  pause = !pause;
  last_road_scroll = 0;
  score = 0;
  game_speed = 800;
  currentLevel = startingLevel;
  currentLives = lives;
  printed = 1;
  lastLevel = currentLevel;
  lastLives = lives;
  offset = road_length;
  last_road_scroll = 0;
  lc.clearDisplay(0);// clear screen
}

void play_real_game(){

  currentLevel = startingLevel;
  currentLives = lives;
  hit_car;
  printed = 1;
  lastLevel = currentLevel;
  lastLives = lives;
  
  while(currentLives != 0 && currentLevel <= levels){

    if(millis() - last_road_scroll > game_speed){
      printed = print_road();
      last_road_scroll = millis();
    }
    
    if(printed){
      hit_car = 0;
      xVal = analogRead(xAxis);
      
      if(!xmoved && xVal < bottomLimit){
        hit_car = move_car(-1);
        xmoved = 1;
      }
      
      if(!xmoved && xVal > topLimit){
        hit_car = move_car(1);
        xmoved = 1;
      }

      if(xVal > bottomLimit && xVal < topLimit)
        xmoved = 0;
      
      
      if(hit_car){
        currentLives --;
        lcd.setCursor(6, 1);
        lcd.print(currentLives);
        offset = road_length;
        last_road_scroll = 0;
      }

      print_car();

      lcd.setCursor(7, 0);
      lcd.print(score);
      
    }else{
       currentLives--;
       lcd.setCursor(6, 1);
       lcd.print(currentLives);
       offset = road_length;
       printed = print_road();
       print_car();
       last_road_scroll = 0;
    }

    lcd.setCursor(15, 1);
    lcd.print(currentLevel);

    if(currentLevel != lastLevel && currentLevel <= levels){
      if(lastLives == currentLives){
        currentLives++;
        lcd.setCursor(6, 1);
        lcd.print(currentLives);
        lastLives = currentLives;
      }
      else
        lastLives = currentLives;
        
      lastLevel = currentLevel;
    }

    /// pause the game 
    check_btn(pause_game); 
    
    if(pause){
      visible[current_menu] = 0; 
      current_menu = 7;     ///Pause
      
      if(!visible[current_menu])
        print_menu(); 
    }
    
    while(pause){
      xVal = analogRead(xAxis);
      yVal = analogRead(yAxis);
      menu_navigate();
      check_btn(change_menu);
    }
  }
  
  check_score();
  
  visible[current_menu] = 0; 
  current_menu = 6;     ///Game over
  
  if(!visible[current_menu])
    print_menu();  

  
  print_over();
  
  locked = 1;
  while(locked){

    xVal = analogRead(xAxis);
    yVal = analogRead(yAxis);
    menu_navigate();
    check_btn(change_menu);
    
  }
}


void change_menu(){

  if(current_menu == 0)
    if(selectedPos[current_menu].l == 1 && selectedPos[current_menu].c == 0)
      {
        visible[current_menu] = 0;
        current_menu = 2;     ///start  
        inGame = 1;   
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

    if(selectedPos[current_menu].l == 1 && selectedPos[current_menu].c == 0){
      for(short int i = 0; i<3; i++){
        strcpy(record[i].player_name, "Player    ");
        record[i].score = 0;
      }
      for (short int i = 0 ; i < EEPROM.length() ; i++) {
        EEPROM.write(i, 0);
      }
      print_menu();
    }else{
      visible[current_menu] = 0;  
      current_menu = 0;     /// principal
    }
  }

  if(current_menu == 3  && visible[current_menu] ){
    visible[current_menu] = 0;  

    if(selectedPos[current_menu].l == 0){
      current_menu = 4;   /// set name
    }else
    if(selectedPos[current_menu].l == 1 && selectedPos[current_menu].c == 9){
       current_menu = 0;
    }
    else{
      current_menu = 5;   /// set startlevel
    }
      
  }

  if((current_menu == 4 || current_menu == 5)  && visible[current_menu] ){
    visible[current_menu] = 0;  
    current_menu = 3;     /// settings
  }

  if(current_menu == 6 && visible[current_menu]){
    if(selectedPos[current_menu].l == 1 && selectedPos[current_menu].c == 0){
      visible[current_menu] = 0;  
      current_menu = 0;     /// principal
      inGame = 0;
    }else{
      visible[current_menu] = 0;
      current_menu = 2;     ///start  
      inGame = 1;
    }
    stop_game();
  }

  if(current_menu == 7 && visible[current_menu]){
    if(selectedPos[current_menu].l == 1 && selectedPos[current_menu].c == 0){
      visible[current_menu] = 0;
      current_menu = 2;     ///start
      print_menu(); 
      pause_game();
    }else
    if(selectedPos[current_menu].l == 1 && selectedPos[current_menu].c == 8){
      visible[current_menu] = 0;
      current_menu = 2;     ///start  
      reset_game();
      print_menu();
    }
    else{
      
      /// Save & Exit
    }
  }
  
}

void menu_navigate(){
  
  /// X Axis
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
        selectedPos[current_menu].c = 9;
        selectedPos[current_menu].l = 1;
      }
      else{
        selectedPos[current_menu].c = 0;
        selectedPos[current_menu].l = 1;
      }

      xmoved = 1;
    }else
    if(current_menu == 1 && selectedPos[current_menu].l == 1){
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
    if(current_menu == 6){
      lcd.setCursor(selectedPos[current_menu].c, selectedPos[current_menu].l);
      lcd.print(" ");
      if(selectedPos[current_menu].c == 0){
        selectedPos[current_menu].c = 6;
        selectedPos[current_menu].l = 1;
      }else{
        selectedPos[current_menu].c = 0;
        selectedPos[current_menu].l = 1;
      }
      xmoved = 1;
    }else
    if(current_menu == 7){
      lcd.setCursor(selectedPos[current_menu].c, selectedPos[current_menu].l);
      lcd.print(" ");
      if(selectedPos[current_menu].c == 0){
        selectedPos[current_menu].c = 8;
        selectedPos[current_menu].l = 1;
      }else{
        selectedPos[current_menu].c = 0;
        selectedPos[current_menu].l = 1;
      }
      xmoved = 1;
    }
  }

  /// Y Axis
  if(!ymoved && (yVal < bottomLimit || yVal > topLimit)){
   
    if(current_menu == 0 || current_menu == 3){
        lcd.setCursor(selectedPos[current_menu].c, selectedPos[current_menu].l);
        lcd.print(" ");                                                            ///delete last position
        if( selectedPos[current_menu].l == 0){
          selectedPos[current_menu].c = 0;
          selectedPos[current_menu].l = 1;
        }
        else{
          selectedPos[current_menu].c = 0;
          selectedPos[current_menu].l = 0;
        }
        ymoved = 1;
    }else
    if(current_menu == 1){

      if(yVal < bottomLimit){
        printedHighScore --;
        if(printedHighScore < 0)
          printedHighScore = 0;
        ymoved = 1;
      }else{
        printedHighScore ++;
        if(printedHighScore > 2)
          printedHighScore = 2;
        ymoved = 1;
      }
     lcd.setCursor(0,0);
     lcd.print(" ");
     lcd.print(record[printedHighScore].player_name);
     lcd.print(" ");
     lcd.print(record[printedHighScore].score);
     lcd.print(" ");
     lcd.print(" ");
     lcd.print(" ");
      /// scroll trought highscoresS
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

}


void setup() {
  // put your setup code here, to run once:

   // the zero refers to the MAX7219 number, it is zero for 1 chip
  lc.shutdown(0, false); // turn off power saving, enables display
  lc.setIntensity(0, 6); // sets brightness (0~15 possible values)
  lc.clearDisplay(0);// clear screen
  
  Serial.begin(9600);
  pinMode(pinSW, INPUT_PULLUP );
  pinMode(xAxis, INPUT);
  pinMode(yAxis, INPUT);

  lcd.begin(16, 2);
  analogWrite(VO, contrast);
  union Data
  {
    unsigned int a;
    unsigned char s[4];
  };

  union Data object;
  int adress = 0;
  for(int j=0; j<3; j++){
    
    for(int i=0; i<4; i++){
      object.s[i] = EEPROM.read(adress);
      adress++;
    }

    adress++; /// i have spaces between variables
    
    record[j].score = object.a;
    
    for(int i=5; i<16; i++){
      record[j].player_name[i-5] = (char)EEPROM.read(adress);
      adress++;
    }

  
    if(strlen(record[j].player_name) < 1)
      strcpy(record[j].player_name, "Player    ");
    Serial.print(record[j].player_name);
    Serial.println(" DA");
  }
  adress++; /// i have spaces between variables
}

void loop() {

  if(!visible[current_menu]){
    print_menu();
  }
  
  xVal = analogRead(xAxis);
  yVal = analogRead(yAxis);

  if(inGame){
    play_real_game();
  }
  
  if(!inGame){
    menu_navigate();
    check_btn(change_menu);
  }
  
}

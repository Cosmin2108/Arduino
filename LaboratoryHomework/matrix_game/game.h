#include "LedControl.h" //  need the library
LedControl lc = LedControl(12, 11, 10, 1); //DIN, CLK, LOAD, No. DRIVER

// pin 12 is connected to the MAX7219 pin 1
// pin 11 is connected to the CLK pin 13
// pin 10 is connected to LOAD pin 12
// 1 as we are only using 1 MAX7219

short int quit = 0;
const int buzz = 6;

/// For setings menu
short int startingLevel = 1; /// INITIAL

///For start display
const int levels = 3;
short int currentLevel = startingLevel;
const int lives = 3;
short int currentLives = lives;
short int score = 0;

struct Car{
  short int left_coord = 3;
  short int car_length = 4;
  short int car_width = 3;
  bool car_State = 1;
  unsigned long long int lastPrintCar = 0;
  short int interval_print_car = 70;
  bool car[8][8]= {
                  {0, 0, 0, 0, 0, 0, 0, 0},
                  {0, 0, 0, 0, 0, 0, 0, 0},
                  {0, 0, 0, 0, 0, 0, 0, 0},
                  {0, 0, 0, 0, 1, 0, 0, 0},
                  {0, 0, 0, 1, 1, 1, 0, 0},
                  {0, 0, 0, 0, 1, 0, 0, 0},
                  {0, 0, 0, 1, 1, 1, 0, 0},
                  {0, 0, 0, 0, 0, 0, 0, 0}
                 };
                 
  bool aux[8][8]= {
                  {0, 0, 0, 0, 0, 0, 0, 0},
                  {0, 0, 0, 0, 0, 0, 0, 0},
                  {0, 0, 0, 0, 0, 0, 0, 0},
                  {0, 0, 0, 0, 1, 0, 0, 0},
                  {0, 0, 0, 1, 1, 1, 0, 0},
                  {0, 0, 0, 0, 1, 0, 0, 0},
                  {0, 0, 0, 1, 1, 1, 0, 0},
                  {0, 0, 0, 0, 0, 0, 0, 0}
                 };

}car;


/// Score record
struct Record{
  char player_name[11];
  short int score;
}record[3];

short int printedHighScore = 0;

/// Menu setings
char player_name[11] = "Player    ";
char *name_aux;
short int poz_in_name;

long long int last_road_scroll = 0;
float game_speed = 800/currentLevel;
short int repeat = 2;
bool pause = 0;
bool hit_car;
bool printed = 1;
short int lastLevel = currentLevel;
short int lastLives = lives;
bool inGame = 0;

/// For the game exit loop, set name loop and set lvl loop
bool locked;
const short int road_length = 48;
 
bool road[road_length][8]= {
                  {1, 0, 0, 0, 0, 0, 0, 1},
                  {1, 1, 0, 0, 0, 0, 0, 1},
                  {1, 0, 0, 0, 0, 0, 0, 1},
                  {1, 0, 0, 0, 0, 0, 0, 1},
                  {1, 0, 0, 0, 0, 0, 1, 1},
                  {1, 0, 1, 0, 0, 0, 0, 1},
                  {1, 1, 0, 0, 0, 0, 1, 1},
                  {1, 0, 0, 0, 0, 0, 0, 1},
                  {1, 0, 0, 0, 0, 0, 0, 1},
                  {1, 1, 0, 0, 0, 0, 0, 1},
                  {1, 0, 0, 0, 0, 0, 1, 1},
                  {1, 0, 0, 0, 0, 0, 0, 1},
                  {1, 1, 0, 0, 0, 1, 0, 1},
                  {1, 0, 0, 0, 0, 0, 1, 1},
                  {1, 0, 0, 0, 0, 0, 0, 1},
                  {1, 0, 0, 0, 0, 0, 0, 1},
                  {1, 1, 1, 0, 0, 0, 0, 1},
                  {1, 0, 0, 0, 0, 0, 0, 1},
                  {1, 1, 0, 0, 0, 0, 0, 1},
                  {1, 0, 1, 1, 0, 0, 0, 1},
                  {1, 0, 0, 0, 0, 0, 0, 1},
                  {1, 0, 0, 0, 0, 0, 0, 1},
                  {1, 0, 0, 0, 0, 0, 0, 1},
                  {1, 0, 0, 0, 0, 0, 0, 1},
                  {1, 0, 0, 0, 0, 0, 0, 1},
                  {1, 0, 0, 0, 0, 0, 0, 1},
                  {1, 0, 0, 0, 0, 1, 1, 1},
                  {1, 0, 0, 0, 0, 0, 0, 1},
                  {1, 0, 0, 0, 0, 0, 0, 1},
                  {1, 0, 0, 0, 0, 0, 0, 1},
                  {1, 0, 0, 0, 0, 0, 0, 1},
                  {1, 1, 1, 1, 0, 0, 0, 1},
                  {1, 0, 0, 0, 0, 0, 0, 1},
                  {1, 1, 0, 0, 0, 0, 0, 1},
                  {1, 0, 0, 0, 0, 0, 0, 1},
                  {1, 0, 0, 0, 0, 0, 0, 1},
                  {1, 0, 0, 0, 0, 0, 1, 1},
                  {1, 0, 1, 0, 0, 0, 0, 1},
                  {1, 1, 0, 0, 0, 0, 1, 1},
                  {1, 0, 0, 0, 0, 0, 0, 1},
                  {1, 0, 0, 0, 0, 0, 0, 1},
                  {1, 0, 0, 0, 0, 0, 0, 1},
                  {1, 0, 0, 0, 0, 0, 0, 1},
                  {1, 0, 0, 0, 0, 0, 0, 1},
                  {1, 0, 0, 0, 0, 0, 0, 1},
                  {1, 0, 0, 0, 0, 0, 0, 1},
                  {1, 0, 0, 0, 0, 0, 0, 1},
                  {1, 0, 0, 0, 0, 0, 0, 1}
                 };  

short int offset = road_length;

bool colision(){
  
  bool hit = 0; /// hit car
  int k = offset - 1;
  for(int i = 6; i >= 3 && !hit; i--){
    for(int j = car.left_coord; j< car.left_coord + car.car_width; j++){
        if(road[k][j] == 1 && road[k][j] == car.car[i][j])
          hit = 1;
    }

    k--;
  }

  if(hit){
    tone(buzz, 2000, 150);
  }

  return hit;

}


void print_car(){

  for(int i = 0; i < 8; i++){
    for(int j = 0; j< 8; j++){
      if(car.car[i][j])
        lc.setLed(0, i, j, car.car_State * car.car[i][j]);
    }
  }
}

void delete_prev_car(){
  for(int i = 0; i < 8; i++){
    for(int j = 0; j< 8; j++){
      if(car.car[i][j])
        lc.setLed(0, i, j, LOW);
    }
  }
}

void reset_car_pos(){
    for(int i = 0; i < 8; i++)
      for(int j = 0; j< 8; j++)
        car.car[i][j] = car.aux[i][j];
    car.left_coord = 3;
}


bool move_car(int dir){
  car.lastPrintCar = 0;
  delete_prev_car();
  
  int i, j;
  if(dir == -1 && car.left_coord - 1 <= 0 ){
    tone(buzz, 2000, 150);
    delete_prev_car();
    reset_car_pos();
    return 1;
  }
  else
  if(dir == 1 && (car.left_coord + car.car_width -1 ) + 1 >= 7 ){
    tone(buzz, 2000, 150);
    delete_prev_car();
    reset_car_pos();
    return 1;
  }
  
  if(dir == -1){ /// stg

    for( i = 3; i< 3 + car.car_length; i++){
      for( j= car.left_coord - 1; j < car.left_coord + car.car_width - 1; j++){
        car.car[i][j] = car.car[i][j + 1];
      }
      car.car[i][j] = 0;
    }
    
    car.left_coord --;
  }else{

    for( i = 3; i< 3 + car.car_length; i++){
      for( j= car.left_coord + car.car_width; j >= car.left_coord  + 1; j--){
        car.car[i][j] = car.car[i][j - 1];
      }
      car.car[i][j] = 0;
    }

    car.left_coord ++;
  }

  if(colision()){
    delete_prev_car();
    reset_car_pos();
    return 1;  /// hit
  }

  return 0;
}


bool print_road(){

  offset--;
  
  if(offset == 6 && repeat != 0){    /// the beginning and the end of the road are identical to simulate a loop that is repeated  'repeat' time
    repeat --;
    offset = road_length - 1 - 9; 
  }

  if(repeat == 0){
    repeat = 3;
    currentLevel++;
    /// increase speed
    game_speed = 750/currentLevel;
    offset = road_length - 1; 
  }

  if(colision()){
    delete_prev_car();
    reset_car_pos();
    return 0;  /// not printed
  }

  score++;
  
  int k = offset;
  for(int i = 7; i >= 0 && k >= offset - 7; i--){
    for(int j = 0; j< 8; j++){
      lc.setLed(0, i, j, road[k][j]);
    }

    k--;
  }

  return 1; ///printed
}

void pause_game(){
  pause = !pause;
}


void print_over(){
  lc.clearDisplay(0);
  lc.setLed(0, 1, 2, 1);
  lc.setLed(0, 2, 2, 1);

  lc.setLed(0, 1, 5, 1);
  lc.setLed(0, 2, 5, 1);

  for(int i = 1; i < 7; i++)
    lc.setLed(0, 5, i, 1);

}

short int find_low_score(){
  short int poz;
  if(record[0].score == record[1].score && record[1].score == record[2].score  && record[2].score == 0)
    return 0;
    
  if(record[1].score == record[2].score  && record[2].score == 0)
    return 1;
  
  if(score <= record[1].score)
    poz =  2;
  else
  if(score <= record[0].score)
    poz =  1;
  else
    poz =  0;
  Serial.print("Poz: ");
  Serial.println(poz);
  
  for(short int i = 2; i>poz; i-- ){
    record[i].score = record[i-1].score;
    strcpy(record[i].player_name, record[i-1].player_name);
  }

  return poz;
}

void check_score(){

    if(record[2].score <= score){

      short int poz = find_low_score();   /// low score that can be replaced with new score
      
      record[poz].score = score;
      strcpy(record[poz].player_name, player_name);
  
      union Data
      {
        unsigned int a;
        unsigned char s[4];
      };
    
      union Data object;
      object.a = score;
      
      short int adress;
      
      if(poz == 0)
        adress = 0;
      else
      if(poz == 1)
        adress = 17;
      else
        adress = 34;
  
      if(adress == 34){
        for(int i=0; i<4; i++){  /// write score at adress 
          EEPROM.write(adress, object.s[i]);
          adress++;
        }
        adress ++;
        for(int i=5; i<16; i++){      /// write name of the player
          EEPROM.write(adress, record[poz].player_name[i-5]);
          adress++;
        }
      }else{
       for(int j=0; j<3; j++){
        
          object.a = record[j].score;
          
          for(int i=0; i<4; i++){
            EEPROM.write(adress, object.s[i]);
            adress++;
          }
      
          adress++; /// i have spaces between variables
                    
          for(int i=5; i<16; i++){
            EEPROM.write(adress, record[j].player_name[i-5]);
            adress++;
          }
          
          adress++; /// i have spaces between variables
      }
    }
    
  }
}

void stop_game(){
  locked = 0;
  last_road_scroll = 0;
  score = 0;
  game_speed = 800;
  car.lastPrintCar = 0;
  lc.clearDisplay(0);// clear screen
}

void reset_game(){
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
  delete_prev_car();
  reset_car_pos();
  car.lastPrintCar = 0;
  lc.clearDisplay(0);// clear screen
}

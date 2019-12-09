#include "LedControl.h" //  need the library
LedControl lc = LedControl(12, 11, 10, 1); //DIN, CLK, LOAD, No. DRIVER


// pin 12 is connected to the MAX7219 pin 1
// pin 11 is connected to the CLK pin 13
// pin 10 is connected to LOAD pin 12
// 1 as we are only using 1 MAX7219


/// Start
int levels = 3;
int currentLevel = 1;
int lives = 3;
int currentLives = lives;
int score = 0;

/// For setings
int startingLevel = 1; /// INITIAL

struct Car{
  int left_coord = 3;
  int car_length = 4;
  int car_width = 3;
  
  int car[8][8]= {
                  {0, 0, 0, 0, 0, 0, 0, 0},
                  {0, 0, 0, 0, 0, 0, 0, 0},
                  {0, 0, 0, 0, 1, 0, 0, 0},
                  {0, 0, 0, 1, 1, 1, 0, 0},
                  {0, 0, 0, 0, 1, 0, 0, 0},
                  {0, 0, 0, 1, 1, 1, 0, 0},
                  {0, 0, 0, 0, 0, 0, 0, 0},
                  {0, 0, 0, 0, 0, 0, 0, 0} 
                  
                  };
  int aux[8][8]= {
                  {0, 0, 0, 0, 0, 0, 0, 0},
                  {0, 0, 0, 0, 0, 0, 0, 0},
                  {0, 0, 0, 0, 1, 0, 0, 0},
                  {0, 0, 0, 1, 1, 1, 0, 0},
                  {0, 0, 0, 0, 1, 0, 0, 0},
                  {0, 0, 0, 1, 1, 1, 0, 0},
                  {0, 0, 0, 0, 0, 0, 0, 0},
                  {0, 0, 0, 0, 0, 0, 0, 0} 
                  
                  };

}car;

long long int last_road_scroll = 0;
int game_speed = 800;

const int road_length = 25;
 
int road[road_length][8]= {
                  {1, 0, 1, 0, 1, 0, 1, 0},
                  {0, 1, 0, 1, 0, 1, 0, 1},
                  {1, 0, 0, 0, 0, 0, 0, 1},
                  {1, 1, 0, 0, 0, 0, 1, 1},
                  {1, 0, 0, 0, 0, 0, 0, 1},
                  {1, 0, 0, 0, 0, 0, 0, 1},
                  {1, 0, 0, 0, 0, 0, 1, 1},
                  {1, 0, 0, 0, 0, 0, 0, 1},
                  {1, 1, 0, 0, 0, 0, 1, 1},
                  {1, 0, 0, 0, 0, 0, 0, 1},
                  {1, 1, 0, 0, 0, 0, 0, 1},
                  {1, 0, 0, 0, 0, 0, 1, 1},
                  {1, 0, 0, 0, 0, 0, 0, 1},
                  {1, 1, 0, 0, 0, 0, 0, 1},
                  {1, 0, 0, 0, 0, 0, 1, 1},
                  {1, 0, 0, 0, 0, 0, 0, 1},
                  {1, 0, 0, 0, 0, 0, 0, 1},
                  {1, 1, 0, 0, 0, 0, 0, 1},
                  {1, 0, 0, 0, 0, 0, 0, 1},
                  {1, 0, 0, 0, 0, 0, 0, 1},
                  {1, 0, 0, 0, 0, 0, 0, 1},
                  {1, 0, 0, 0, 0, 0, 0, 1},
                  {1, 0, 0, 0, 0, 0, 0, 1},
                  {1, 0, 0, 0, 0, 0, 0, 1},
                  {1, 0, 0, 0, 0, 0, 0, 1}
                                          };  

int offset = road_length;

bool colision(){
  
  bool hit = 0; /// hit car
  int k = offset - 2;
  for(int i = 5; i >= 2 && !hit; i--){
    for(int j = car.left_coord; j< car.left_coord + car.car_width; j++){
        if(road[k][j] == 1 && road[k][j] == car.car[i][j])
          hit = 1;
    }

    k--;
  }

  return hit;

}


void print_car(){

  for(int i = 0; i < 8; i++){
    for(int j = 0; j< 8; j++){
      if(car.car[i][j])
        lc.setLed(0, i, j, HIGH);
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

  delete_prev_car();
  
  int i, j;
  if(dir == -1 && car.left_coord - 1 <= 0 ){
    return 1;
  }
  else
  if(dir == 1 && (car.left_coord + car.car_width -1 ) + 1 >= 7 ){
    return 1;
  }
  
  if(dir == -1){ /// stg

    for( i = 2; i< 2 + car.car_length; i++){
      for( j= car.left_coord - 1; j < car.left_coord + car.car_width - 1; j++){
        car.car[i][j] = car.car[i][j + 1];
      }
      car.car[i][j] = 0;
    }
    
    car.left_coord --;
  }else{

    for( i = 2; i< 2 + car.car_length; i++){
      for( j= car.left_coord + car.car_width; j >= car.left_coord  + 1; j--){
        car.car[i][j] = car.car[i][j - 1];
      }
      car.car[i][j] = 0;
    }

    car.left_coord ++;
  }

  if(colision()){
    reset_car_pos();
    return 1;  /// hit
  }

  return 0;
}


bool print_road(){

  offset--;
  
  if(offset == 6){
    currentLevel++;
    /// increase speed
    game_speed = game_speed - (game_speed/10)*4;
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

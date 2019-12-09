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

int offset = road_length - 1;

bool colision(){
  
  bool hit = 0; /// hit car
  int k = offset - 2;
  for(int i = 5; i >= 2 && k > offset - car.car_length  && !hit; i--){
    for(int j = car.left_coord; j< car.left_coord + car.car_width; j++){
        if(road[k][j] == 1 && road[k][j] == car.car[i][j])
          hit = 1;
    }

    k--;
  }

  return hit;

}

bool print_road(){

  if(colision()){
    Serial.println("Hit");
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

  offset--;
  if(offset == 6){
    currentLevel++;
    /// increase speed
    game_speed = game_speed - game_speed/10;
    offset = road_length - 1; 
  }
  
  return 1; ///printed
}


void print_car(){

  for(int i = 0; i < 8; i++){
    for(int j = 0; j< 8; j++){
      if(car.car[i][j])
        lc.setLed(0, i, j, HIGH);
    }
  }
  
}


bool move_car(int dir){
  int i, j;
//Serial.print("Coord left: ");
//Serial.println(car.left_coord);
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
    return 1;  /// hit
  }

  return 0;
  
}

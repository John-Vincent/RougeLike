#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "room.h"


/**
 * generates a random rock hardness value for each position in the map.
 * @param  map                  the matrix representing the game area
 * @author Collin Vincent <collinvincent96@gmail.com>
 * @date   2017-01-24T22:46:08+000
 */
void generateHardness(int map[mapHeight][mapWidth]);

/**
 * initalizes the random number generator base on user input, this is basically just to keep main clean.
 * @param  argc                    argument count from command line
 * @param  argv                    command line arguments
 * @author Collin Vincent <collinvincent96@gmail.com>
 * @date   2017-01-24T23:04:05+000
 */
void initRandom(int argc, char const *argv[]);

/**
 * prints the map to the stdout
 * @param  map                     matrix representing objects in the map
 * @author Collin Vincent <collinvincent96@gmail.com>
 * @date   2017-01-25T16:44:49+000
 */
void printGrid(char map[mapHeight][mapWidth]);

/**
 * sets all the characters to a space
 * @param  map_char                  the map in character form
 * @author Collin Vincent <collinvincent96@gmail.com>
 * @date   2017-01-25T18:28:25+000
 */
void init_map_char(char map_char[mapHeight][mapWidth]);


int main(int argc, char const *argv[]) {

  //initalize random number generator and map matrix
  initRandom(argc, argv);

  //array of map hardness
  int map_hard[mapHeight][mapWidth];
  //array of characters for map
  char map_char[mapHeight][mapWidth];
  //array of rooms
  room_t *rooms;

  init_map_char(map_char);

  //assigns hardness value to all areas in the map
  generateHardness(map_hard);

  //generates all the rooms
  int numRooms = rand()%6 + 10;
  if(!(rooms = malloc((sizeof (*rooms)) * numRooms))){
      return 1;
  }
  if(generateRooms(rooms, numRooms)){
    printf("could not generate Rooms");
    return 1;
  }
  //set the hardness of map array to 0 to create paths between rooms
  connect_rooms(map_hard, rooms, numRooms);
  drawMap(map_hard, map_char, rooms, numRooms);

  printGrid(map_char);


  return 0;
}

void initRandom(int argc, char const *argv[]){
  int t;
  if(argc >= 2){
    t = atoi(argv[1]);
  }
  else{
    t = time(NULL);
  }
  printf("time seed: %d\n", t);
  srand(t);
}


void generateHardness(int map[][mapWidth]){
  int x, y;

  for(y = 0; y < mapHeight; y++){
    for(x = 0; x < mapWidth; x++){
      if(x==0 && y==0 && x==mapWidth-1 && y==mapHeight){
        map[y][x] = 255;
      }
      else{
        map[y][x] = rand() % 253 + 1;
      }
    }
  }
}

void init_map_char(char map_char[mapHeight][mapWidth]){
  int i, j;

  for(j = 0; j < mapHeight; j++){
    for(i = 0; i < mapWidth; i++){
      map_char[j][i] = ' ';
    }
  }
}


void printGrid(char map[mapHeight][mapWidth]){
  int j, i;

  for(j = 0; j < mapHeight; j++){
    for(i = 0; i < mapWidth; i++){
      printf("%c", map[j][i]);
    }
    printf("%d\n", j);
  }

}

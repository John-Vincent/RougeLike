#include "../headers/main.h"




int main(int argc, char const *argv[]) {

  int *seed = NULL;
  //reads the args sets the mode and gives the
  game_mode_t mode = readArgs(argc, argv, &seed);
  //initalize random number generator and map matrix
  initRandom(seed);

  dungeon_t dungeon;

  if(mode == GENERATE){

    if(generateMap(dungeon.hardness, dungeon.chars, &dungeon.rooms, &dungeon.numRooms)){
      printf("failed to generate Map\n");
      return -1;
    }

  } else if(mode == SAVE){

    if(save(dungeon.hardness, dungeon.chars, dungeon.rooms, dungeon.numRooms)){
      return -1;
    }

  } else if(mode == LOAD){

    if(load(dungeon.hardness, dungeon.chars, &dungeon.rooms, &dungeon.numRooms)){
      return -1;
    }

  } else if(mode == LOAD_SAVE){

    if(load_save(dungeon.hardness, dungeon.chars, &dungeon.rooms, &dungeon.numRooms)){
      return -1;
    }

  }


  return 0;
}

void initRandom(int const *seed){
  int t;
  if(seed == NULL){
    t = time(NULL);
  } else{
    t = *seed;
  }
  printf("time seed: %d\n", t);
  srand(t);
}

game_mode_t readArgs(int argc, char const *argv[], int **seed){
  int i, t;
  game_mode_t mode = GENERATE;

  for(i = 1; i < argc; i++){
    if(strcmp(argv[i], "-t")==0){
      i++;
      t = atoi(argv[i]);
      *seed = &t;
    } else if(strcmp(argv[i], "--save") == 0){
      if(mode == LOAD){
        mode = LOAD_SAVE;
      } else {
        mode = SAVE;
      }
    } else if(strcmp(argv[i], "--load") == 0){
      if(mode == SAVE){
        mode = LOAD_SAVE;
      } else {
        mode = LOAD;
      }
    }
  }

  return mode;
}

int save(uint8_t map_hard[mapHeight][mapWidth], char map_char[mapHeight][mapWidth], room_t *rooms, int room_count){

  if(generateMap(map_hard, map_char, &rooms, &room_count)){
    printf("failed to generate Map\n");
    return -1;
  }

  if(saveMap(room_count, rooms, map_hard)){
    printf("failed to save map\n");
    return -1;
  }

  printf("map saved\n");

  return 0;
}


int load(uint8_t map_hard[mapHeight][mapWidth], char map_char[mapHeight][mapWidth], room_t **rooms, int *room_count){

  if(loadMap(rooms, room_count,  map_hard)){
    return -1;
  }

  init_map_char(map_char);
  drawMap(map_hard, map_char, *rooms, *room_count);
  printMap(map_char);

  return 0;
}


int load_save(uint8_t map_hard[mapHeight][mapWidth], char map_char[mapHeight][mapWidth], room_t **rooms, int *room_count){

  if(loadMap(rooms, room_count,  map_hard)){
    return -1;
  }

  init_map_char(map_char);
  drawMap(map_hard, map_char, *rooms, *room_count);
  printMap(map_char);

  if(saveMap(*room_count, *rooms, map_hard)){
    printf("failed to save map\n");
    return -1;
  }

  printf("map saved\n");

  return 0;
}

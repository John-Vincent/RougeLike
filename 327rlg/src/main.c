#include "../headers/main.h"



int main(int argc, char const *argv[]) {

  dungeon_t *dungeon;

  int *seed = NULL;

  int nummon = default_monster_num;
  //reads the args sets the mode and gives the
  game_mode_t mode;

  dungeon = calloc(1, sizeof (dungeon_t));

  if(readArgs(argc, argv, &mode, &seed, &nummon)){
    return -1;
  }
  //initalize random number generator and map matrix
  initRandom(seed);



  if(mode == GENERATE){

    if(generateMap(dungeon->hardness, dungeon->chars, &(dungeon->rooms), &(dungeon->numRooms))){
      printf("failed to generate Map\n");
      return -1;
    }

  } else if(mode == SAVE){

    if(save(dungeon->hardness, dungeon->chars, &(dungeon->rooms), &(dungeon->numRooms))){
      return -1;
    }

  } else if(mode == LOAD){

    if(load(dungeon->hardness, dungeon->chars, &(dungeon->rooms), &(dungeon->numRooms))){
      return -1;
    }

  } else if(mode == LOAD_SAVE){

    if(load_save(dungeon->hardness, dungeon->chars, &(dungeon->rooms), &(dungeon->numRooms))){
      return -1;
    }

  }

  dungeon->num_characters = 0;

  if(generate_characters(dungeon, nummon)){
    return -1;
  }

  /*printMap(dungeon.chars);
  if(find_distances(dungeon.hardness, dungeon.distances, player.x, player.y)){
    return -1;
  }
  printDistances(dungeon.distances, dungeon.hardness, player.x, player.y, player.sym);
  */

 run_game(dungeon);

 free_mem(dungeon);

 return 0;
}




void initRandom(int *seed){
  int t;
  if(seed == NULL){
    t = time(NULL);
  } else{
    t = *seed;
  }
  printf("time seed: %d\n", t);
  srand(t);
  free(seed);
}


uint32_t readArgs(int argc, char const *argv[], game_mode_t *mode, int **seed, int *nummon){
  int i;
  *mode = GENERATE;

  for(i = 1; i < argc; i++){
    if(strcmp(argv[i], "-t")==0){
      i++;
      *seed = malloc(sizeof (int));
      **seed = atoi(argv[i]);
    } else if(strcmp(argv[i], "--save") == 0){
      if(*mode == LOAD){
        *mode = LOAD_SAVE;
      } else {
        *mode = SAVE;
      }
    } else if(strcmp(argv[i], "--load") == 0){
      if(*mode == SAVE){
        *mode = LOAD_SAVE;
      } else {
        *mode = LOAD;
      }
    } else if(!strcmp(argv[i], "--nummon")){
      i++;
      *nummon = atoi(argv[i]);
    } else{
      printf("invalid argument: %s\n", argv[i]);
      return -1;
    }
  }

  return 0;
}






int save(uint8_t map_hard[mapHeight][mapWidth], char map_char[mapHeight][mapWidth], room_t **rooms, uint8_t *room_count){

  if(generateMap(map_hard, map_char, rooms, room_count)){
    printf("failed to generate Map\n");
    return -1;
  }

  if(saveMap(*room_count, *rooms, map_hard)){
    printf("failed to save map\n");
    return -1;
  }

  printf("map saved\n");

  return 0;
}





int load(uint8_t map_hard[mapHeight][mapWidth], char map_char[mapHeight][mapWidth], room_t **rooms, uint8_t *room_count){

  if(loadMap(rooms, room_count,  map_hard)){
    return -1;
  }

  init_map_char(map_char);
  drawMap(map_hard, map_char, *rooms, *room_count);

  return 0;
}






int load_save(uint8_t map_hard[mapHeight][mapWidth], char map_char[mapHeight][mapWidth], room_t **rooms, uint8_t *room_count){

  if(loadMap(rooms, room_count,  map_hard)){
    return -1;
  }

  init_map_char(map_char);
  drawMap(map_hard, map_char, *rooms, *room_count);

  if(saveMap(*room_count, *rooms, map_hard)){
    printf("failed to save map\n");
    return -1;
  }

  printf("map saved\n");

  return 0;
}

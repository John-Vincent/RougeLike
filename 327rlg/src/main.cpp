#include <iostream>
#include "../headers/main.h"
#include "../headers/mon_file_parser"
#include "../headers/item_file_parser"



int main(int argc, char const *argv[]) {

  Dungeon *dungeon;

  int seed = 0;

  int nummon = default_monster_num;
  //reads the args sets the mode and gives the
  game_mode_t mode;

  if(readArgs(argc, argv, &mode, &seed, &nummon)){
    return -1;
  }
  //initalize random number generator and map matrix
  initRandom(seed);

  if(mode == GENERATE || mode == SAVE){

    try{
      dungeon = Dungeon::get_instance(nummon);
      dungeon->fill_level(0);
    }catch(char const* c){
      std::cout << c << std::endl;
      exit(-1);
    }

  } else if(mode == LOAD || mode == LOAD_SAVE){

    if(loadGame()){
      return -1;
    }

  }

 run_game();

 if(mode == SAVE || mode == LOAD_SAVE){
   if(saveGame()){
     return -1;
   }
 }

 delete (Dungeon::get_instance());

  return 0;
}




void initRandom(int seed){
  int t;
  if(!seed){
    t = time(NULL);
  } else{
    t = seed;
  }
  printf("time seed: %d\n", t);
  srand(t);
}


uint32_t readArgs(int argc, char const *argv[], game_mode_t *mode, int *seed, int *nummon){
  int i;
  *mode = GENERATE;

  for(i = 1; i < argc; i++){
    if(strcmp(argv[i], "-t")==0){
      i++;
      *seed = atoi(argv[i]);
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

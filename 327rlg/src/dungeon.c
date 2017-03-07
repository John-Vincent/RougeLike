#include <stdlib.h>
#include <stdio.h>
#include "../headers/dungeon.h"
#include "../headers/heap.h"



void free_mem(dungeon_t *dungeon){
    if(dungeon->downstairs){
      free_mem(dungeon->downstairs);
    }
    clear_heap(dungeon->turn_order);
    free(dungeon->player);
    free(dungeon->rooms);
    free(dungeon);
}


int init_dungeon(dungeon_t **d, int nummon){
  int i;

  *d = calloc(1, sizeof(dungeon_t));
  dungeon_t *dungeon = *d;

  if(!dungeon){
    printf("error allocating downstairs");
    return -1;
  }
  if(generateMap(dungeon->hardness, dungeon->chars, &(dungeon->rooms), &(dungeon->numRooms))){
    printf("error making new map");
    return -1;
  }
  dungeon->num_characters = 0;
  if(generate_characters(dungeon, nummon)){
    printf("error placing new characters");
    return -1;
  }


  init_heap(&(dungeon->turn_order), compare_characters);

  for(i = 0; i < dungeon->num_characters; i++){
      (dungeon->turn_order)->insert((dungeon->turn_order), dungeon->player + i);
  }
  dungeon->x = dungeon->player->x;
  dungeon->y = dungeon->player->y;
  return 0;
}

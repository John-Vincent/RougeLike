#include "../headers/des.h"




int run_game(dungeon_t *dungeon){
  int i;
  character_t *c;
  int turn = 0;
  heap_t *h;

  init_heap(&h, compare_characters);
  for(i = 0; i < dungeon->num_characters; i++){
    h->insert(h, dungeon->player + i);
  }

  display(dungeon);
  usleep(1000000);

  while(1){
    c = (character_t *)h->peek(h);
    if(c->attrib != 0xFFFFFFFF){
      if(!h->top){
        printf("you win!");
        break;
      }
      if(c->next_turn <= turn){
        c = (character_t *)h->pop(h);
        if(take_turn(dungeon, c)){
          display(dungeon);
          printf("you died!");
          break;
        }
        if(c == dungeon->player){
          printDistances(dungeon->distances, dungeon->hardness, dungeon->player->x, dungeon->player->y, dungeon->player->sym);
          printDistances(dungeon->distances_notun, dungeon->hardness, dungeon->player->x, dungeon->player->y, dungeon->player->sym);
          display(dungeon);
          usleep(1000000);
        }
        c->next_turn = c->next_turn + turnbias / c->speed;
        h->insert(h, c);
      }
    }else{
      h->pop(h);
    }
    turn++;
  }
  return 0;
}





void display(const dungeon_t *dungeon){
  int i, j;

  for(j = 0; j<mapHeight; j++){
    for(i = 0; i<mapWidth; i++){
      if(dungeon->characters[j][i]){
        printf("%c", dungeon->characters[j][i]->sym);
      } else{
        printf("%c", dungeon->chars[j][i]);
      }
    }
    printf("\n");
  }
}

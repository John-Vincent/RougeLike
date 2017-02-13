#include "../headers/character.h"


const struct{
  char *type;
  char symbol;
} character_lookup[] = {
  {"player", '@'},
  {NULL, ' '}
};

int make_character(character_t *c, const room_t *rooms, uint8_t numRooms, char *type){
  uint8_t t, i;

  t = rand()%numRooms;

  c->x = rand() % ((rooms + t)->width) + ((rooms + t)->xPos);
  c->y = rand() % ((rooms + t)->height) + ((rooms + t)->yPos);

  i = 0;

  while(1){
    if(!character_lookup[i].type){
      return -1;
    }
    if(!strcmp(type, character_lookup[i].type)){
      c->sym = character_lookup[i].symbol;
      return 0;
    }
    i++;
  }

}

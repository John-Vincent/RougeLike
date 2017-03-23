#include "../headers/character_cpp.h"

int character::compare(const character &other) const{
    int dif = this->next_turn - other.next_turn;
    if(dif != 0){
      return dif;
    }
    return this->get_gen() - other.get_gen();
}


int generate_characters(dungeon_t *dungeon, int num_monsters){
  unsigned int i;

  dungeon->characters = (character**) calloc( num_monsters + 1, sizeof (void*));
  if(!dungeon->characters){
    return 1;
  }
  dungeon->num_characters = num_monsters + 1;

  dungeon->characters[0] = new pc(dungeon);

  for(i = 1; i < dungeon->num_characters; i++){
    dungeon->characters[i] = new npc(dungeon, i);
  }

  dungeon->x = dungeon->characters[0]->x;
  dungeon->y = dungeon->characters[0]->y;

  return 0;
}

int compare_characters(void *child, void *parent){
  character *one;
  character *two;
  one = (character *) child;
  two = (character *) parent;
  return two->compare(*one);
}

int take_turn(character *c, int input){
  return c->take_turn(input);
}

int on_stairs(dungeon_t *dungeon){
  int x, y;

  x = dungeon->characters[0]->x;
  y = dungeon->characters[0]->y;

  if(dungeon->chars[y][x] == '<')
    return 1;
  else if (dungeon->chars[y][x] == '>')
    return -1;

  return 0;
}

int is_dead(character *c){
  return (c->attrib == 0xffffffff);
}

char get_char(dungeon_t *dungeon, int x, int y){
  unsigned int i;
  if( x >= (dungeon->characters[0]->x - torch_distance) && x <= (dungeon->characters[0]->x + torch_distance) &&
       y >= (dungeon->characters[0]->y - torch_distance) && y <= (dungeon->characters[0]->y + torch_distance) ){
    for(i = 0; i < dungeon->num_characters; i++){
      if(dungeon->characters[i]->x == x && dungeon->characters[i]->y == y && dungeon->characters[i]->attrib != 0xffffffff){
        return dungeon->characters[i]->sym;
      }
    }
  }
  return 0;
}

void set_dungeon_view(dungeon *d){
  d->x = d->characters[0]->x;
  d->y = d->characters[0]->y;
}

void free_characters(dungeon_t *dungeon){
  unsigned int i;

  for(i = 0; i < dungeon->num_characters; i++){
    delete(dungeon->characters[i]);
  }
  free(dungeon->characters);
}

void set_turn(character *c){
  c->next_turn += (turnbias / c->speed);
}

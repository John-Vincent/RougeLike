#include "../headers/character"
#include "../headers/path_finder.h"
#include "../headers/dungeon"

const struct{
  uint8_t type;
  char symbol;
} character_lookup[] = {
  {0x1F, '@'},
  {0x00, 'I'}, //0000
  {0x01, 'f'}, //0001
  {0x02, 'a'}, //0010
  {0x03, 'r'}, //0011
  {0x04, '1'}, //0100
  {0x05, 'j'}, //0101
  {0x06, 'b'}, //0110
  {0x07, '5'}, //0111
  {0x08, 'V'}, //1000
  {0x09, 'k'}, //1001
  {0x0A, '&'}, //1010
  {0x0B, '*'}, //1011
  {0x0C, 'y'}, //1100
  {0x0D, '+'}, //1101
  {0x0E, '~'}, //1110
  {0x0F, '$'}, //1111
  {0xFF, ' '}
};

npc::npc(int gen){
  Dungeon *dungeon;
  character *c;
  room_t* r;
  int a, x, y, flag;
  int i;

  dungeon = Dungeon::get_instance();
  r = dungeon->get_room(0);

  this->gen = gen;
  this->speed = (rand() & 0xF) + 5;
  this->attrib = (rand() & 0xF);
  this->next_turn = 0;
  r = r + (rand() % dungeon->get_num_rooms());
  a = (r->width)*(r->height);
  flag = 0;
  do{
    if(a > 0){
      x = rand() % (r->width+1) + r->xPos;
      y = rand() % (r->height+1) + r->yPos;
    } else{
      x = rand() % (mapWidth-2) + 1;
      y = rand() % (mapHeight-2) + 1;
    }
    a--;
    for(i = 0; i<gen; i++){
      c = dungeon->get_character(i);
      if(c && c->x == x && c->y == y)
        flag = 1;
    }
  } while(flag);

  this->x = x;
  this->y = y;

  x = 0;
  while(character_lookup[x].type != 0xFF){
    if(character_lookup[x].type == this->attrib){
      this->sym = character_lookup[x].symbol;
      break;
    }
    x++;
  }
}




int npc::take_turn(int input){
  Dungeon *dungeon;
  character *c;
  int x, y, rando, i, j;
  uint8_t hardness;
  unsigned int k, dist;
  uint16_t *distance;
  dist = 0x0FFFFFFF;

  dungeon = Dungeon::get_instance();

  x = 0;
  y = 0;

  distance = dungeon->get_distance_array(this->attrib);

  rando = rand() & 0x1;
  if( !(this->attrib & erratic && rando) &&
    (this->attrib & telepathic || dungeon->can_see_player(this->x, this->y))){
    for(i = this->y-1; i < this->y +2; i++){
      for(j = this->x - 1; j < this->x + 2; j++){
        if((i == this->y||j == this->x) && (i != this->y || j != this->x)){
          if(*(distance + i*mapWidth + j) < dist){
            if(!dungeon->get_hardness(j, i)){
              x = j;
              y = i;
              dist = *(distance + i * mapWidth + j);
            } else if(this->attrib & tunneler){
              x = j;
              y = i;
              dist = *(distance + i*mapWidth + j);
            }
          }
        }
      }
    }
  }
  if(!x){
    x = 0;
    while(!x){
      if(rand() & 1){
        x = rand() % 3 - 1 + this->x;
        y = this->y;
      }
      else{
        y = rand() % 3 - 1 + this->y;
        x = this->x;
      }
      if(dungeon->get_hardness(x, y)){
        if(!(this->attrib & tunneler && x > 0 && x<mapWidth-1 && y>0 && y<mapHeight-1)){
          x = 0;
        }
      } else if(!(y - this->y||x - this->x) || !(!y - this->y||!x - this->x)){
        x=0;
      }
    }
  }

  hardness = dungeon->get_hardness(x,y);
  if(hardness){
    if(hardness > 85){
      dungeon->get_hardness(x, y) -= 85;
      dungeon->calculate_distances();
      return 0;
    }
    dungeon->get_hardness(x,y) = 0;
    dungeon->get_environment(x, y) = '#';
    dungeon->calculate_distances();
  }

  for(k = 0; k < dungeon->get_num_characters(); k++) {
    c = dungeon->get_character(k);
    if(c->x == x && c->y == y && c != this){
      c->attrib = 0xFFFFFFFF;
    }
  }

  this->x = x;
  this->y = y;

  return 0;

}

#include "../headers/character"
#include "../headers/path_finder.h"
#include "../headers/dungeon"

#define look_scroll 5

pc::pc(){
  Dungeon *dungeon;
  int j, i;
  room_t *r;

  dungeon = Dungeon::get_instance();
  r = dungeon->get_room(0);

  this->speed = 10;
  this->attrib = 0xFF;
  this->gen = 0;
  this->next_turn = 0;
  r = r + (rand() % dungeon->get_num_rooms());
  this->x = dungeon->get_x() =rand() % r->width + r->xPos;
  this->y = dungeon->get_y() = rand() % r->height + r->yPos;
  for(i = this->y - torch_distance; i < this->y + 1 + torch_distance; i++){
    for(j = this->x - torch_distance; j < this->x + 1 + torch_distance; j++){
      if(j > 0 && j < mapWidth && i > 0 && i < mapHeight){
        dungeon->visited(j,i) = 1;
      }
    }
  }
  this->sym = '@';
}

int pc::take_turn(int input){
  Dungeon *dungeon;
  character *c;
  int x = 0, y = 0, look;
  int i, j;
  unsigned int k;

  dungeon = Dungeon::get_instance();
  look = dungeon->get_look();

  switch(input){
    // 7 || y
    case '7':
    case 'y':
      if(!look){
        x = this->x - 1;
        y = this->y - 1;
      }
      break;
    // 8 || k
    case '8':
    case 'k':
      if(!look){
        x = this->x;
        y = this->y - 1;
      }else{
        x = dungeon->get_x();
        y = dungeon->get_y() - look_scroll;
      }
      break;
    // 9 || u
    case '9':
    case 'u':
      if(!look){
        x = this->x + 1;
        y = this->y - 1;
      }
      break;
    // 6 || l
    case '6':
    case 'l':
      if(!look){
        x = this->x + 1;
        y = this->y;
      }else{
        x = dungeon->get_x() + look_scroll;
        y = dungeon->get_y();
      }
      break;
    // 3 || n
    case 'n':
    case '3':
      if(!look){
        x = this->x + 1;
        y = this->y + 1;
      }
      break;
    // 2 || j
    case '2':
    case 'j':
      if(!look){
        x = this->x;
        y = this->y + 1;
      }else{
        x = dungeon->get_x();
        y = dungeon->get_y() + look_scroll;
      }
      break;
    // 1 || b
    case '1':
    case 'b':
      if(!look){
        x = this->x - 1;
        y = this->y + 1;
      }
      break;
    // 4 || h
    case '4':
    case 'h':
      if(!look){
        x = this->x - 1;
        y = this->y;
      }else{
        x = dungeon->get_x() - look_scroll;
        y = dungeon->get_y();
      }
      break;
    // 5 || space
    case '5':
    case ' ':
      if(!look){
        x = this->x;
        y = this->y;
      }
      break;

  }

  if(!x){
    return 1;
  }

  if(!look){

    if(!dungeon->get_hardness(x, y)){

      for(k = 0; k < dungeon->get_num_characters(); k++) {
        c = dungeon->get_character(k);
        if(c->x == x && c->y == y && c != this){
          c->attrib = 0xFFFFFFFF;
        }
      }

      this->x = dungeon->get_x() = x;
      this->y = dungeon->get_y() = y;
      for(i = this->y-torch_distance; i < this->y+torch_distance; i++){
        for(j = this->x-torch_distance; j < this->x + torch_distance; j++){
          if(j > 0 && j < mapWidth && i > 0 && i < mapHeight){
            dungeon->visited(j, i) = 1;
          }
        }
      }
      dungeon->calculate_distances();
    } else{
      return 1;
    }

  } else {

    if(x < 0 + (screen_width >> 1))
      x = 0 + (screen_width >> 1);
    else if(x > mapWidth - (screen_width >> 1))
      x = mapWidth - (screen_width >> 1);

    if(y < 0 + (screen_height >> 1))
      y = 0 + (screen_height >> 1);
    else if(y > mapHeight - (screen_height >> 1))
      y = mapHeight - (screen_height >> 1);

    dungeon->get_x() = x;
    dungeon->get_y() = y;

  }

  return 0;
}

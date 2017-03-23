#include "../headers/character_cpp.h"
#include "../headers/path_finder.h"

#define look_scroll 5

pc::pc(dungeon_t *dungeon){
  int j, i;
  room_t *r;

  this->speed = 10;
  this->attrib = 0x1F;
  this->gen = 0;
  this->next_turn = 0;
  this->dungeon = dungeon;
  r = dungeon->rooms + (rand() % dungeon->numRooms);
  this->x = dungeon->x =rand() % r->width + r->xPos;
  this->y = dungeon->y = rand() % r->height + r->yPos;
  for(i = this->y-torch_distance; i < this->y + 1 + torch_distance; i++){
    for(j = this->x-torch_distance; j < this->x + 1 + torch_distance; j++){
      if(j > 0 && j < mapWidth && i > 0 && i < mapHeight){
        dungeon->visited[i][j] = 1;
      }
    }
  }
  this->sym = '@';
}

int pc::take_turn(int input){
  int x = 0, y = 0, look = dungeon->look;
  int i, j;
  unsigned int k;

  switch(input){
    // 7 || y
    case '7':
    case 'y':
      if(!look){
        x = dungeon->characters[0]->x - 1;
        y = dungeon->characters[0]->y - 1;
      }
      break;
    // 8 || k
    case '8':
    case 'k':
      if(!look){
        x = dungeon->characters[0]->x;
        y = dungeon->characters[0]->y - 1;
      }else{
        x = dungeon->x;
        y = dungeon->y - look_scroll;
      }
      break;
    // 9 || u
    case '9':
    case 'u':
      if(!look){
        x = dungeon->characters[0]->x + 1;
        y = dungeon->characters[0]->y - 1;
      }
      break;
    // 6 || l
    case '6':
    case 'l':
      if(!look){
        x = dungeon->characters[0]->x + 1;
        y = dungeon->characters[0]->y;
      }else{
        x = dungeon->x + look_scroll;
        y = dungeon->y;
      }
      break;
    // 3 || n
    case 'n':
    case '3':
      if(!look){
        x = dungeon->characters[0]->x + 1;
        y = dungeon->characters[0]->y + 1;
      }
      break;
    // 2 || j
    case '2':
    case 'j':
      if(!look){
        x = dungeon->characters[0]->x;
        y = dungeon->characters[0]->y + 1;
      }else{
        x = dungeon->x;
        y = dungeon->y + look_scroll;
      }
      break;
    // 1 || b
    case '1':
    case 'b':
      if(!look){
        x = dungeon->characters[0]->x - 1;
        y = dungeon->characters[0]->y + 1;
      }
      break;
    // 4 || h
    case '4':
    case 'h':
      if(!look){
        x = dungeon->characters[0]->x - 1;
        y = dungeon->characters[0]->y;
      }else{
        x = dungeon->x - look_scroll;
        y = dungeon->y;
      }
      break;
    // 5 || space
    case '5':
    case ' ':
      if(!look){
        x = dungeon->characters[0]->x;
        y = dungeon->characters[0]->y;
      }
      break;

  }

  if(!x){
    return 1;
  }

  if(!look){

    if(!dungeon->hardness[y][x]){

      for(k = 0; k < dungeon->num_characters; k++) {
        if(dungeon->characters[k]->x == x && dungeon->characters[k]->y == y && dungeon->characters[k] != this){
          dungeon->characters[k]->attrib = 0xFFFFFFFF;
        }
      }

      dungeon->characters[0]->x = dungeon->x = x;
      dungeon->characters[0]->y = dungeon->y = y;
      for(i = this->y-torch_distance; i < this->y+torch_distance; i++){
        for(j = this->x-torch_distance; j < this->x + torch_distance; j++){
          if(j > 0 && j < mapWidth && i > 0 && i < mapHeight){
            dungeon->visited[i][j] = 1;
          }
        }
      }
      find_distances(dungeon->hardness, dungeon->distances, x, y, 0, 1);
      find_distances(dungeon->hardness, dungeon->distances_intel, x, y, 1, 1);
      find_distances(dungeon->hardness, dungeon->distances_notun, x, y, 1, 0);
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

    dungeon->x = x;
    dungeon->y = y;

  }

  return 0;
}

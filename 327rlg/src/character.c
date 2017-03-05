#include "../headers/character.h"

#define look_scroll 10

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


  int generate_characters(dungeon_t *dungeon, uint32_t num_monsters){

    if(num_monsters + 1 > (mapWidth-2) * (mapHeight-2)){
      printf("too many Monsters!");
      return -1;
    }

    int i, x, y, a;
    room_t *r;
    character_t *characters = calloc(num_monsters +1, sizeof (character_t));
    if(!characters){
      printf("allocation falure");
      return -1;
    }

    characters[0].speed = 10;
    characters[0].attrib = 0x1F;
    characters[0].gen = dungeon->num_characters++;
    r = dungeon->rooms + (rand() % dungeon->numRooms);
    characters[0].x = dungeon->x =rand() % r->width + r->xPos;
    characters[0].y = dungeon->y = rand() % r->height + r->yPos;
    characters[0].sym = character_lookup[0].symbol;
    dungeon->characters[characters[0].y][characters[0].x] = characters;
    dungeon->player = characters;

    for(i = 1; i < num_monsters+1; i++){
      characters[i].speed = (rand() & 0xF) + 5;
      characters[i].attrib = rand() & 0xF;
      characters[i].gen = dungeon->num_characters++;
      r = dungeon->rooms + (rand() % dungeon->numRooms);
      a = (r->width)*(r->height);
      do{
        if(a > 0){
          x = rand() % (r->width+1) + r->xPos;
          y = rand() % (r->height+1) + r->yPos;
        } else{
          x = rand() % (mapWidth-2) + 1;
          y = rand() % (mapHeight-2) + 1;
        }
        a--;
      } while(dungeon->characters[y][x]);

      characters[i].x = x;
      characters[i].y = y;
      dungeon->characters[y][x] = characters + i;
      x = 0;
      while(character_lookup[x].type != 0xFF){
        if(character_lookup[x].type == characters[i].attrib){
          characters[i].sym = character_lookup[x].symbol;
          break;
        }
        x++;
      }
      if(character_lookup[x].type == 0xFF){
        printf("lookup failure");
        return -1;
      }
    }
    return 0;
  }




  int compare_characters(void *child, void *parent){
    character_t *b = (character_t*) parent;
    character_t *a = (character_t*) child;
    int32_t dif = b->next_turn - a->next_turn;
    if(dif != 0){
      return dif;
    }
    return b->gen - a->gen;
  }





  int take_turn(dungeon_t *dungeon, character_t *character){
    int x, y, i, j, dist, rando;
    uint16_t *distance;
    dist = 0x0FFFFFFF;


    x = 0;
    y = 0;
    if(character->attrib & intelligent){
      if(character->attrib & tunneler)
        distance = *dungeon->distances_intel;
      else
        distance = *dungeon->distances_notun;
    }
    else{
      distance = *dungeon->distances;
    }

    rando = rand() & 0x1;
    if( !(character->attrib & erratic && rando) &&
      (character->attrib & telepathic || can_see(dungeon->hardness, dungeon->player->x, dungeon->player->y, character->x, character->y))){
      for(i = character->y-1; i < character->y +2; i++){
        for(j = character->x - 1; j < character->x + 2; j++){
          if((i == character->y||j == character->x) && (i != character->y || j != character->x)){
            if(*(distance + i*mapWidth + j) < dist){
              if(!dungeon->hardness[i][j]){
                x = j;
                y = i;
                dist = *(distance + i * mapWidth + j);
              } else if(character->attrib & tunneler){
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
          x = rand() % 3 - 1 + character->x;
          y = character->y;
        }
        else{
          y = rand() % 3 - 1 + character->y;
          x = character->x;
        }
        if(dungeon->hardness[y][x]){
          if(!(character->attrib & tunneler && x > 0 && x<mapWidth-1 && y>0 && y<mapHeight-1)){
            x = 0;
          }
        } else if(!(y-character->y||x-character->x) || !(!y-character->y||!x-character->x)){
          x=0;
        }
      }
    }


    if(dungeon->hardness[y][x]){
      if(dungeon->hardness[y][x] > 85){
        dungeon->hardness[y][x] -= 85;
        find_distances(dungeon->hardness, dungeon->distances, dungeon->player->x, dungeon->player->y, 0, 1);
        find_distances(dungeon->hardness, dungeon->distances_intel, dungeon->player->x, dungeon->player->y, 1, 1);
        find_distances(dungeon->hardness, dungeon->distances_notun, dungeon->player->x, dungeon->player->y, 1, 0);
        return 0;
      }
      dungeon->hardness[y][x] = 0;
      dungeon->chars[y][x] = '#';
      find_distances(dungeon->hardness, dungeon->distances, dungeon->player->x, dungeon->player->y, 0, 1);
      find_distances(dungeon->hardness, dungeon->distances_intel, dungeon->player->x, dungeon->player->y, 1, 1);
      find_distances(dungeon->hardness, dungeon->distances_notun, dungeon->player->x, dungeon->player->y, 1, 0);
    }

    dungeon->characters[character->y][character->x] = NULL;

    if(dungeon->characters[y][x]){
      if(dungeon->characters[y][x] == dungeon->player){
        dungeon->player->attrib = 0xffffffff;
        dungeon->characters[y][x] = character;
        return 1;
      }else{
        dungeon->characters[character->y][character->x] = dungeon->characters[y][x];
        dungeon->characters[y][x]->y = character->y;
        dungeon->characters[y][x]->x = character->x;
      }
    }

    character->x = x;
    character->y = y;
    dungeon->characters[y][x] = character;


    return 0;

  }


  int player_turn(dungeon_t *dungeon, int look, int input){
    int x = 0, y = 0;

    switch(input){
      // 7 || y
      case '7':
      case 'y':
        if(!look){
          x = dungeon->player->x - 1;
          y = dungeon->player->y - 1;
        }
        break;
      // 8 || k
      case '8':
      case 'k':
        if(!look){
          x = dungeon->player->x;
          y = dungeon->player->y - 1;
        }else{
          x = dungeon->x;
          y = dungeon->y - look_scroll;
        }
        break;
      // 9 || u
      case '9':
      case 'u':
        if(!look){
          x = dungeon->player->x + 1;
          y = dungeon->player->y - 1;
        }
        break;
      // 6 || l
      case '6':
      case 'l':
        if(!look){
          x = dungeon->player->x + 1;
          y = dungeon->player->y;
        }else{
          x = dungeon->x + look_scroll;
          y = dungeon->y;
        }
        break;
      // 3 || n
      case 'n':
      case '3':
        if(!look){
          x = dungeon->player->x + 1;
          y = dungeon->player->y + 1;
        }
        break;
      // 2 || j
      case '2':
      case 'j':
        if(!look){
          x = dungeon->player->x;
          y = dungeon->player->y + 1;
        }else{
          x = dungeon->x;
          y = dungeon->y + look_scroll;
        }
        break;
      // 1 || b
      case '1':
      case 'b':
        if(!look){
          x = dungeon->player->x - 1;
          y = dungeon->player->y + 1;
        }
        break;
      // 4 || h
      case '4':
      case 'h':
        if(!look){
          x = dungeon->player->x - 1;
          y = dungeon->player->y;
        }else{
          x = dungeon->x - look_scroll;
          y = dungeon->y;
        }
        break;
      // 5 || space
      case '5':
      case ' ':
        if(!look){
          x = dungeon->player->x;
          y = dungeon->player->y;
        }
        break;

    }

    if(!x){
      return 1;
    }

    if(!look){

      if(!dungeon->hardness[y][x]){
        dungeon->characters[dungeon->player->y][dungeon->player->x] = NULL;

        if(dungeon->characters[y][x]){
          dungeon->characters[y][x]->attrib = 0xFFFFFFFF;
        }

        dungeon->player->x = dungeon->x = x;
        dungeon->player->y = dungeon->y = y;
        dungeon->characters[y][x] = dungeon->player;
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

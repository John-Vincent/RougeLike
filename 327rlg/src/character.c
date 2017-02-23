#include "../headers/character.h"


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
  {0x0D, '<'}, //1101
  {0x0E, '~'}, //1110
  {0x0F, '$'}, //1111
  {0xFF, ' '}
};


  void *generate_characters(dungeon_t *dungeon, uint32_t num_monsters){

    if(num_monsters + 1 > (mapWidth-2) * (mapHeight-2)){
      printf("too many Monsters!");
      return NULL;
    }

    int i, x, y, a;
    room_t *r;
    character_t *characters = calloc(num_monsters +1, sizeof (character_t));

    characters[0].speed = 10;
    characters[0].attrib = 0x1F;
    characters[0].gen = dungeon->num_characters++;
    r = dungeon->rooms + (rand() % dungeon->numRooms);
    characters[0].x = rand() % r->width + r->xPos;
    characters[0].y = rand() % r->height + r->yPos;
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
        return NULL;
      }
    }
    return characters;
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
    if(character->attrib & intelligent)
      distance = *dungeon->distances_intel;
    else
      distance = *dungeon->distances;

    if(character != dungeon->player){
      rando = rand() & 0x1;
      if( !(character->attrib & erratic && rando) &&
        (character->attrib & telepathic || can_see(dungeon->hardness, dungeon->player->x, dungeon->player->y, character->x, character->y))){
        for(i = character->y-1; i < character->y +2; i++){
          for(j = character->x - 1; j < character->x + 2; j++){
            if(i-character->y||j-character->x){
              if(*(distance + i*mapWidth + j) < dist){
                if(!dungeon->hardness[i][j]){
                  x = j;
                  y = i;
                  dist = *(distance + i * mapWidth + j);
                } else if(character->attrib & tunneler){
                  if(dungeon->hardness[i][j] > 85){
                    dungeon->hardness[i][j] -= 85;
                  } else{
                    dungeon->hardness[i][j] = 0;
                    dungeon->chars[i][j] = '#';
                  }
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
          x = rand() % 3 - 1 + character->x;
          y = rand() % 3 - 1 + character->y;
          if(dungeon->hardness[y][x]){
            if(character->attrib & tunneler && x > 0 && x<mapWidth-1 && y>0 && y<mapHeight-1){
              if(dungeon->hardness[y][x] > 85){
                dungeon->hardness[y][x] -= 85;
              } else{
                dungeon->hardness[y][x] = 0;
                dungeon->chars[y][x] = '#';
              }
            } else{
              x = 0;
            }
          }
        }
      }
    } else{
      while(!x){
        x = rand() % 3 - 1 + character->x;
        y = rand() % 3 - 1 + character->y;
        if(dungeon->hardness[y][x]){
            x = 0;
        }
      }
      find_distances(dungeon->hardness, dungeon->distances, x, y, 0);
      find_distances(dungeon->hardness, dungeon->distances_intel, x, y, 1);
    }

    dungeon->characters[character->y][character->x] = NULL;

    if(dungeon->characters[y][x]){
      dungeon->characters[y][x]->attrib = 0xFFFFFFFF;
    }

    character->x = x;
    character->y = y;
    dungeon->characters[y][x] = character;



    if(dungeon->player->attrib == 0xFFFFFFFF)
      return 1;

    return 0;

  }

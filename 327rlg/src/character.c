#include "../headers/character.h"


const struct{
  uint8_t type;
  char symbol;
} character_lookup[] = {
  {0x1F, '@'},
  {0x00, 'I'},
  {0x01, 'f'},
  {0x02, 'a'},
  {0x03, 'r'},
  {0x04, '1'},
  {0x05, 'j'},
  {0x06, 'b'},
  {0x07, '5'},
  {0x08, 'V'},
  {0x09, 'k'},
  {0x0A, '&'},
  {0x0B, '*'},
  {0x0C, 'y'},
  {0x0D, '<'},
  {0x0E, '~'},
  {0x0F, '$'},
  {0xFF, ' '}
};


  uint32_t generate_characters(dungeon_t *dungeon, uint32_t num_monsters){

    if(num_monsters + 1 > (mapWidth-2) * (mapHeight-2)){
      printf("too many Monsters!");
      return -1;
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


    for(i = 1; i < num_monsters+1; i++){
      characters[i].speed = (rand() & 0xFF) + 5;
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

  uint32_t compareCharacters(void *child, void *parent){
    character_t *b = (character_t*) parent;
    character_t *a = (character_t*) child;
    int32_t dif = b->next_turn - a->next_turn;
    if(dif != 0){
      return dif;
    }
    return b->gen - a->gen;
  }

  uint32_t takeTurn(dungeon_t *dungeon, character_t *character){

    return 0;

  }

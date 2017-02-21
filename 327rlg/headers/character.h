#ifndef CHARACTER_H
#define CHARACTER_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "map.h"


#define intelligent = 0x1;
#define telepathic = 0x2;
#define tunneler = 0x4;
#define erratic = 0x8;

struct dungeon;

typedef struct character{
  char sym;
  uint8_t speed;
  uint32_t next_turn;
  uint32_t gen;
  uint32_t attrib;
  uint8_t x;
  uint8_t y;
} character_t;
#include "main.h"



uint32_t takeTurn(struct dungeon *dungeon, character_t *character);

uint32_t generate_characters(struct dungeon *dungeon, uint32_t num_monsters);


#endif

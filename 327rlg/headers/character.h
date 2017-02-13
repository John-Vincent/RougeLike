#ifndef CHARACTER_H
#define CHARACTER_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "map.h"


typedef struct character{
  char sym;
  uint8_t x;
  uint8_t y;
} character_t;

int make_character(character_t *c, const room_t *rooms, uint8_t numRooms, char *type);

#endif

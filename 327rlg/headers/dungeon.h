#ifndef DUNGEON_H
#define DUNGEON_H

#include <stdint.h>
#include "constants.h"
#include "character_cpp.h"
#include "map.h"
#include "heap.h"

#ifndef __cplusplus
typedef void character;
#endif

#ifdef __cplusplus
extern "C"{
#endif


typedef struct dungeon{
  uint8_t x;
  uint8_t y;
  uint8_t look;
  uint8_t hardness[mapHeight][mapWidth];
  uint8_t visited[mapHeight][mapWidth];
  char chars[mapHeight][mapWidth];
  uint16_t distances[mapHeight][mapWidth];
  uint16_t distances_notun[mapHeight][mapWidth];
  uint16_t distances_intel[mapHeight][mapWidth];
  character **characters;
  room_t *rooms;
  uint8_t numRooms;
  uint32_t num_characters;
  struct dungeon *upstairs;
  struct dungeon *downstairs;
  heap_t *turn_order;
  char *message;
} dungeon_t;


/**
 * this will recursivly free all the dungeons made during the operation of this game
 * @param  dungeon                 the root dungeon
 * @author Collin Vincent <collinvincent96@gmail.com>
 * @date   2017-03-05T20:12:12+000
 */
void free_mem(dungeon_t *dungeon);

/**
 * allocates the dungeon and generates the floor
 *
 * @param  dungeon                 pointer to the dungeon pointer to be given a dungeon
 * @author Collin Vincent <collinvincent96@gmail.com>
 * @date   2017-03-06T22:24:42+000
 */
int init_dungeon(dungeon_t **dungeon, int nummon);


#ifdef __cplusplus
}
#endif

#endif

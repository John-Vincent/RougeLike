#ifndef RLG327
#define RLG327

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "map.h"
#include "gameio.h"
#include "character.h"
#include "path_finder.h"

#define default_monster_num 10;

typedef enum{ GENERATE, SAVE, LOAD, LOAD_SAVE} game_mode_t;

typedef struct dungeon{
  uint8_t hardness[mapHeight][mapWidth];
  char chars[mapHeight][mapWidth];
  uint16_t distances[mapHeight][mapWidth];
  character_t *characters[mapHeight][mapWidth];
  character_t player;
  room_t *rooms;
  uint8_t numRooms;
  uint32_t num_characters;
} dungeon_t;


/**
 * initalizes the random number generator base on user input, this is basically just to keep main clean.
 * @param  argc                    argument count from command line
 * @param  argv                    command line arguments
 * @author Collin Vincent <collinvincent96@gmail.com>
 * @date   2017-01-24T23:04:05+000
 */
void initRandom(int *seed);

/**
 * reads the command line arguments
 * @param  argc                    the number of args
 * @param  argv                    the arguments
 * @param  mode                    pointer to a game mode
 * @param  seed                    the time seed that is to be used
 * @param  nummon                  the number of monsters to generate
 * @return                         0 if success -1 if fail
 * @author Collin Vincent <collinvincent96@gmail.com>
 * @date   2017-02-20T22:05:29+000
 */
uint32_t readArgs(int argc, char const *argv[], game_mode_t *mode, int **seed, int *nummon);

/**
 * does the logic for saveing the game to keep the main clean
 * @param  [name]                  array of hardness values
 * @param  [name]                  array of characters
 * @param  rooms                   array of rooms
 * @param  room_count              number of rooms
 * @return                         0 if successs -1 if fail
 * @author Collin Vincent <collinvincent96@gmail.com>
 * @date   2017-02-01T23:13:24+000
 */
int save(uint8_t map_hard[mapHeight][mapWidth], char map_char[mapHeight][mapWidth], room_t **rooms, uint8_t *room_count);

/**
 * does the logic for loading the map to keep main clean
 * @param  [name]                  array of hardness values
 * @param  [name]                  array of characters
 * @param  rooms                   pointer to the rooms pointer
 * @return                         [description]
 * @author Collin Vincent <collinvincent96@gmail.com>
 * @date   2017-02-01T23:18:02+000
 */
int load(uint8_t map_hard[mapHeight][mapWidth], char map_char[mapHeight][mapWidth], room_t **rooms, uint8_t *room_count);

/**
 * does the logic for the load and save option to keep the main clean
 * @param  [name]                  array of hardness values
 * @param  [name]                  array of characters
 * @param  rooms                   array of rooms
 * @param  room_count              number of rooms
 * @return                         [description]
 * @author Collin Vincent <collinvincent96@gmail.com>
 * @date   2017-02-01T23:40:08+000
 */
int load_save(uint8_t map_hard[mapHeight][mapWidth], char map_char[mapHeight][mapWidth], room_t **rooms, uint8_t *room_count);



#endif

#ifndef CHARACTER_H
#define CHARACTER_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "map.h"


#define intelligent 0x1
#define telepathic 0x2
#define tunneler 0x4
#define erratic 0x8

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


/**
 * does the logic for proccessing an NPC turn
 * @param  dungeon                 the dungeon the npc is in
 * @param  character               the npc to take a turn
 * @return                         0 if the player is alive 1 if the player is dead
 * @author Collin Vincent <collinvincent96@gmail.com>
 * @date   2017-03-05T20:17:29+000
 */
int take_turn(struct dungeon *dungeon, character_t *character);

/**
 * proccesses a players turn
 * @param  dungeon                 the dungeon the player is in
 * @param  look                    0 if in control mode anything else if in look mode
 * @param  input                   the input to proccess from the player
 * @return                         0 if a turn consuming operation has been made 1 if a non-turn consuming operation has been made
 * @author Collin Vincent <collinvincent96@gmail.com>
 * @date   2017-03-05T20:18:18+000
 */
int player_turn(struct dungeon *dungeon, int look, int input);

/**
 * allocates an array of characters of size num_monsters + 1 which is then filled with randomly created npcs as well as the player
 * the player is the first element in the array and a pointer to it is placed in the dungeon_t's player pointer
 *
 * @param  dungeon                 the dungeon that gets the characters
 * @param  num_monsters            the number of monsters to be made
 * @author Collin Vincent <collinvincent96@gmail.com>
 * @date   2017-03-05T20:20:26+000
 */
int generate_characters(struct dungeon *dungeon, uint32_t num_monsters);

/**
 * compares two character_t's based on turn time then creation order if turn time is the same
 * @param  child                   first character to be compared
 * @param  parent                  second character to be compared
 * @author Collin Vincent <collinvincent96@gmail.com>
 * @date   2017-03-05T20:22:10+000
 */
int compare_characters(void *child, void *parent);

#endif

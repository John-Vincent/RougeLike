#ifndef RLGDES_H
#define RLGDES_H

#include <unistd.h>
#include <curses.h>
#include <string.h>
#include <stdio.h>
#include "main.h"
#include "map.h"
#include "heap.h"
#include "path_finder.h"
#include "character.h"

#define turnbias 1000

/**
 * this displays the dungeon with all its characters to the ncurses screen
 * @param  dungeon                 the dungeon to be drawn
 * @author Collin Vincent <collinvincent96@gmail.com>
 * @date   2017-03-05T20:15:47+000
 */
void display(dungeon_t *dungeon);


/**
 * this is the main game loop that does the logic for input and timing monster movements
 * @param  dungeon                 the dungeon that is being run
 * @return                         [description]
 * @author Collin Vincent <collinvincent96@gmail.com>
 * @date   2017-03-05T20:16:29+000
 */
int run_game(dungeon_t *dungeon);





#endif

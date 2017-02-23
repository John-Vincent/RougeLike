#ifndef RLGDES_H
#define RLGDES_H

#include <unistd.h>
#include "main.h"
#include "map.h"
#include "heap.h"
#include "path_finder.h"

#define turnbias 1000

void display(const dungeon_t *dungeon);

int run_game(dungeon_t *dungeon);





#endif

#ifndef PATH_FINDER
#define PATH_FINDER

#include <stdlib.h>
#include <stdint.h>
#include "map.h"
#include "heap.h"


typedef struct path_node{
  struct path_node *prev;
  struct path_node *next;
  uint8_t y;
  uint8_t x;
} path_node_t;

typedef struct path{
  path_node_t *start;
  uint32_t dist;
} path_t;

typedef enum{
  up,
  down,
  left,
  right,
} direction_t;

/**
 * finds the shortest path between two locations
 * @param  [name]                  array of hardness values
 * @param  start_x                 [description]
 * @param  start_y                 [description]
 * @param  end_x                   [description]
 * @param  end_y                   [description]
 * @author Collin Vincent <collinvincent96@gmail.com>
 * @date   2017-02-04T00:14:39+000
 */
path_t find_shortest_path(uint8_t hardness[mapHeight][mapWidth], uint8_t start_x, uint8_t start_y, uint8_t end_x, uint8_t end_y);

/**
 * finds the distances from one point in the map to every other point
 * @param  [name]                  [description]
 * @param  [name]                  [description]
 * @param  start_x                 [description]
 * @param  starty                  [description]
 * @author Collin Vincent <collinvincent96@gmail.com>
 * @date   2017-02-13T13:29:04+000
 */
int find_distances(uint8_t hardness[mapHeight][mapWidth], uint16_t distances[mapHeight][mapWidth], uint8_t start_x, uint8_t starty);

/**
 * this method frees all the nodes of the dynamically allocated path linked list
 * @param  p                       a pointer to a path object;
 * @author Collin Vincent <collinvincent96@gmail.com>
 * @date   2017-02-08T10:44:57+000
 */
void deletePath(path_t p);


#endif

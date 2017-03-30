#ifndef GAMEIO_H
#define GAMEIO_H

#include <endian.h>
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "constants.h"
#include "map.h"


/**
 * saves the current game map with the following rules
 * byte range         data contents
 * 0-11               file type marker "RLG327-S2017"
 * 12-15              unsigned int with file version number
 * 16-19              unsigned int with file size
 * 20-16819           map hardness data with one byte for each room
 * 16820-end          room data with 4 bytes per room xpos, ypos, width, height
 *
 * @param  rooms                   the array of rooms
 * @param  map_hard                the array of hardness values
 * @return                         0 if it worked -1 if it failed
 * @author Collin Vincent <collinvincent96@gmail.com>
 * @date   2017-02-01T20:23:07+000
 */
int saveMap(uint8_t numRooms, room_t const *rooms, uint8_t map_hard[mapHeight][mapWidth]);

/**
 * loads a map from the files
 * @param  rooms                   pointer to a room_t pointer that will hold the room data read from file
 * @param  map_hard                array to hold map hardness values
 * @return                         int 0 if it worked -1 if it failed
 * @author Collin Vincent <collinvincent96@gmail.com>
 * @date   2017-02-01T20:26:11+000
 */
int loadMap(room_t **rooms, uint8_t *numRooms, uint8_t map_hard[mapHeight][mapWidth]);


#endif

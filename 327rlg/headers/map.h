#ifndef MAP_H
#define MAP_H

#define mapWidth 160
#define mapHeight 105
#define Room_Min_Width 7
#define Room_Min_Height 5
#define Room_Min_Number 10
#define Room_Width_Range 20
#define Room_Height_Range 15
#define Room_Number_Range 10

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "path_finder.h"


typedef struct room {
  uint8_t xPos;
  uint8_t yPos;
  uint8_t width;
  uint8_t height;
} room_t;

/**
 * generates an array of Rooms
 * @param  numRooms                the number of rooms to be generated
 * @return                         a pointer to the array of rooms
 * @author Collin Vincent <collinvincent96@gmail.com>
 * @date   2017-01-25T17:36:02+000
 */
int generateRooms(room_t *rooms, uint8_t numRooms);

/**
 * draws the Rooms to the map
 * this reflects the data in the hardness array and the room arry to the character array
 * @param  map                     a pointer to the matrix of the game map
 * @param  rooms                   a pointer to the array of rooms
 * @param  numRooms                the number of rooms
 * @author Collin Vincent <collinvincent96@gmail.com>
 * @date   2017-01-25T17:36:38+000
 */
void drawMap(uint8_t map[mapHeight][mapWidth], char map_char[mapHeight][mapWidth], room_t *rooms, uint8_t numRooms);

/**
 * draws a straight line from the center of one room and and the center of its neighbor in the array
 * this only effects the hardness array
 * @param  map                     array of hardnesses
 * @param  rooms                   array of rooms
 * @param  numRooms                amount of rooms in the array
 * @author Collin Vincent <collinvincent96@gmail.com>
 * @date   2017-01-25T20:54:01+000
 */
void connect_rooms(uint8_t map[mapHeight][mapWidth], room_t *rooms, uint8_t numRooms);

/**
 * generates a random rock hardness value for each position in the map.
 * @param  map                  the matrix representing the game area
 * @author Collin Vincent <collinvincent96@gmail.com>
 * @date   2017-01-24T22:46:08+000
 */
void generateHardness(uint8_t map[mapHeight][mapWidth]);

/**
 * sets all the characters to a space
 * @param  map_char                  the map in character form
 * @author Collin Vincent <collinvincent96@gmail.com>
 * @date   2017-01-25T18:28:25+000
 */
void init_map_char(char map_char[mapHeight][mapWidth]);


/**
 * prints the map to the stdout
 * @param  map                     matrix representing objects in the map
 * @author Collin Vincent <collinvincent96@gmail.com>
 * @date   2017-01-25T16:44:49+000
 */
void printMap(char map[mapHeight][mapWidth]);

/**
 * prints the distances for tunnling and non tunnling monsters
 * @param  distances               the distances for the map
 * @param  hardness                the hardness values for the map
 * @return
 * @author Collin Vincent <collinvincent96@gmail.com>
 * @date   2017-02-13T20:18:49+000
 */
void printDistances(uint16_t distances[mapHeight][mapWidth], uint8_t hardness[mapHeight][mapWidth], uint8_t x, uint8_t y, char sym);

/**
 * generates a new map
 *
 * @param  map_hard                an empty int array that will be filled with the maps hardness values.
 * @param  map_char                an empty char array that will  be filled with the maps characters.
 * @param  rooms                   a pointer to a room_t that will be allocated in the function and filled with an array of rooms.
 * @param  room_count              a int pointer that will be filled with the number of rooms.
 * @author Collin Vincent <collinvincent96@gmail.com>
 * @date   2017-02-01T16:22:56+000
 */
int generateMap(uint8_t map_hard[mapHeight][mapWidth], char map_char[mapHeight][mapWidth], room_t **rooms, uint8_t *room_count);


#endif

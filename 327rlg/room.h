#ifndef ROOM_H
#define ROOM_H
#define mapWidth 160
#define mapHeight 105

typedef struct room {
  int width;
  int height;
  int xPos;
  int yPos;
} room_t;

/**
 * generates an array of Rooms
 * @param  numRooms                the number of rooms to be generated
 * @return                         a pointer to the array of rooms
 * @author Collin Vincent <collinvincent96@gmail.com>
 * @date   2017-01-25T17:36:02+000
 */
int generateRooms(room_t *rooms, int numRooms);

/**
 * draws the Rooms to the map
 * @param  map                     a pointer to the matrix of the game map
 * @param  rooms                   a pointer to the array of rooms
 * @param  numRooms                the number of rooms
 * @author Collin Vincent <collinvincent96@gmail.com>
 * @date   2017-01-25T17:36:38+000
 */
void drawMap(int map[mapHeight][mapWidth], char map_char[mapHeight][mapWidth], room_t *rooms, int numRooms);

/**
 * draws a straight line from the center of one room and and the center of its neighbor in the array
 * @param  map                     array of hardnesses
 * @param  rooms                   array of rooms
 * @param  numRooms                amount of rooms in the array
 * @author Collin Vincent <collinvincent96@gmail.com>
 * @date   2017-01-25T20:54:01+000
 */
void connect_rooms(int map[mapHeight][mapWidth], room_t *rooms, int numRooms);

#endif

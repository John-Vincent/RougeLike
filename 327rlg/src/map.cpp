#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../headers/map.h"
#include "../headers/path_finder.h"


int generateRooms(room_t *rooms, uint8_t numRooms){
  int i, j, d = 1, distx, disty, c;

  c = 0;

  //printf("----------Rooms----------\nXpos, Ypos, Width, Height\n");
  for(i = 0; i<numRooms; i++){
    rooms[i].width = rand() % Room_Width_Range + Room_Min_Width;
    rooms[i].height = rand() % Room_Height_Range + Room_Min_Height;

    //while the room has not been place try to place the room
    while(d){
      //x and y position to test
      rooms[i].xPos = rand() % (mapWidth - rooms[i].width - 2) + 1;
      rooms[i].yPos = rand() % (mapHeight - rooms[i].height - 2) + 1;

      //assume no collisions will happen
      d = 0;

      //loop through rooms and check for any collsion
      for(j=i-1; j>-1; j--){

        //difference in position in each direction
        distx = rooms[j].xPos - rooms[i].xPos;
        disty = rooms[j].yPos - rooms[i].yPos;
        //check if its within the x range of any of the other rooms
        if(distx == 0 || (distx > 0 && distx <= rooms[i].width+1) || (distx < 0 && -1 - distx <= rooms[j].width)){
          //check if its within the y range of any of the other rooms, this would mean the room is within both x and y range so the rooms have collided
          if(disty == 0 || (disty > 0 && disty <= rooms[i].height+1) || (disty < 0 && -1 -disty <= rooms[j].height)){
            //collision detected reset the while loop control and skip checking the rest of the rooms
            d=1;break;
          }
        }
      }
      c++;
      if(c>1000){
        return -1;
      }
    }
    d = 1;
    //printf("%03d,  %03d,   %03d,  %03d\n", rooms[i].xPos, rooms[i].yPos, rooms[i].width, rooms[i].height);
  }
  //printf("-------------------------\n");
  return 0;
}


void drawMap(uint8_t map[mapHeight][mapWidth], char map_c[mapHeight][mapWidth], room_t *rooms, uint8_t numRooms){
  int i, j, r;

  for(i = 0; i < mapHeight; i++){
    for(j = 0; j < mapWidth; j++){
      if(map[i][j] == 0x00)
        map_c[i][j] = '#';
    }
  }

  for(r = 0; r < numRooms; r++){

    for(j = rooms[r].yPos; j <= rooms[r].yPos + rooms[r].height; j++){
      for(i = rooms[r].xPos; i <= rooms[r].xPos + rooms[r].width; i++){
        map[j][i] = 0x00;
        map_c[j][i] = '.';
      }
    }
  }
}

void place_stairs(uint8_t downstair[2], uint8_t upstair[2], room_t *rooms, uint8_t numRooms){
  room_t *r;

  if(upstair){
    r = rooms + (rand() % numRooms);
    upstair[1] = rand() % r->width + r->xPos;
    upstair[0] = rand() % r->height + r->yPos;
  }
  if(downstair){
    r = rooms + (rand() % numRooms);
    downstair[1] = rand() % r->width + r->xPos;
    downstair[0] = rand() % r->height + r->yPos;
  }
}

void connect_rooms(uint8_t map[mapHeight][mapWidth], room_t *rooms, uint8_t numRooms){
  int r, p1x, p1y, p2x, p2y;
  path_t p[numRooms];
  path_node_t *pn;

  for(r = 0; r < numRooms; r++){
    p1x = (2*rooms[r].xPos + rooms[r].width)/2;
    p1y = (2*rooms[r].yPos + rooms[r].height)/2;
    p2x = (2*rooms[(r+1)%numRooms].xPos + rooms[(r+1)%numRooms].width)/2;
    p2y = (2*rooms[(r+1)%numRooms].yPos + rooms[(r+1)%numRooms].height)/2;
    p[r] = find_shortest_path(map, p1x, p1y, p2x, p2y);
  }
  for(r = 0; r< numRooms; r++){
    pn = p[r].start;
    while(pn){
      map[pn->y][pn->x] = 0;
      pn = pn->next;
    }
    deletePath(p[r]);
  }
}

void generateHardness(uint8_t map[][mapWidth]){
  int x, y;

  for(y = 0; y < mapHeight; y++){
    for(x = 0; x < mapWidth; x++){
      if(x==0 || y==0 || x==mapWidth-1 || y==mapHeight-1){
        map[y][x] = 0xFF;
      }
      else{
        map[y][x] = (uint8_t)(rand() % 253 + 1);
      }
    }
  }
}


void init_map_char(char map_char[mapHeight][mapWidth]){
  int i, j;

  for(j = 0; j < mapHeight; j++){
    for(i = 0; i < mapWidth; i++){
      map_char[j][i] = ' ';
    }
  }
}


void printMap(char map[mapHeight][mapWidth]){
  int j;

  for(j = 0; j < mapHeight; j++){
    printf("%.*s\n", mapWidth, map[j]);
  }
  printf("\n");
}

void printDistances(uint16_t distances[mapHeight][mapWidth], uint8_t hardness[mapHeight][mapWidth], uint8_t x, uint8_t y, char sym){
  int j, i;
  char c[mapHeight][mapWidth], temp;

  for(i = 0; i<mapHeight; i++){
    for(j = 0; j<mapWidth; j++){
      temp = (char)(distances[i][j]%10 + '0');
      if(hardness[i][j]==0){
        if(i == y && j == x){
          printf("%c", sym);
        } else{
          printf("%c", temp);
        }
      } else{
        printf(" ");
      }
      c[i][j] = temp;
    }
      printf("\n");
  }

  c[y][x] = sym;

  printf("\n");

  for(i = 0; i<mapHeight; i++){
    printf("%.*s\n", mapWidth,  c[i]);
  }
}


int generateMap(uint8_t map_hard[mapHeight][mapWidth], char map_char[mapHeight][mapWidth], room_t **rooms, uint8_t *room_count){

  //prevent valgrind errors
  memset(map_hard, mapHeight*mapWidth, sizeof(**map_hard));

  //assigne characters
  init_map_char(map_char);
  //assigns hardness value to all areas in the map
  generateHardness(map_hard);

  //generates all the rooms
  *room_count = rand()% Room_Number_Range + Room_Min_Number;
  int numRooms = *room_count;
  if(!(*rooms =(room_t *) calloc( numRooms, sizeof (**rooms)))){
      return -1;
  }
  if(generateRooms(*rooms, numRooms)){
    printf("could not generate Rooms");
    return -1;
  }
  //set the hardness of map array to 0 to create paths between rooms
  connect_rooms(map_hard, *rooms, numRooms);
  drawMap(map_hard, map_char, *rooms, numRooms);
  return 0;
}

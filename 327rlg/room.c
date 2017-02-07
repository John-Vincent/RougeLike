#include "room.h"
#include <stdlib.h>
#include <stdio.h>

int generateRooms(room_t *rooms, int numRooms){
  int i, j, d = 1, distx, disty, c;


  for(i = 0; i<numRooms; i++){
    rooms[i].width = rand() % 20 + 7;
    rooms[i].height = rand() % 15 + 5;

    //while the room has not been place try to place the room
    while(d){
      //x and y position to test
      rooms[i].xPos = rand() % 151 + 1;
      rooms[i].yPos = rand() % 98 + 1;
      while(rooms[i].xPos + rooms[i].width > 158 || rooms[i].yPos + rooms[i].height > 103){
        rooms[i].xPos = rand() % 151 + 1;
        rooms[i].yPos = rand() % 98 + 1;
      }

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
        return 1;
      }
    }
    d = 1;
    printf("%d, %d, %d, %d\n", rooms[i].xPos, rooms[i].yPos, rooms[i].width, rooms[i].height);
  }
  return 0;
}


void drawMap(int map[mapHeight][mapWidth], char map_c[mapHeight][mapWidth], room_t *rooms, int numRooms){
  int i, j, r;

  for(i = 0; i < mapHeight; i++){
    for(j = 0; j < mapWidth; j++){
      if(map[i][j] == 0)
        map_c[i][j] = '#';
    }
  }

  for(r = 0; r < numRooms; r++){

    for(j = rooms[r].yPos; j <= rooms[r].yPos + rooms[r].height; j++){
      for(i = rooms[r].xPos; i <= rooms[r].xPos + rooms[r].width; i++){
        map[j][i] = 0;
        map_c[j][i] = '.';
      }
    }
  }
}

void connect_rooms(int map[mapHeight][mapWidth], room_t *rooms, int numRooms){
  int r, p1x, p1y, p2x, p2y, i, j, s, sy, ny1, ny2;
  float m;

  for(r = 0; r < numRooms; r++){
    p1x = (2*rooms[r].xPos + rooms[r].width)/2;
    p1y = (2*rooms[r].yPos + rooms[r].height)/2;
    p2x = (2*rooms[(r+1)%numRooms].xPos + rooms[(r+1)%numRooms].width)/2;
    p2y = (2*rooms[(r+1)%numRooms].yPos + rooms[(r+1)%numRooms].height)/2;
    m = (float)(p1y - p2y) / (float)(p1x - p2x);
    s = 1;
    if(p1x>p2x){
      s = -1;
    }
    i = p1x;
    while(i != p2x){
      ny1 = (int)(m*(i-p1x)+p1y);
      ny2 = (int)(m*(i-p1x+s)+p1y);
      map[ny1][i] = 0;
      sy = 1;
      if(ny1>ny2){
        sy = -1;
      }
      j = ny1;
      while(j!=ny2+sy){
        map[j][i] = 0;
        j+=sy;
      }
      i+=s;
    }
  }

}

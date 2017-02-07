#include "path_finder.h"


path_t find_shortest_path(uint8_t hardness[mapHeight][mapWidth], uint8_t start_x, uint8_t start_y, uint8_t end_x, uint8_t end_y){

  list_t unvisited;
  vertex_t nodes[mapHeight-2][mapWidth-2];

  initNodes(nodes);
  unvisited.top = &nodes[start_x-1][start_y-1];
  unvisited.top->dist = 0;



}


void initNodes(vertex_t nodes[mapHeight][mapWidth]){
  int i, j;
  for(i = 0; i < mapHeight-2; i++){
    for(j = 0; j < mapWidth-2; j++){
      nodes[i][j].hard = &hardness[i+1][j+1];
      nodes[i][j].dist = -1;
      if(i > 0){
        nodes[i][j].up = &nodes[i-1][j];
      }
      else {
        nodes[i][j].up = NULL;
      }
      if(i < mapHeight-3){
        nodes[i][j].down = &nodes[i+1][j];
      }
      else {
        nodes[i][j].down = NULL;
      }
      if(j > 0){
        nodes[i][j].left = &nodes[i][j-1];
      }
      else {
        nodes[i][j].down = NULL;
      }
      if(j < mapWidth-3){
        nodes[i][j].right = &nodes[i][j+1];
      }
      else {
        nodes[i][j].right = NULL;
      }
    }
  }
}

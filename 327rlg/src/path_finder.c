#include <stdlib.h>
#include <stdio.h>
#include "../headers/path_finder.h"
#include "../headers/map.h"
#include "../headers/heap.h"

typedef struct vertex{
  uint8_t y;
  uint8_t x;
  uint8_t hard;
  int32_t dist;
  void *heap;
  struct vertex *up;
  struct vertex *down;
  struct vertex *left;
  struct vertex *right;
  struct vertex *prev;
} vertex_t;


static int32_t cmp_vertex(void *a, void *b);

static int initNodes(heap_t *h, vertex_t nodes[mapHeight][mapWidth], uint8_t hardness[mapHeight][mapWidth], uint8_t tunnel);

#define turn_bias 30

path_t find_shortest_path(uint8_t hardness[mapHeight][mapWidth], uint8_t start_x, uint8_t start_y, uint8_t end_x, uint8_t end_y){

  vertex_t nodes[mapHeight][mapWidth];
  vertex_t *u;
  heap_t *h;
  path_t p;
  path_node_t *pn, *n;

  int32_t alt;

  init_heap(&h, cmp_vertex);
  initNodes(h, nodes, hardness, 1);
  u = &nodes[start_y][start_x];
  n = NULL;
  u->dist = 0;
  h->update(h, u->heap);

  while(h->peek(h)){
    u = (vertex_t*)(h->pop(h));
    if(u->x == end_x && u->y == end_y){
      break;
    }
    if(u->left){
      alt = u->dist + turn_bias + u->left->hard;
      if(u->left->dist == -1 || alt < u->left->dist){
        u->left->dist = alt;
        h->update(h, u->left->heap);
        u->left->prev = u;
      }
    }
    if(u->right){
      alt = u->dist + turn_bias + u->right->hard;
      if(u->right->dist == -1 || alt < u->right->dist){
        u->right->dist = alt;
        h->update(h, u->right->heap);
        u->right->prev = u;
      }
    }
    if(u->up){
      alt = u->dist + turn_bias + u->up->hard;
      if(u->up->dist == -1 || alt < u->up->dist){
        u->up->dist = alt;
        h->update(h, u->up->heap);
        u->up->prev = u;
      }
    }
    if(u->down){
      alt = u->dist + turn_bias + u->down->hard;
      if(u->down->dist == -1 || alt < u->down->dist){
        u->down->dist = alt;
        h->update(h, u->down->heap);
        u->down->prev = u;
      }
    }
  }

  h->clear(h);

  p.dist = u->dist;
  while(u){
    pn = calloc(1, sizeof(*pn));
    pn->x = u->x;
    pn->y = u->y;
    pn->next = n;
    if(n){
      n->prev = pn;
    }
    n = pn;
    u = u->prev;
  }

  p.start = pn;
  return p;
}

void deletePath(path_t p){
  path_node_t *pn;
  pn = p.start;
  if(!pn){
    return;
  }
  while(pn->next){
    pn = pn->next;
    free(pn->prev);
  }
  free(pn);
}

static int32_t cmp_vertex(void *a, void *b){
  int32_t ai = ((vertex_t*)a)->dist;
  int32_t bi = ((vertex_t*)b)->dist;
  if(ai == -1){
    return -1;
  }
  if(bi == -1){
    return 1;
  }
  return bi - ai;
}


static int initNodes(heap_t *h, vertex_t nodes[mapHeight][mapWidth], uint8_t hardness[mapHeight][mapWidth], uint8_t tunnel){
  int i, j;
  for(i = 0; i < mapHeight; i++){
    for(j = 0; j < mapWidth; j++){
      nodes[i][j].hard = hardness[i][j];
      nodes[i][j].heap = NULL;
      if(!hardness[i][j]||tunnel){
        nodes[i][j].x = j;
        nodes[i][j].y = i;
        nodes[i][j].dist = -1;
        nodes[i][j].heap = h->insert(h, &nodes[i][j]);
        if(!nodes[i][j].heap){
          printf("failed to allocated memory");
          return -1;
        }
        nodes[i][j].prev = NULL;
        if(i > 0 && (tunnel || !hardness[i-1][j])){
          nodes[i][j].up = &nodes[i-1][j];
        }
        else {
          nodes[i][j].up = NULL;
        }
        if(i < mapHeight-1 && (tunnel || !hardness[i+1][j])){
          nodes[i][j].down = &nodes[i+1][j];
        }
        else {
          nodes[i][j].down = NULL;
        }
        if(j > 0 && (tunnel || !hardness[i][j-1])){
          nodes[i][j].left = &nodes[i][j-1];
        }
        else {
          nodes[i][j].left = NULL;
        }
        if(j < mapWidth-1 && (tunnel || !hardness[i][j+1])){
          nodes[i][j].right = &nodes[i][j+1];
        }
        else {
          nodes[i][j].right = NULL;
        }
      }
    }
  }
  return 0;
}


int find_distances(uint8_t hardness[mapHeight][mapWidth], uint16_t distances[mapHeight][mapWidth], uint8_t start_x, uint8_t start_y, uint8_t intel, uint8_t tunnel){

  vertex_t nodes[mapHeight][mapWidth];
  vertex_t *u;
  heap_t *h;

  int16_t alt;

  init_heap(&h, cmp_vertex);
  if(initNodes(h, nodes, hardness, tunnel)){
    return -1;
  }
  u = &nodes[start_y][start_x];
  u->dist = 0;
  h->update(h, u->heap);

  while(h->peek(h)){
    u = (vertex_t*)(h->pop(h));
    distances[u->y][u->x] = u->dist;
    if(u->left){
      alt = u->dist + 1;
      if(u->left->hard > 84 && intel){
        alt++;
        if(u->left->hard > 170 && intel){
          alt++;
          if(u->left->hard == 255){
            alt = 0x7FF7;
          }
        }
      }
      if(u->left->dist == -1 || alt < u->left->dist){
        u->left->dist = alt;
        h->update(h, u->left->heap);
      }
    }
    if(u->right){
      alt = u->dist + 1;
      if(u->right->hard > 84 && intel){
        alt++;
        if(u->right->hard > 170 && intel){
          alt++;
          if(u->right->hard == 255){
            alt = 0x7FF7;
          }
        }
      }
      if(u->right->dist == -1 || alt < u->right->dist){
        u->right->dist = alt;
        h->update(h, u->right->heap);
      }
    }
    if(u->up){
      alt = u->dist + 1;
      if(u->up->hard > 84 && intel){
        alt++;
        if(u->up->hard > 170 && intel){
          alt++;
          if(u->up->hard == 255){
            alt = 0x7FF7;
          }
        }
      }
      if(u->up->dist == -1 || alt < u->up->dist){
        u->up->dist = alt;
        h->update(h, u->up->heap);
      }
    }
    if(u->down){
      alt = u->dist + 1;
      if(u->down->hard > 84 && intel){
        alt++;
        if(u->down->hard > 170 && intel){
          alt++;
          if(u->down->hard == 255){
            alt = 0x7FF7;
          }
        }
      }
      if(u->down->dist == -1 || alt < u->down->dist){
        u->down->dist = alt;
        h->update(h, u->down->heap);
      }
    }
  }
  h->clear(h);

  return 0;
}

int can_see(uint8_t hardness[mapHeight][mapWidth], int x1, int y1, int x2, int y2){
  int i, j;
  float m, e;

  if(x1-x2==0){
    if(y1 > y2){
      i = y2+1;
      j = y1;
    } else{
      i = y1+1;
      j = y2;
    }
    for(; i < j; i++){
      if(hardness[i][x1] != 0)
        return 0;
    }
    return 1;
  }

  m = (float)(y1 - y2) / (float)(x1 - x2);

  if(m<0){
    m = -1 * m;
  }
  e = m - 0.5;
  if(x1 > x2){
    i = -1;
  }else{
    i = 1;
  }

  for(; x1 != x2; x1+=i){
    if(hardness[y1][x1]!=0){
      return 0;
    }
    e += m;
    if(e >= 0.5){
      y1++;
      e -= 1;
    }
  }

  return 1;
}

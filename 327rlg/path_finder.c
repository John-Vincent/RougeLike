#include "path_finder.h"

static int32_t cmp_vertex(void *a, void *b);

static void initNodes(heap_t *h, vertex_t nodes[mapHeight-2][mapWidth-2], uint8_t hardness[mapHeight][mapWidth]);

path_t find_shortest_path(uint8_t hardness[mapHeight][mapWidth], uint8_t start_x, uint8_t start_y, uint8_t end_x, uint8_t end_y){

  vertex_t nodes[mapHeight-2][mapWidth-2];
  vertex_t *u;
  heap_t *h;
  path_t p;
  path_node_t *pn, *n;

  int32_t alt;

  init_heap(&h, cmp_vertex);
  initNodes(h, nodes, hardness);
  u = &nodes[start_y-1][start_x-1];
  n = NULL;
  u->dist = 0;
  h->update(h, u->heap);

  while(h->top){
    u = (vertex_t*)(h->pop(h));
    if(u->x == end_x && u->y == end_y){
      break;
    }
    if(u->left){
      alt = u->dist + 50 + u->left->hard;
      if(u->left->dist == -1 || alt < u->left->dist){
        u->left->dist = alt;
        h->update(h, u->left->heap);
        u->left->prev = u;
      }
    }
    if(u->right){
      alt = u->dist + 50 + u->right->hard;
      if(u->right->dist == -1 || alt < u->right->dist){
        u->right->dist = alt;
        h->update(h, u->right->heap);
        u->right->prev = u;
      }
    }
    if(u->up){
      alt = u->dist + 50 + u->up->hard;
      if(u->up->dist == -1 || alt < u->up->dist){
        u->up->dist = alt;
        h->update(h, u->up->heap);
        u->up->prev = u;
      }
    }
    if(u->down){
      alt = u->dist + 50 + u->down->hard;
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


static void initNodes(heap_t *h, vertex_t nodes[mapHeight-2][mapWidth-2], uint8_t hardness[mapHeight][mapWidth]){
  int i, j;
  for(i = 0; i < mapHeight-2; i++){
    for(j = 0; j < mapWidth-2; j++){
      nodes[i][j].hard = hardness[i+1][j+1];
      nodes[i][j].x = j+1;
      nodes[i][j].y = i+1;
      nodes[i][j].dist = -1;
      nodes[i][j].heap = h->insert(h, &nodes[i][j]);
      nodes[i][j].prev = NULL;
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

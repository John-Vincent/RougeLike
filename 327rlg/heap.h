#ifndef HEAP_H
#define HEAP_H

#include <stdint.h>
#include <stdlib.h>

typedef struct heap_node{
  struct heap_node *parent;
  struct heap_node *child[2];
  uint32_t children;
  void *data;
} heap_node_t;

typedef struct heap{
  heap_node_t *top;
  int32_t (*compare)(void*, void*);
  void *(*pop)(struct heap*);
  heap_node_t *(*insert)(struct heap*, void*);
  void (*clear)(struct heap*);
  void (*update)(struct heap*, heap_node_t*);
} heap_t;

void init_heap(heap_t **h, int32_t (*compare)(void*, void*));


#endif

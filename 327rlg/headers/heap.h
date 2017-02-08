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

/**
 * allocated memory for a heap and puts it into h, then sets all the function pointers for the heap
 * @param  h                       pointer to the heap pointer that needs to be allocated
 * @param  compare                 function that will be used to compare the data stored in the heap
 * @author Collin Vincent <collinvincent96@gmail.com>
 * @date   2017-02-08T10:51:43+000
 */
void init_heap(heap_t **h, int32_t (*compare)(void*, void*));


#endif

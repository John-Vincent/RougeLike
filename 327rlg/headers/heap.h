#ifndef HEAP_H
#define HEAP_H

#include <stdint.h>

#ifdef __cplusplus
  extern "C" {
#endif

typedef struct heap{
  struct heap_node *top;
  int32_t (*compare)(void*, void*);
  void *(*pop)(struct heap*);
  struct heap_node *(*insert)(struct heap*, void*);
  void *(*peek)(struct heap*);
  void (*clear)(struct heap*);
  void (*update)(struct heap*, struct heap_node*);
} heap_t;

/**
 * allocated memory for a heap and puts it into h, then sets all the function pointers for the heap
 * @param  h                       pointer to the heap pointer that needs to be allocated
 * @param  compare                 function that will be used to compare the data stored in the heap
 * @author Collin Vincent <collinvincent96@gmail.com>
 * @date   2017-02-08T10:51:43+000
 */
void init_heap(heap_t **h, int32_t (*compare)(void*, void*));

void clear_heap(heap_t *h);

#ifdef __cplusplus
  }
#endif

#endif

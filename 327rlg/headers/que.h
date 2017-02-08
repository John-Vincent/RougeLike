#ifndef QUE_H
#define QUE_H

#include <stdint.h>
#include <stdlib.h>


typedef struct que_link{
  struct que_link *next;
  struct que_link *prev;
  void *data;
} que_link_t;

typedef struct que{
  que_link_t *top;
  que_link_t *bottom;
  void (*add)(struct que*, void*);
  void *(*pop)(struct que*);
  uint32_t size;
} que_t;


void init_que(que_t **q);


#endif

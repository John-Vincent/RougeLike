#include "que.h"

#define TESTING_QUE


static void add(que_t *q, void *d){
  que_link_t *n = calloc(1, sizeof (que_link_t));
  n->data = d;
  if(!(q->size)){
    q->bottom = n;
    q->top = n;
  } else{
    q->bottom->next = n;
    n->prev = q->bottom;
    q->bottom = n;
  }
  q->size++;
}

static void *pop(que_t *q){
  if(!q->top){
    return NULL;
  }
  que_link_t *o = q->top;
  if(o->next){
    o->next->prev = NULL;
  }
  q->top = o->next;
  void *t = o->data;
  free(o);
  q->size--;
  return t;
}

void init_que(que_t **q){
  *q = calloc(1, sizeof (**q));
  (*q)->add = &add;
  (*q)->pop = &pop;
}


#ifdef TESTING_QUE

#include <stdio.h>
#include <string.h>

void print_que(que_t *q){
  que_link_t *l = q->top;
  while(l){
    printf("%d ", *((int*)(l->data)) );
    l = l->next;
  }
  printf("\n");
}


int main(int argc, char *argv[]){
  int *t;
  int c = 1;
  char s[20];
  que_t *q = NULL;
  init_que(&q);

  while(c){
    printf("enter command: ");
    scanf("%s", s);
    if(!strcmp(s, "add")){
      t = malloc(sizeof (int));
      printf("enter number: ");
      scanf("%d", t);
      q->add(q, t);
      print_que(q);
    } else if(!strcmp(s, "pop")){
      t = q->pop(q);
      if(t){
        printf("number popped: %d\n", (int)(*t));
        print_que(q);
        free(t);
      }
    } else if(!strcmp(s, "exit")){
      c = 0;
    }
  }
}


#endif

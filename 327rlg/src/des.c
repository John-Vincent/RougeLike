#include <unistd.h>
#include <curses.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include "../headers/des.h"
#include "../headers/map.h"
#include "../headers/heap.h"
#include "../headers/path_finder.h"
#include "../headers/character_cpp.h"


int run_game(dungeon_t *base){

  int i, input, running = 1;
  dungeon_t *dungeon = base;
  character *c;
  heap_t *h;
  char temp[100] = " ";

  dungeon->look = 0;

  init_heap(&(dungeon->turn_order), compare_characters);
  h = dungeon->turn_order;
  for(i = 0; i < dungeon->num_characters; i++){
    h->insert(h, dungeon->characters[i]);
  }

  initscr();
  start_color();
  init_pair(1, COLOR_WHITE, COLOR_BLACK);
  init_pair(2, COLOR_GREEN, COLOR_BLACK);
  init_pair(3, COLOR_RED, COLOR_BLACK);
  init_pair(4, COLOR_YELLOW, COLOR_BLACK);
  cbreak();
  noecho();
  curs_set(0);

  display(dungeon);

  while(running){
    input = getch();
    switch(input){
      // L
      case 76:
        dungeon->look = 1;
        break;
      // escape
      case 27:
        dungeon->look = 0;
        set_dungeon_view(dungeon);
        break;
      // Q
      case 81:
        running = 0;
        break;
      // upstairs
      case '<':
        if(on_stairs(dungeon) == 1){
          if(dungeon->upstairs == NULL){
            if(init_dungeon( &(dungeon->upstairs), dungeon->num_characters)){
              running = false;
              break;
            }
            dungeon->upstairs->downstairs = dungeon;
            //for connected stairs
            //dungeon->chars[dungeon->player->y][dungeon->player->x] = '<';
          }
          dungeon = dungeon->upstairs;
          h = dungeon->turn_order;
        }
        break;
      // downstairs
      case '>':
        if(on_stairs(dungeon) == -1){
          if(dungeon->downstairs == NULL){
            if(init_dungeon( &(dungeon->downstairs), dungeon->num_characters)){
              running = false;
              break;
            }
            dungeon->downstairs->upstairs = dungeon;
            //for connected stairs
            //dungeon->chars[dungeon->player->y][dungeon->player->x] = '<';
          }
          dungeon = dungeon->downstairs;
          h = dungeon->turn_order;
        }
        break;
    }
    if(!dungeon->look && !is_dead(dungeon->characters[0]) && running){
      sprintf(temp, "input = %d, no action taken", input);
      dungeon->message = temp;
      if(!take_turn(dungeon->characters[0], input)){
        c = (character *)h->pop(h);
        set_turn(c);
        h->insert(h, c);

        while(h->peek(h) != dungeon->characters[0]){
          c = (character *)h->pop(h);
          if(!is_dead(c)){
            if(take_turn(c, input)){
              dungeon->message = "you lost!";
            }else{
              sprintf(temp, "input = %d, player life = %d", input, 1);
            }
            set_turn(c);
            h->insert(h, c);
          }
        }
      }
      display(dungeon);
    } else if(running){
      if(is_dead(dungeon->characters[0])){
        dungeon->message = "you lost!";
        dungeon->look = 1;
      }else{
        dungeon->message = "LOOK MODE";
      }
      take_turn(dungeon->characters[0], input);
      display(dungeon);
    }
  }

  endwin();
  return 0;
}


void display(dungeon_t *dungeon){
  int i, j, x, y;
  char symbol;

  clear();

  x = dungeon->x - screen_width/2;
  y = dungeon->y - screen_height/2;

  if(x<0)
    x = 0;
  else if(x>mapWidth - screen_width)
    x = mapWidth - screen_width;

  if(y<-1)
    y = -1;
  else if(y>mapHeight- screen_height - 1)
    y = mapHeight- screen_height - 1;

  for(j = 1; j< screen_height + 1; j++){
    for(i = 0; i< screen_width; i++){
      symbol = get_char(dungeon, x+i, y+j);
      if(symbol){
        if(symbol == '@')
          attron(COLOR_PAIR(2));
        else
          attron(COLOR_PAIR(3));
        mvprintw(j, i, "%c", symbol);
      } else if(dungeon->visited[y+j][x+i] == 1){
        if(dungeon->chars[y+j][x+i] == '<' || dungeon->chars[y+j][x+i] == '>')
          attron(COLOR_PAIR(4));
        else
          attron(COLOR_PAIR(1));
        mvprintw(j, i, "%c", dungeon->chars[y+j][x+i]);
      }
    }

  }
  for(i = 0; i< screen_width; i++){
    mvprintw(0, i, " ");
  }
  if(dungeon->message){
    mvprintw(0, screen_width/2 - strlen(dungeon->message)/2, "%s", dungeon->message);
  }
  refresh();
  dungeon->message = NULL;
}

#include <unistd.h>
#include <curses.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include "../headers/des.h"
#include "../headers/map.h"
#include "../headers/heap.h"
#include "../headers/path_finder.h"
#include "../headers/character"


int run_game(){

  int input, running = 1;
  Dungeon *dungeon = Dungeon::get_instance();
  character *c;
  heap_t *h;
  char temp[100] = " ";

  h = dungeon->get_turns();

  initscr();
  start_color();
  init_pair(1, COLOR_WHITE, COLOR_BLACK);
  init_pair(2, COLOR_GREEN, COLOR_BLACK);
  init_pair(3, COLOR_RED, COLOR_BLACK);
  init_pair(4, COLOR_YELLOW, COLOR_BLACK);
  cbreak();
  noecho();
  curs_set(0);

  display();

  while(running){
    input = getch();
    switch(input){
      // L
      case 76:
        dungeon->get_look() = 1;
        break;
      // escape
      case 27:
        dungeon->get_look() = 0;
        dungeon->center_map();
        break;
      // Q
      case 81:
        running = 0;
        break;
      // upstairs
      case '<':
        if(dungeon->go_upstairs()){
          h = dungeon->get_turns();
        }
        break;
      // downstairs
      case '>':
        if(dungeon->go_downstairs()){
          h = dungeon->get_turns();
        }
        break;
    }
    if(!dungeon->get_look() && !dungeon->get_character(0)->is_dead() && running){
      sprintf(temp, "input = %d, no action taken", input);
      dungeon->get_message() = temp;
      if(!dungeon->get_character(0)->take_turn(input)){
        c = (character *)h->pop(h);
        c->next_turn += (turnbias / c->speed);
        h->insert(h, c);

        while(h->peek(h) != dungeon->get_character(0)){
          c = (character *)h->pop(h);
          if(!c->is_dead()){
            if(c->take_turn(input)){
              dungeon->get_message() = (char*)"you lost!";
            }else{
              sprintf(temp, "input = %d, player life = %d", input, 1);
            }
            c->next_turn += (turnbias / c->speed);
            h->insert(h, c);
          }
        }
      }
      display();
    } else if(running){
      if(dungeon->get_character(0)->is_dead()){
        dungeon->get_message() = (char *)"you lost!";
        dungeon->get_look() = 1;
      }else{
        dungeon->get_message() = (char *)"LOOK MODE";
      }
      dungeon->get_character(0)->take_turn(input);
      display();
    }
  }

  endwin();
  return 0;
}


void display(){
  int i, j, x, y;
  char symbol;
  Dungeon *dungeon;
  dungeon = Dungeon::get_instance();

  clear();

  x = dungeon->get_x() - screen_width/2;
  y = dungeon->get_y() - screen_height/2;

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
      symbol = dungeon->get_char_sym(x+i, y+j);
      if(symbol){
        if(symbol == '@')
          attron(COLOR_PAIR(2));
        else
          attron(COLOR_PAIR(3));
        mvprintw(j, i, "%c", symbol);
      } else if(dungeon->visited(x+i, y+j) == 1){
        if(dungeon->get_environment(x+i, y+j) == '<' || dungeon->get_environment(x+i, y+j) == '>')
          attron(COLOR_PAIR(4));
        else if(dungeon->visible(x+i, y+j)){
          attron(COLOR_PAIR(1));
        }else{
          attron(COLOR_PAIR(1));
          attroff(A_BOLD);
        }
        mvprintw(j, i, "%c", dungeon->get_environment(x+i, y+j));
      }
      attron(A_BOLD);
    }
  }

  for(i = 0; i< screen_width; i++){
    mvprintw(0, i, " ");
  }
  if(dungeon->get_message()){
    mvprintw(0, screen_width/2 - strlen(dungeon->get_message())/2, "%s", dungeon->get_message());
  }
  refresh();
  dungeon->get_message() = NULL;
}

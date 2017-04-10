#include <unistd.h>
#include <curses.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <iostream>
#include <string>
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
  std::string str = "";

  h = dungeon->get_turns();

  initscr();
  start_color();
  init_pair(1, COLOR_WHITE, COLOR_BLACK);
  init_pair(2, COLOR_GREEN, COLOR_BLACK);
  init_pair(3, COLOR_RED, COLOR_BLACK);
  init_pair(4, COLOR_YELLOW, COLOR_BLACK);
  init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(6, COLOR_CYAN, COLOR_BLACK);
  init_pair(7, COLOR_BLUE, COLOR_BLACK);
  init_pair(8, COLOR_BLACK, COLOR_WHITE);
  cbreak();
  noecho();
  curs_set(0);

  display();

  while(running){
    input = getch();
    switch(input){
      // L
      case 'L':
        dungeon->set_mode(LOOK);
        break;
      // escape
      case 27:
        dungeon->set_mode(CONTROL);
        dungeon->set_message("");
        dungeon->set_display1("");
        dungeon->set_display2(""); 
        dungeon->center_map();
        break;
      // Q
      case 'Q':
        running = 0;
        break;
      // upstairs
      case '<':
        if(dungeon->get_mode() == CONTROL && dungeon->go_upstairs()){
          h = dungeon->get_turns();
        }
        break;
      // downstairs
      case '>':
        if(dungeon->get_mode() == CONTROL && dungeon->go_downstairs()){
          h = dungeon->get_turns();
        }
        break;
      case 'w':
        dungeon->set_mode(WEAR);
        break;
      case 'd':
        dungeon->set_mode(DROP);
        break;
      case 't':
        dungeon->set_mode(TAKE);
        break;
      case 'x':
        dungeon->set_mode(EX);
        break;
      case 'i':
        dungeon->set_mode(INVENTORY);
        break;
      case 'e':
        dungeon->set_mode(EQUIPMENT);
        break;
    }
    if(dungeon->get_mode() == CONTROL && !dungeon->get_player()->is_dead() && running){
      if(!dungeon->get_player()->take_turn(input)){
        c = (character *)h->pop(h);
        h->insert(h, c);
        dungeon->set_display2(str);
        while(h->peek(h) != dungeon->get_player()){
          c = (character *)h->pop(h);
          if(!c->is_dead()){
            c->take_turn(input);
            h->insert(h, c);
          } else{
            delete c;
          }
        }
        sprintf(temp, "input = %c, player life = %d", (char)input, dungeon->get_life());
        dungeon->get_message() += temp;
      }
    } else if(running && dungeon->get_mode() == LOOK){
      if(dungeon->get_player()->is_dead()){
        dungeon->set_mode(LOOK);
      }else{
        dungeon->get_message() = (char *)"LOOK MODE";
      }
      dungeon->get_player()->take_turn(input);
    } else if(running && dungeon->get_mode() == WEAR){
      dungeon->equip(input);
    } else if(running && dungeon->get_mode() == TAKE){
      dungeon->unequip(input);
    } else if(running && dungeon->get_mode() == DROP){
      dungeon->drop(input);
    } else if(running && dungeon->get_mode() == EX){
      dungeon->destroy(input);
    } else if(running && dungeon->get_mode() == INVENTORY){
      dungeon->inventory(input);
    } else if(running && dungeon->get_mode() == EQUIPMENT){
      dungeon->equipment(input);
    }
    display();
  }

  endwin();
  return 0;
}


void display(){
  int i, j, x, y, mode;
  char symbol;
  std::string str, str2, desc;
  Dungeon *dungeon;
  character *c;
  item *it;
  dungeon = Dungeon::get_instance();

  clear();

  x = dungeon->get_x();
  y = dungeon->get_y();
  str = dungeon->get_display1();
  str2 = dungeon->get_display2();
  c = dungeon->get_character(x, y);
  it = dungeon->get_item(x, y);

  mode = dungeon->get_mode();

  if(mode == CONTROL || mode == LOOK){

    attron(COLOR_PAIR(WHITE));
    attron(A_BOLD);
    if( str.length() || str2.length() ){
      mvprintw(screen_height + 1, screen_width/2 - str.length()/2, "%s", str.c_str());
      mvprintw(screen_height + 2, screen_width/2 - str2.length()/2, "%s", str2.c_str());
    } else if(c && c != dungeon->get_player()){
      desc = c->get_desc();
      mvprintw(screen_height + 1, 0, "%s", desc.c_str());
    } else if(it){
      desc = it->get_desc();
      mvprintw(screen_height + 1, 0, "%s", desc.c_str());
    }

    x -= screen_width / 2;
    y -= screen_height / 2;
    if(x<0)
      x = 0;
    else if(x>mapWidth - screen_width)
      x = mapWidth - screen_width;

    if(y<-1)
      y = -1;
    else if(y>mapHeight- screen_height - 1)
      y = mapHeight - screen_height - 1;

    for(j = 1; j< screen_height + 1; j++){
      for(i = 0; i< screen_width; i++){
        symbol = dungeon->for_print(x+i, y+j);
        mvprintw(j, i, "%c", symbol);
      }
    }
  } else{
    attron(A_BOLD);
    x = 0;
    if(mode == EQUIPMENT || mode == TAKE){
      j = screen_width/2 - 36;
      for(i = 0; i < 12; i++){
        it = dungeon->get_equipment(i);
        if(i < 10){
          mvprintw(screen_height + 2, j, "%d:", i);
          j+=2;
        } else if(i == 10){
          mvprintw(screen_height + 2, j, "-:");
          j+=2;
        } else if(i == 11){
          mvprintw(screen_height + 2, j, "=:");
          j+=2;
        }
        if(i == dungeon->get_selected()){
          x = i;
          mvprintw(screen_height + 2, j, "[");
          j+=1;
        }else{
          mvprintw(screen_height + 2, j, " ");
          j+=1;
        }
        if(it){
          attron(COLOR_PAIR(it->get_color()));
          mvprintw(screen_height + 2, j, "%c", it->get_symb());
          j+=1;
          attron(COLOR_PAIR(WHITE));
        }else{
          mvprintw(screen_height + 2, j, " ");
          j+=1;
        }
        if(i == dungeon->get_selected()){
          mvprintw(screen_height + 2, j, "]");
          j+=1;
        }else{
          mvprintw(screen_height + 2, j, " ");
          j+=1;
        }
        mvprintw(screen_height + 2, j, " ");
        j++;
      }
      it = dungeon->get_equipment(dungeon->get_selected());
    } else{
      j = screen_width/2 - 30;
      for(i = 0; i < bag_size; i++){
        it = dungeon->get_player()->bag[i];
        attron(COLOR_PAIR(WHITE));
        if(dungeon->get_selected() == i){
          mvprintw(screen_height + 2, j, "%d:[", i);
          j+=3;
        } else{
          mvprintw(screen_height + 2, j, "%d: ", i);
          j+=3;
        }
        if(it){
          attron(COLOR_PAIR(it->get_color()));
          mvprintw(screen_height + 2, j, "%c", it->get_symb());
          j+=1;
        }else{
          mvprintw(screen_height + 2, j, " ");
          j+=1;
        }
        if(dungeon->get_selected() == i){
          attron(COLOR_PAIR(WHITE));
          mvprintw(screen_height + 2, j, "]");
          j+=1;
        }else{
          mvprintw(screen_height + 2, j, " ");
          j++;
        }
        mvprintw(screen_height + 2, j, " ");
        j+=1;
      }
      it = dungeon->get_player()->bag[dungeon->get_selected()];
    }
    if(it){
      str = it->get_name();
      attron(COLOR_PAIR(it->get_color()));
      mvprintw(3, screen_width/2 - str.length()/2, "%s", str.c_str());
      attron(COLOR_PAIR(WHITE));
      str = it->get_str_type();
      mvprintw(4, screen_width/2 - str.length()/2, "%s", str.c_str());
      mvprintw(5, 0, "%s", it->get_desc().c_str());
    }
    str = dungeon->get_display1();
    mvprintw(screen_height + 1, screen_width/2 - str.length()/2, "%s", str.c_str());
  }

  mvprintw(0, screen_width/2 - dungeon->get_message().length()/2, "%s", dungeon->get_message().c_str());

  refresh();
  dungeon->get_message() = "";
  dungeon->get_display1() = "";
  dungeon->get_display2() = "";
}

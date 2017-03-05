#include "../headers/des.h"


int npc_moves(dungeon_t *dungeon);

int run_game(dungeon_t *base){

  int i, mode = 0, input, running = 1;
  dungeon_t *dungeon = base;
  character_t *c;
  heap_t *h;
  char temp[100] = " ";

  init_heap(&(dungeon->turn_order), compare_characters);
  h = dungeon->turn_order;
  for(i = 0; i < dungeon->num_characters; i++){
    h->insert(h, dungeon->player + i);
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
        mode = 1;
        break;
      // escape
      case 27:
        mode = 0;
        dungeon->x = dungeon->player->x;
        dungeon->y = dungeon->player->y;
        break;
      // Q
      case 81:
        running = 0;
        break;
      // upstairs
      case '<':
        if(dungeon->chars[dungeon->player->y][dungeon->player->x] == '<'){
          dungeon = dungeon->upstairs;
          h = dungeon->turn_order;
          clear();
        }
        break;
      // downstairs
      case '>':
        if(dungeon->chars[dungeon->player->y][dungeon->player->x] == '>'){
          if(dungeon->downstairs == NULL){
            dungeon->downstairs = calloc(1, sizeof(dungeon_t));
            if(!dungeon->downstairs){
              running = false;
              printf("error allocating downstairs");
            }
            if(generateMap(dungeon->downstairs->hardness, dungeon->downstairs->chars, &(dungeon->downstairs->rooms), &(dungeon->downstairs->numRooms))){
              running = false;
              printf("error making new map");
            }
            dungeon->downstairs->num_characters = 0;
            if(generate_characters(dungeon->downstairs, dungeon->num_characters)){
              running = false;
              printf("error placing new characters");
            }
            dungeon->downstairs->chars[dungeon->downstairs->player->y][dungeon->downstairs->player->x] = '<';
            dungeon->downstairs->upstairs = dungeon;
            init_heap(&(dungeon->downstairs->turn_order), compare_characters);
            h = dungeon->downstairs->turn_order;
            for(i = 0; i < dungeon->downstairs->num_characters; i++){
              h->insert(h, dungeon->downstairs->player + i);
            }
          }
          dungeon = dungeon->downstairs;
          h = dungeon->turn_order;
          dungeon->x = dungeon->player->x;
          dungeon->y = dungeon->player->y;
          clear();
        }
        break;
    }
    if(!mode && dungeon->player->attrib != 0xffffffff){
      sprintf(temp, "input = %d, no action taken", input);
      dungeon->message = temp;
      if(!player_turn(dungeon, mode, input)){
        c = (character_t *)h->pop(h);
        c->next_turn = c->next_turn + turnbias / c->speed;
        h->insert(h, c);
        if(npc_moves(dungeon)){
          dungeon->message = "you lost!";
        }else{
          sprintf(temp, "input = %d, player life = %d", input, dungeon->player->attrib);
        }
      }
      display(dungeon);
    } else{
      if(dungeon->player->attrib == 0xffffffff){
        dungeon->message = "you lost!";
        mode = 1;
      }else{
        dungeon->message = "LOOK MODE";
      }
      player_turn(dungeon, mode, input);
      display(dungeon);
    }
  }

  endwin();
  return 0;
}


int npc_moves(dungeon_t *dungeon){
  character_t *c;
  int died = 0;

  while(dungeon->player != (character_t *)dungeon->turn_order->peek(dungeon->turn_order)){
    c = (character_t *)dungeon->turn_order->pop(dungeon->turn_order);
    if(c->attrib != 0xFFFFFFFF){
      if(take_turn(dungeon, c)){
        died = 1;
      }
      c->next_turn = c->next_turn + turnbias / c->speed;
      dungeon->turn_order->insert(dungeon->turn_order, c);
    }
  }
  return died;
}

void display(dungeon_t *dungeon){
  int i, j, x, y;

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
      if(dungeon->characters[y + j][x + i]){
        if(dungeon->characters[y + j][x + i] == dungeon->player)
          attron(COLOR_PAIR(2));
        else
          attron(COLOR_PAIR(3));
        mvprintw(j, i, "%c", dungeon->characters[y+j][x+i]->sym);
      } else{
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

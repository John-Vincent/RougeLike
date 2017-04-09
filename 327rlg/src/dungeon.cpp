#include "../headers/dungeon"
#include <string.h>
#include <curses.h>

Dungeon *Dungeon::d_instance = NULL;

Dungeon::level::level(int nummon, int up_stairs){

  if(generateMap(this->hardness, this->chars, &(this->rooms), &(this->numRooms), up_stairs)){
    printf("error making new map");
  }

  this->num_characters = nummon + 1;

  memset(this->visited, 0, sizeof (this->visited));
  memset(characters, 0, sizeof(characters));
  memset(items, 0, sizeof(items));

  init_heap(&(this->turn_order), compare_characters);
}

Dungeon::level::~level(){
  unsigned int i, j;

  turn_order->clear(turn_order);

  i = player->x;
  j = player->y;
  if(characters[j][i] == player){
    characters[j][i] = NULL;
  }
  delete player;
  
  for(i = 0; i < mapHeight; i++){
    for(j = 0; j < mapWidth; j++){
      if(characters[i][j]){
        delete characters[i][j];
      }
      if(items[i][j]){
        delete items[i][j];
      }
    }
  }

  free(this->rooms);
}

Dungeon *Dungeon::get_instance(int nummon){
  if(!d_instance)
    d_instance = new Dungeon(nummon);
  return d_instance;
}

Dungeon *Dungeon::get_instance(){
  return d_instance;
}

int Dungeon::go_upstairs(){
  int x, y;
  char env;
  x = curlev->player->x;
  y = curlev->player->y;
  env = curlev->chars[y][x];
  if(env == '<'){
    if(curlev->upstairs){
      curlev = curlev->upstairs;
    }
    return 1;
  }
  return 0;
}

int Dungeon::go_downstairs(){
  int x, y;
  char env;
  x = curlev->player->x;
  y = curlev->player->y;
  env = curlev->chars[y][x];
  if(env == '>'){
    if(!curlev->downstairs){
      curlev->downstairs = new level(curlev->num_characters, 1);
      curlev->downstairs->upstairs = curlev;
      curlev = curlev->downstairs;
      fill_level();
      x = curlev->player->x;
      y = curlev->player->y;
      curlev->chars[y][x] = '<';
    }
    else{
      curlev = curlev->downstairs;
    }
    return 1;
  }
  return 0;
}

void Dungeon::calculate_distances(){
  find_distances(curlev->hardness, curlev->distances, curlev->player->x, curlev->player->y, 0, 1);
  find_distances(curlev->hardness, curlev->distances_intel, curlev->player->x, curlev->player->y, 1, 1);
  find_distances(curlev->hardness, curlev->distances_notun, curlev->player->x, curlev->player->y, 1, 0);
}

Dungeon::~Dungeon(){
  Dungeon::level *level;
  level = top;
  while(level->downstairs){
    level = level->downstairs;
    delete (level->upstairs);
  }
  delete level;
}

int Dungeon::visible(int x, int y){
  int i, j;

  i = curlev->player->x - x;
  j = curlev->player->y - y;

  if(i <= torch_distance && i >= -torch_distance && j <= torch_distance && j >= -torch_distance){
    return 1;
  }
  return 0;
}


void Dungeon::fill_level(){
  unsigned int i, control = num_items;
  int x, y;
  npc *n;
  item *it;

  curlev->player = new pc();

  x = curlev->player->x;
  y = curlev->player->y;

  curlev->characters[y][x] = curlev->player;
  (curlev->turn_order)->insert((curlev->turn_order), curlev->player);

  for(i = 1; i < curlev->num_characters; i++){
    n = mc.get_monster(i);
    (curlev->turn_order)->insert((curlev->turn_order), n);
  }

  for(i = 0; i < control; i++){
    it = ic.get_item();
    x = rand() % mapWidth;
    y = rand() % mapHeight;
    while(curlev->hardness[y][x] || curlev->items[y][x]){
      x = rand() % mapWidth;
      y = rand() % mapHeight;
    }
    curlev->items[y][x] = it;
  }

  curlev->x = curlev->player->x;
  curlev->y = curlev->player->y;
}

char Dungeon::for_print(int x, int y){
  int color, type, vis;
  char symbol;

  vis = visible(x, y);

  if(!curlev->visited[y][x]){
    return ' ';
  }

  if(curlev->characters[y][x] && vis){
    symbol = curlev->characters[y][x]->sym;
    color = curlev->characters[y][x]->color;
  } else if(curlev->items[y][x]){
    type = curlev->items[y][x]->get_type();
    color = curlev->items[y][x]->get_color();
    if(type & WEAPON){
      symbol = '|';
    }else if(type & LIGHT){
      symbol = '_';
    }else if(type & OFFHAND){
      symbol = ')';
    }else if(type & RANGED){
      symbol = '}';
    }else if(type & ARMOR){
      symbol = '[';
    }else if(type & HELMET){
      symbol = ']';
    }else if(type & CLOAK){
      symbol = '(';
    }else if(type & GLOVES){
      symbol = '{';
    }else if(type & BOOTS){
      symbol = '\\';
    }else if(type & RING){
      symbol = '=';
    }else if(type & AMULET){
      symbol = '"';
    }else if(type & SCROLL){
      symbol = '~';
    }else if(type & BOOK){
      symbol = '?';
    }else if(type & FLASK){
      symbol = '!';
    }else if(type & GOLD){
      symbol = '$';
    }else if(type & AMMUNITION){
      symbol = '/';
    }else if(type & FOOD){
      symbol = ',';
    }else if(type & WAND){
      symbol = '-';
    }else if(type & CONTAINER){
      symbol = '%';
    }
  } else{
    symbol = curlev->chars[y][x];
    if(symbol == '<' || symbol == '>'){
      color = GREEN;
    }else {
      color = WHITE;
    }
  }

  attron(COLOR_PAIR(color));

  if(vis)
    attron(A_BOLD);
  else
    attroff(A_BOLD);

  return symbol;
}

character *Dungeon::get_character(int x, int y){
  return curlev->characters[y][x];
}

void Dungeon::set_character(int x, int y, character *c){
  curlev->characters[y][x] = c;
}

void Dungeon::set_message(std::string a){
  curlev->message = a;
}

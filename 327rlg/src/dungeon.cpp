#include "../headers/dungeon"
#include <string.h>
#include <curses.h>

Dungeon *Dungeon::d_instance = NULL;

Dungeon::level::level(int nummon, int up_stairs){

  if(generateMap(this->hardness, this->chars, &(this->rooms), &(this->numRooms))){
    printf("error making new map");
  }
  if(up_stairs){
    place_stairs(downstair, upstair, rooms, numRooms);
  } else{
    place_stairs(downstair, NULL, rooms, numRooms);
  }

  this->num_characters = nummon + 1;
  this->player_turn = 0;

  memset(this->visited, 0, sizeof (this->visited));
  memset(characters, 0, sizeof(characters));
  memset(items, 0, sizeof(items));

  init_heap(&(this->turn_order), compare_characters);
}

Dungeon::level::~level(){
  unsigned int i, j;
  character *c;
  Dungeon *dungeon = Dungeon::get_instance();

  c = (character *)turn_order->pop(turn_order);
  while(c){
    if(c != dungeon->get_player())
    delete c;
    c = (character *)turn_order->pop(turn_order);
  }

  turn_order->clear(turn_order);

  for(i = 0; i < mapHeight; i++){
    for(j = 0; j < mapWidth; j++){
      if(items[i][j]){
        delete items[i][j];
      }
    }
  }

  free(this->rooms);
}


Dungeon::Dungeon(int nummon){
  curlev = top = new level(nummon, 0);
  player = new pc(curlev->rooms, curlev->numRooms);
  mode = CONTROL;
}

Dungeon::~Dungeon(){
  Dungeon::level *level, *temp;

  level = this->top;

  curlev->characters[player->y][player->x] = NULL;
  delete player;

  while(level){
    temp = level->downstairs;
    delete level;
    level = temp;
  }
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
  x = player->x;
  y = player->y;

  if(curlev->upstair[0] == y && curlev->upstair[1] == x){
    curlev->characters[y][x] = NULL;
    if(curlev->upstairs){
      curlev = curlev->upstairs;
      add_player(0, 1);
    }
    return 1;
  }
  return 0;
}

int Dungeon::go_downstairs(){
  int x, y;
  Dungeon::level *temp;
  x = player->x;
  y = player->y;

  if(x == curlev->downstair[1] && y == curlev->downstair[0]){
    curlev->characters[y][x] = NULL;
    if(!curlev->downstairs){
      curlev->downstairs = new level(curlev->num_characters, 1);
      temp = curlev;
      curlev = curlev->downstairs;
      curlev->upstairs = temp;
      fill_level(1);
    }
    else{
      curlev = curlev->downstairs;
      add_player(1, 0);
    }
    return 1;
  }
  return 0;
}

void Dungeon::calculate_distances(){
  find_distances(curlev->hardness, curlev->distances, player->x, player->y, 0, 1);
  find_distances(curlev->hardness, curlev->distances_intel, player->x, player->y, 1, 1);
  find_distances(curlev->hardness, curlev->distances_notun, player->x, player->y, 1, 0);
}

int Dungeon::visible(int x, int y){
  int i, j, temp;

  i = player->x - x;
  j = player->y - y;

  if(player->equipment[LIGHT_SLOT]){
    temp = player->equipment[LIGHT_SLOT]->get_val();
  }else{
    temp = 0;
  }
  if(i <= (torch_distance + temp) && i >= -(torch_distance + temp) && j <= (torch_distance + temp) && j >= -(torch_distance + temp)){
    return 1;
  }
  return 0;
}

void Dungeon::add_player(int up, int down){
  int i, j;

  player->next_turn = curlev->player_turn;

  if(up){
    player->x = curlev->upstair[1];
    player->y = curlev->upstair[0];
  } else if(down){
    player->x = curlev->downstair[1];
    player->y = curlev->downstair[0];
  }

  curlev->characters[player->y][player->x] = player;

  curlev->x = player->x;
  curlev->y = player->y;

  for(i = player->y - torch_distance; i < player->y + 1 + torch_distance; i++){
    for(j = player->x - torch_distance; j < player->x + 1 + torch_distance; j++){
      if(j > 0 && j < mapWidth && i > 0 && i < mapHeight){
        this->visited(j,i) = 1;
      }
    }
  }
}

void Dungeon::fill_level(int up){
  unsigned int i, control = num_items;
  int x, y;
  npc *n;
  item *it;

  add_player(up, 0);

  x = curlev->x = player->x;
  y = curlev->y = player->y;

  (curlev->turn_order)->insert((curlev->turn_order), player);

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
}

char Dungeon::for_print(int x, int y){
  int color, vis;
  char symbol;

  vis = visible(x, y);

  if(!curlev->visited[y][x]){
    return ' ';
  }

  if(curlev->characters[y][x] && vis){
    symbol = curlev->characters[y][x]->sym;
    color = curlev->characters[y][x]->color;
  } else if(curlev->items[y][x]){
    symbol = curlev->items[y][x]->get_symb();
    color = curlev->items[y][x]->get_color();
  } else{
    if(x == curlev->upstair[1] && y == curlev->upstair[0]){
      color = GREEN;
      symbol = '<';
    }else if(x == curlev->downstair[1] && y == curlev->downstair[0]){
      color = GREEN;
      symbol = '>';
    }else if(mode & LOOK && x == curlev->x && y == curlev->y){
      symbol = curlev->chars[y][x];
      color = INVERT;
    }else {
      symbol = curlev->chars[y][x];
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

void Dungeon::show_map(){
  memset(curlev->visited, 1, sizeof (curlev->visited));
}

character *Dungeon::get_character(int x, int y){
  return curlev->characters[y][x];
}

void Dungeon::set_character(int x, int y, character *c){
  curlev->characters[y][x] = c;
}

void Dungeon::set_message(std::string a){
  message = a;
}

void Dungeon::equip(int input){
  int i, j;
  message = "Select an Inventory item to Equip";
  display1 = "Current item in this equipment slot: ";

  switch(input){
    case 'w':
      selected = 0;
      break;
    case 'h':
      //left
      if(selected > 0){
        selected--;
      }
      break;
    case 'l':
      //right
      if(selected < bag_size-1){
        selected++;
      }
      break;
    default:
      i = input - '0';
      if(i < bag_size && i >= 0 && player->bag[i]){
        if(player->equip(i)){
          message = "cannot equip that item";
          return;
        }else{
          message = "";
          display1 = "";
          display2 = "";
          return;
        }
      } else{
        message = "invalid input";
        return;
      }
  }
  if(player->bag[selected]){
    j = player->bag[selected]->get_type();
    display1 += player->equip_name(j);
  }
}

void Dungeon::unequip(int input){
  int i;
  message = "Select Equipment to Remove";
  display1 = "";

  switch(input){
    case 't':
      selected = 0;
      break;
    case 'h':
      //left
      if(selected > 0){
        selected--;
      }
      break;
    case 'l':
      //right
      if(selected < equip_slots - 1){
        selected++;
      }
      break;
    default:
      i = input - '0';
      if(input == '-'){
        i  = 10;
      } else if(input == '='){
        i = 11;
      }
      if(i < equip_slots && i >= 0 && player->equipment[i]){
        if(player->unequip(i)){
          display1 = "bag full remove item from bag first";
        } else{
          message = "";
          display1 = "";
          display2 = "";
        }
      } else{
        display1 = "invalid input";
      }
  }
}

void Dungeon::drop(int input){
  int i;
  message = "Select an Inventory item to Drop";
  display1 = "Drop an Item at your current location";

  switch(input){
    case 'd':
      selected = 0;
      break;
    case 'h':
      //left
      if(selected > 0){
        selected--;
      }
      break;
    case 'l':
      //right
      if(selected < bag_size-1){
        selected++;
      }
      break;
    default:
      i = input - '0';
      if(i < bag_size && i >= 0 && player->bag[i]){
        if(curlev->items[player->y][player->x]){
          display1 = "there is already an item on the floor move to an empty space first";
        }else{
          curlev->items[player->y][player->x] = player->bag[i];
          player->bag[i] = NULL;
          message = "";
          display1 = "";
          display2 = "";
          mode = CONTROL;
        }
      } else{
        display1 = "invalid input";
        return;
      }
  }
}

void Dungeon::destroy(int input){
  int i;
  message = "DELETE MENU";
  display1 = "Select an item to Delete";

  switch(input){
    case 'x':
      selected = 0;
      break;
    case 'h':
      //left
      if(selected > 0){
        selected--;
      }
      break;
    case 'l':
      //right
      if(selected < bag_size-1){
        selected++;
      }
      break;
    default:
      i = input - '0';
      if(i < bag_size && i >= 0 && player->bag[i]){
        delete (player->bag[i]);
        player->bag[i] = NULL;
        message = "";
        display1 = "";
        display2 = "";
      } else{
        display1 = "invalid input";
        return;
      }
  }
}

void Dungeon::inventory(int input){
  int j;
  message = "INVENTORY";
  display1 = "your inventory";

  switch(input){
    case 'i':
      selected = 0;
      break;
    case 'h':
      //left
      if(selected > 0){
        selected--;
      }
      break;
    case 'l':
      //right
      if(selected < bag_size-1){
        selected++;
      }
      break;
  }
  if(player->bag[selected]){
    j = player->bag[selected]->get_type();
    display1 = "Current item in this equipment slot: ";
    display1 += player->equip_name(j);
  }
}

void Dungeon::equipment(int input){
  message = "EQUIPMENT";
  display1 = "Your Gear";

  switch(input){
    case 'e':
      selected = 0;
      break;
    case 'h':
      //left
      if(selected > 0){
        selected--;
      }
      break;
    case 'l':
      //right
      if(selected < equip_slots - 1){
        selected++;
      }
      break;
  }
}

item *Dungeon::get_equipment(int index){
  return player->equipment[index];
}

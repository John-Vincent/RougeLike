#include "../headers/dungeon"
#include <string.h>

Dungeon *Dungeon::d_instance = NULL;

Dungeon::level::level(int nummon, int up_stairs){

  if(generateMap(this->hardness, this->chars, &(this->rooms), &(this->numRooms), up_stairs)){
    printf("error making new map");
  }

  this->characters = (character**) calloc( nummon + 1, sizeof (void*));

  this->num_characters = nummon + 1;

  memset(this->visited, 0, sizeof (this->visited));

  init_heap(&(this->turn_order), compare_characters);
}

Dungeon::level::~level(){
  unsigned int i;

  turn_order->clear(turn_order);

  for(i = 0; i < this->num_characters; i++){
    delete (this->characters[i]);
  }
  free(this->characters);
  free(this->rooms);
}

void Dungeon::init_dungeon(int nummon){
  d_instance = new Dungeon(nummon);
}

Dungeon *Dungeon::get_instance(){
  return d_instance;
}

char Dungeon::get_char_sym(int x, int y){
  unsigned int i;
  if( x >= (curlev->characters[0]->x - torch_distance) && x <= (curlev->characters[0]->x + torch_distance) &&
       y >= (curlev->characters[0]->y - torch_distance) && y <= (curlev->characters[0]->y + torch_distance) ){
    for(i = 0; i < curlev->num_characters; i++){
      if(curlev->characters[i]->x == x && curlev->characters[i]->y == y && curlev->characters[i]->attrib != 0xffffffff){
        return curlev->characters[i]->sym;
      }
    }
  }
  return 0;
}

int Dungeon::go_upstairs(){
  int x, y;
  char env;
  x = curlev->characters[0]->x;
  y = curlev->characters[0]->y;
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
  x = curlev->characters[0]->x;
  y = curlev->characters[0]->y;
  env = curlev->chars[y][x];
  if(env == '>'){
    if(!curlev->downstairs){
      curlev->downstairs = new level(curlev->num_characters, 1);
      curlev->downstairs->upstairs = curlev;
      curlev = curlev->downstairs;
      generate_characters();
      x = curlev->characters[0]->x;
      y = curlev->characters[0]->y;
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
  find_distances(curlev->hardness, curlev->distances, curlev->characters[0]->x, curlev->characters[0]->y, 0, 1);
  find_distances(curlev->hardness, curlev->distances_intel, curlev->characters[0]->x, curlev->characters[0]->y, 1, 1);
  find_distances(curlev->hardness, curlev->distances_notun, curlev->characters[0]->x, curlev->characters[0]->y, 1, 0);
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

  i = curlev->characters[0]->x - x;
  j = curlev->characters[0]->y - y;

  if(i <= torch_distance && i >= -torch_distance && j <= torch_distance && j >= -torch_distance){
    return 1;
  }
  return 0;
}


void Dungeon::generate_characters(){
  unsigned int i;

  curlev->characters[0] = new pc();

  for(i = 1; i < curlev->num_characters; i++){
    curlev->characters[i] = new npc(i);
  }

  for(i = 0; i < curlev->num_characters; i++){
    (curlev->turn_order)->insert((curlev->turn_order), curlev->characters[i]);
  }

  curlev->x = curlev->characters[0]->x;
  curlev->y = curlev->characters[0]->y;
}

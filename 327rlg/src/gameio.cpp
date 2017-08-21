#include <endian.h>
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include "../headers/constants.h"
#include "../headers/gameio.h"
#include "../headers/dungeon"
#include "../headers/character"
#include "../headers/dice"
#include "../headers/item"


/**
 * returns a string with the path to the file
 * @param  file_path               char pointer to be filled with the path
 * @return                         0 if successful -1 if not
 * @author Collin Vincent <collinvincent96@gmail.com>
 * @date   2017-02-01T22:52:22+000
 */
int getPath(char *file_path);

int saveGame(){
  Dungeon *dungeon = Dungeon::get_instance();
  const char *file_marker = "RLG327-S2017";
  uint32_t version = 110;
  char file_path[100];

  getPath(file_path);

  FILE *f = fopen(file_path, "w");
  if(!f){
    printf("failed to open file: %s\nerror: %s\n", file_path, strerror(errno));
    return -1;
  }

  version = htobe32(version);

  if(fwrite(file_marker, 1, 12, f) != 12){
    printf("failed to write file marker\nerror: %s\n", strerror(errno));
    return -1;
  }
  if(fwrite(&version, sizeof (version), 1, f) != 1){
    printf("failed to write version\nerror: %s\n", strerror(errno));
    return -1;
  }
  try{
    dungeon->save(f);
  } catch(const char *c){
    printf("failed to save game: %s", c);
  }

  return fclose(f);
}

int loadGame(){
  char file_marker[12];
  uint32_t version;
  errno = 0;

  char file_path[100];
  getPath(file_path);

  FILE *dungeon = fopen(file_path, "r");
  if(dungeon == NULL){
    printf("failed to open file: %s\nerror: %s\n", file_path, strerror(errno));
    return -1;
  }

  if(fread(file_marker, 1, 12, dungeon) != 12){
    printf("failed to read file marker\nerror: %s\n", strerror(errno));
    return -1;
  }
  if(fread(&version, sizeof (version), 1, dungeon) != 1){
    printf("failed to read version\nerror: %s\n", strerror(errno));
    return -1;
  }

  try{
    Dungeon::load_instance(dungeon);
  } catch(const char *c){
    printf("failed to load game: %s", c);
  }

  return fclose(dungeon);
}

void pc::save(FILE *f){
  int temp, i;
  unsigned int utemp;
  uint8_t stemp, item_count;

  temp = dam.get_base();
  temp = htobe32(temp);
  if(fwrite(&temp, sizeof (temp), 1, f) !=  1) throw strerror(errno);
  temp = dam.get_num();
  temp = htobe32(temp);
  if(fwrite(&temp, sizeof (temp), 1, f) !=  1) throw strerror(errno);
  temp = dam.get_sides();
  temp = htobe32(temp);
  if(fwrite(&temp, sizeof (temp), 1, f) !=  1) throw strerror(errno);
  temp = htobe32(hp);
  if(fwrite(&temp, sizeof (temp), 1, f) !=  1) throw strerror(errno);
  stemp = (uint8_t)this->color;
  if(fwrite(&stemp, sizeof (stemp), 1, f) !=  1) throw strerror(errno);
  temp = htobe32(speed);
  if(fwrite(&temp, sizeof (temp), 1, f) !=  1) throw strerror(errno);
  temp = htobe32(def);
  if(fwrite(&temp, sizeof (temp), 1, f) !=  1) throw strerror(errno);
  temp = htobe32(dodge);
  if(fwrite(&temp, sizeof (temp), 1, f) !=  1) throw strerror(errno);
  utemp = htobe32(attrib);
  if(fwrite(&utemp, sizeof (utemp), 1, f) !=  1) throw strerror(errno);
  stemp = (uint8_t)x;
  if(fwrite(&stemp, sizeof (stemp), 1, f) !=  1) throw strerror(errno);
  stemp = (uint8_t)y;
  if(fwrite(&stemp, sizeof (stemp), 1, f) !=  1) throw strerror(errno);

  item_count = 0;
  for(i = 0; i < bag_size; i++){
    if(bag[i]){
      item_count++;
    }
  }
  if(fwrite(&item_count, sizeof (item_count), 1, f) !=  1) throw strerror(errno);
  for(i = 0; i < bag_size; i++){
    if(bag[i]){
      bag[i]->save(f);
    }
  }
  item_count = 0;
  for(i = 0; i < equip_slots; i++){
    if(equipment[i]){
      item_count++;
    }
  }
  if(fwrite(&item_count, sizeof (item_count), 1, f) !=  1) throw strerror(errno);
  for(i = 0; i < equip_slots; i++){
    if(equipment[i]){
      equipment[i]->save(f);
    }
  }
}

void npc::save(FILE *f){
  int temp;
  unsigned int utemp;

  uint8_t stemp;
  stemp = name.length();
  if(fwrite(&stemp, sizeof (stemp), 1, f) !=  1) throw strerror(errno);
  if(fwrite(name.c_str(), sizeof (char), stemp, f) !=  stemp) throw strerror(errno);
  stemp = desc.length();
  if(fwrite(&stemp, sizeof (stemp), 1, f) !=  1) throw strerror(errno);
  if(fwrite(desc.c_str(), sizeof (char), stemp, f) !=  stemp) throw strerror(errno);
  temp = htobe32(gen);
  if(fwrite(&temp, sizeof (temp), 1, f) !=  1) throw strerror(errno);
  if(fwrite(&sym, sizeof (char), 1, f) !=  1) throw strerror(errno);
  temp = dam.get_base();
  temp = htobe32(temp);
  if(fwrite(&temp, sizeof (temp), 1, f) !=  1) throw strerror(errno);
  temp = dam.get_num();
  temp = htobe32(temp);
  if(fwrite(&temp, sizeof (temp), 1, f) !=  1) throw strerror(errno);
  temp = dam.get_sides();
  temp = htobe32(temp);
  if(fwrite(&temp, sizeof (temp), 1, f) !=  1) throw strerror(errno);
  temp = htobe32(hp);
  if(fwrite(&temp, sizeof (temp), 1, f) !=  1) throw strerror(errno);
  stemp = (uint8_t)this->color;
  if(fwrite(&stemp, sizeof (stemp), 1, f) !=  1) throw strerror(errno);
  temp = htobe32(speed);
  if(fwrite(&temp, sizeof (temp), 1, f) !=  1) throw strerror(errno);
  temp = htobe32(def);
  if(fwrite(&temp, sizeof (temp), 1, f) !=  1) throw strerror(errno);
  temp = htobe32(dodge);
  if(fwrite(&temp, sizeof (temp), 1, f) !=  1) throw strerror(errno);
  utemp = htobe32(attrib);
  if(fwrite(&utemp, sizeof (utemp), 1, f) !=  1) throw strerror(errno);
  stemp = (uint8_t)x;
  if(fwrite(&stemp, sizeof (stemp), 1, f) !=  1) throw strerror(errno);
  stemp = (uint8_t)y;
  if(fwrite(&stemp, sizeof (stemp), 1, f) !=  1) throw strerror(errno);
}

void item::save(FILE *f){
  int temp;

  uint8_t stemp;
  stemp = name.length();
  if(fwrite(&stemp, sizeof (stemp), 1, f) !=  1) throw strerror(errno);
  if(fwrite(name.c_str(), sizeof (char), stemp, f) !=  stemp) throw strerror(errno);
  stemp = desc.length();
  if(fwrite(&stemp, sizeof (stemp), 1, f) !=  1) throw strerror(errno);
  if(fwrite(desc.c_str(), sizeof (char), stemp, f) !=  stemp) throw strerror(errno);
  temp = htobe32(type);
  if(fwrite(&temp, sizeof (temp), 1, f) !=  1) throw strerror(errno);
  stemp = (uint8_t)this->color;
  if(fwrite(&stemp, sizeof (stemp), 1, f) !=  1) throw strerror(errno);
  temp = dam.get_base();
  temp = htobe32(temp);
  if(fwrite(&temp, sizeof (temp), 1, f) !=  1) throw strerror(errno);
  temp = dam.get_num();
  temp = htobe32(temp);
  if(fwrite(&temp, sizeof (temp), 1, f) !=  1) throw strerror(errno);
  temp = dam.get_sides();
  temp = htobe32(temp);
  if(fwrite(&temp, sizeof (temp), 1, f) !=  1) throw strerror(errno);
  temp = htobe32(hit);
  if(fwrite(&temp, sizeof (temp), 1, f) !=  1) throw strerror(errno);
  temp = htobe32(dodge);
  if(fwrite(&temp, sizeof (temp), 1, f) !=  1) throw strerror(errno);
  temp = htobe32(def);
  if(fwrite(&temp, sizeof (temp), 1, f) !=  1) throw strerror(errno);
  temp = htobe32(weight);
  if(fwrite(&temp, sizeof (temp), 1, f) !=  1) throw strerror(errno);
  temp = htobe32(speed);
  if(fwrite(&temp, sizeof (temp), 1, f) !=  1) throw strerror(errno);
  temp = htobe32(attr);
  if(fwrite(&temp, sizeof (temp), 1, f) !=  1) throw strerror(errno);
  temp = htobe32(val);
  if(fwrite(&temp, sizeof (temp), 1, f) !=  1) throw strerror(errno);
}

void Dungeon::save(FILE *f){
  Dungeon::level *l;

  if(fwrite(&floor_num, sizeof (floor_num), 1, f) != 1){
    throw strerror(errno);
  }

  player->save(f);
  l = top;
  while(l){
    l->save(f, player);
    l = l->downstairs;
  }
}

void Dungeon::level::save(FILE *f, character *p){
  int k;
  uint8_t i, j;

  if(fwrite(upstair, sizeof (*upstair), 2, f) != 2) throw strerror(errno);
  if(fwrite(downstair, sizeof (*downstair), 2, f) != 2) throw strerror(errno);
  if(fwrite(visited, sizeof (**visited), mapWidth * mapHeight, f) != mapWidth * mapHeight) throw strerror(errno);
  if(fwrite(hardness, sizeof (**hardness), mapWidth * mapHeight, f) != mapWidth*mapHeight) throw strerror(errno);
  if(fwrite(chars, sizeof (**chars), mapWidth*mapHeight, f) != mapWidth*mapHeight) throw strerror(errno);
  k = htobe32(num_characters);
  if(fwrite(&k, sizeof (k), 1, f) != 1) throw strerror(errno);
  if(fwrite(&numRooms, sizeof (numRooms), 1, f) != 1) throw strerror(errno);
  if(fwrite(rooms, sizeof (*rooms), numRooms, f) != numRooms) throw strerror(errno);

  k = 0;
  for(i = 0; i < mapHeight; i++){
    for(j = 0; j < mapWidth; j++){
      if(characters[i][j] && characters[i][j] != p)
        k++;
    }
  }
  k = htobe32(k);
  if(fwrite(&k, sizeof (k), 1, f) != 1) throw strerror(errno);
  for(i = 0; i < mapHeight; i++){
    for(j = 0; j < mapWidth; j++){
      if(characters[i][j] && characters[i][j] != p)
        characters[i][j]->save(f);
    }
  }

  k = 0;
  for(i = 0; i < mapHeight; i++){
    for(j = 0; j < mapWidth; j++){
      if(items[i][j])
        k++;
    }
  }
  k = htobe32(k);
  if(fwrite(&k, sizeof (k), 1, f) != 1) throw strerror(errno);
  for(i = 0; i < mapHeight; i++){
    for(j = 0; j < mapWidth; j++){
      if(items[i][j]){
        if(fwrite(&i, sizeof (i), 1, f) != 1) throw strerror(errno);
        if(fwrite(&j, sizeof (j), 1, f) != 1) throw strerror(errno);
        items[i][j]->save(f);
      }
    }
  }
}

Dungeon::Dungeon(FILE *f){
  Dungeon::level *l, *d;
  int i;

  if(fread(&floor_num, sizeof (floor_num), 1, f) != 1) throw strerror(errno);
  player = new pc(f);

  l = top = new Dungeon::level(f);
  try{
    while(1){
      d = new Dungeon::level(f);
      d->upstairs = l;
      l->downstairs = d;
      d = l;
    }
  } catch(int i){

  }
  selected = 0;
  mode = CONTROL;
  curlev = top;
  for(i = 0; i < floor_num; i++){
    curlev = curlev->downstairs;
  }
  add_player(0,0);
}

Dungeon::level::level(FILE *f){
  int num, i;
  uint8_t pos[2];
  npc *n;
  item *it;

  if(fread(upstair, sizeof(*upstair), 2, f) != 2){
    if(feof(f)){
      throw num;
    } else{
      throw strerror(errno);
    }
  }

  memset(characters, 0, sizeof(characters));
  memset(items, 0, sizeof(items));
  init_heap(&(this->turn_order), compare_characters);

  if(fread(downstair, sizeof(*downstair), 2, f) != 2) throw strerror(errno);
  if(fread(visited, sizeof (**visited), mapWidth * mapHeight, f) != mapWidth * mapHeight) throw strerror(errno);
  if(fread(hardness, sizeof (**hardness), mapWidth * mapHeight, f) != mapWidth*mapHeight) throw strerror(errno);
  if(fread(chars, sizeof (**chars), mapWidth*mapHeight, f) != mapWidth*mapHeight) throw strerror(errno);
  if(fread(&num_characters, sizeof (num_characters), 1, f) != 1) throw strerror(errno);
  num_characters = be32toh(num_characters);
  if(fread(&numRooms, sizeof (numRooms), 1, f) != 1) throw strerror(errno);
  rooms = (room_t *) malloc((sizeof (*rooms)) * numRooms);
  if(!rooms) throw "could not allocate space for rooms";
  if(fread(rooms, sizeof (*rooms), numRooms, f) != numRooms) throw strerror(errno);

  if(fread(&num, sizeof (num), 1, f) != 1) throw strerror(errno);
  num = be32toh(num);
  for(i = 0; i < num; i++){
    n = new npc(f);
    characters[n->y][n->x] = n;
    turn_order->insert(turn_order, n);
  }

  if(fread(&num, sizeof (num), 1, f) != 1) throw strerror(errno);
  num = be32toh(num);
  for(i = 0; i < num; i++){
    if(fread(pos, sizeof (*pos), 2, f) != 2) throw strerror(errno);
    it = new item(f);
    items[pos[0]][pos[1]] = it;
  }
  downstairs = NULL;
  upstairs = NULL;
}

item::item(FILE *f){
  int temp[7];
  char c[1000];
  uint8_t stemp;

  if(fread(&stemp, sizeof (stemp), 1, f) != 1) throw strerror(errno);
  if(fread(c, sizeof (*c), stemp, f) != stemp) throw strerror(errno);
  name = c;
  if(fread(&stemp, sizeof (stemp), 1, f) != 1) throw strerror(errno);
  if(fread(c, sizeof (*c), stemp, f) != stemp) throw strerror(errno);
  desc = c;
  if(fread(temp, sizeof (*temp), 1, f) != 1) throw strerror(errno);
  type = be32toh(temp[0]);
  if(fread(&stemp, sizeof (stemp), 1, f) != 1) throw strerror(errno);
  color = (int)stemp;
  if(fread(temp, sizeof (*temp), 3, f) != 3) throw strerror(errno);
  dam = dice(be32toh(temp[0]), be32toh(temp[1]), be32toh(temp[2]));
  if(fread(temp, sizeof(*temp), 7, f) != 7) throw strerror(errno);
  hit = be32toh(temp[0]);
  dodge = be32toh(temp[1]);
  def = be32toh(temp[2]);
  weight = be32toh(temp[3]);
  speed = be32toh(temp[4]);
  attr = be32toh(temp[5]);
  val = be32toh(temp[6]);
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
}

pc::pc(FILE *f){
  int temp[3], type;
  unsigned int utemp;
  uint8_t stemp, i;
  item *it;

  for( i = 0; i < bag_size; i++){
    bag[i] = NULL;
    equipment[i]=NULL;
  }
  for(i = bag_size; i < 12; i++){
    equipment[i] = NULL;
  }
  name = "YOU";
  desc = "Back at it again with the green ampersand";

  if(fread(temp, sizeof (*temp), 3, f) != 3)throw strerror(errno);
  dam = dice(be32toh(temp[0]), be32toh(temp[1]), be32toh(temp[2]));
  if(fread(temp, sizeof (*temp), 1, f) != 1) throw strerror(errno);
  hp = be32toh(temp[0]);
  if(fread(&stemp, sizeof (stemp), 1, f) != 1) throw strerror(errno);
  color = (int) stemp;
  if(fread(temp, sizeof (*temp), 3, f) != 3) throw strerror(errno);
  speed = be32toh(temp[0]);
  def = be32toh(temp[1]);
  dodge = be32toh(temp[2]);
  if(fread(&utemp, sizeof (utemp), 1, f) != 1) throw strerror(errno);
  attrib = be32toh(utemp);
  if(fread(&stemp, sizeof (stemp), 1, f) != 1) throw strerror(errno);
  x = stemp;
  if(fread(&stemp, sizeof (stemp), 1, f) != 1) throw strerror(errno);
  y = stemp;

  if(fread(&stemp, sizeof (stemp), 1, f) != 1) throw strerror(errno);
  for(i = 0; i < stemp; i++){
    bag[i] = new item(f);
  }
  if(fread(&stemp, sizeof (stemp), 1, f) != 1) throw strerror(errno);
  for(i = 0; i < stemp; i++){
    it = new item(f);
    type = it->get_type();
    if(type & WEAPON){
      equipment[WEAPON_SLOT] = it;
    }
    if(type & LIGHT){
      equipment[LIGHT_SLOT] = it;
    }
    if(type & OFFHAND){
      equipment[OFFHAND_SLOT] = it;
    }else if(type & RANGED){
      equipment[RANGED_SLOT] = it;
    }else if(type & ARMOR){
      equipment[ARMOR_SLOT] = it;
    }else if(type & HELMET){
      equipment[HELMET_SLOT] = it;
    }else if(type & CLOAK){
      equipment[CLOAK_SLOT] = it;
    }else if(type & GLOVES){
      equipment[GLOVES_SLOT] = it;
    }else if(type & BOOTS){
      equipment[BOOTS_SLOT] = it;
    }else if(type & RING){
      if(equipment[RING1_SLOT])
        equipment[RING1_SLOT] = it;
      else
        equipment[RING2_SLOT] = it;
    }else if(type & AMULET){
      equipment[AMULET_SLOT] = it;
    }
  }
  sym = '@';
  gen = 0;
  next_turn = 0;
}

npc::npc(FILE *f){
  int temp[3];
  unsigned int utemp;
  uint8_t stemp;
  char c[1000];

  if(fread(&stemp, sizeof (stemp), 1, f) != 1) throw strerror(errno);
  if(fread(c, sizeof (*c), stemp, f) != stemp) throw strerror(errno);
  name = c;
  if(fread(&stemp, sizeof (stemp), 1, f) != 1) throw strerror(errno);
  if(fread(c, sizeof (*c), stemp, f) != stemp) throw strerror(errno);
  desc = c;
  if(fread(temp, sizeof (*temp), 1, f) != 1)
  gen = be32toh(temp[0]);
  if(fread(&sym, sizeof (sym), 1, f) != 1) throw strerror(errno);
  if(fread(temp, sizeof (*temp), 3, f) != 3)throw strerror(errno);
  dam = dice(be32toh(temp[0]), be32toh(temp[1]), be32toh(temp[2]));
  if(fread(temp, sizeof (*temp), 1, f) != 1) throw strerror(errno);
  hp = be32toh(temp[0]);
  if(fread(&stemp, sizeof (stemp), 1, f) != 1) throw strerror(errno);
  color = (int) stemp;
  if(fread(temp, sizeof (*temp), 3, f) != 3) throw strerror(errno);
  speed = be32toh(temp[0]);
  def = be32toh(temp[1]);
  dodge = be32toh(temp[2]);
  if(fread(&utemp, sizeof (utemp), 1, f) != 1) throw strerror(errno);
  attrib = be32toh(utemp);
  if(fread(&stemp, sizeof (stemp), 1, f) != 1) throw strerror(errno);
  x = stemp;
  if(fread(&stemp, sizeof (stemp), 1, f) != 1) throw strerror(errno);
  y = stemp;
  next_turn = 0;
}

int getPath(char *file_path){
  char *env = getenv("HOME");
  if(env == NULL){
    printf("could not get home directory\n");
    return -1;
  }
  strcpy(file_path, env);
  strcat(file_path, "/.rlg327/dungeon");
  return 0;
}

#include "../headers/character"
#include "../headers/path_finder.h"
#include "../headers/dungeon"
#include "../headers/item"
#include <sstream>


npc::npc(std::string name, std::string desc, int gen, int speed, dice dam, int hp, char sym, int attrib, int color){
  Dungeon *dungeon;
  room_t* r;
  int a, x, y, flag;
  x = 0;
  y = 0;

  dungeon = Dungeon::get_instance();
  r = dungeon->get_room(0);

  this->name = name;
  this->sym = sym;
  this->desc = desc;
  this->hp = hp;
  this->gen = gen;
  this->dodge = 0;
  this->speed = speed;
  this->dam = dam;
  this->def = 0;
  this->color = color;
  this->attrib = (unsigned int) attrib;
  this->next_turn = 0;
  r = r + (rand() % dungeon->get_num_rooms());
  a = (r->width)*(r->height);
  flag = 0;
  do{
    if(a > 0){
      x = rand() % (r->width+1) + r->xPos;
      y = rand() % (r->height+1) + r->yPos;
    } else{
      x = rand() % (mapWidth-2) + 1;
      y = rand() % (mapHeight-2) + 1;
    }
    a--;

    if(dungeon->get_character(x, y)){
        flag = 1;
    }
  } while(flag);

  this->x = x;
  this->y = y;

}

int npc::take_turn(int input){
  Dungeon *dungeon;
  character *c;
  item *it;
  std::string str;
  std::stringstream ss;
  int x, y, rando, i, j;
  uint8_t hardness;
  unsigned int dist;
  uint16_t *distance;
  dist = 0x0FFFFFFF;

  dungeon = Dungeon::get_instance();

  x = 0;
  i = 0;
  j = 0;
  y = 0;
  it = NULL;
  distance = dungeon->get_distance_array(this->attrib);

  rando = rand() & 0x1;
  if( !(this->attrib & erratic && rando) &&
    (this->attrib & telepathic || dungeon->can_see_player(this->x, this->y))){
    for(i = this->y-1; i < this->y +2; i++){
      for(j = this->x - 1; j < this->x + 2; j++){
        if((i == this->y||j == this->x) && (i != this->y || j != this->x)){
          if(*(distance + i*mapWidth + j) < dist){
            if(!dungeon->get_hardness(j, i)){
              x = j;
              y = i;
              dist = *(distance + i * mapWidth + j);
            } else if(this->attrib & tunneler){
              x = j;
              y = i;
              dist = *(distance + i*mapWidth + j);
            }
          }
        }
      }
    }
  }
  if(!x){
    x = 0;
    while(!x){
      if(rand() & 1){
        x = rand() % 3 - 1 + this->x;
        y = this->y;
      }
      else{
        y = rand() % 3 - 1 + this->y;
        x = this->x;
      }
      if(dungeon->get_hardness(x, y)){
        if(!(this->attrib & tunneler && attrib & pass && x > 0 && x<mapWidth-1 && y>0 && y<mapHeight-1)){
          x = 0;
        }
      } else if(!(y - this->y||x - this->x) || !(!y - this->y||!x - this->x)){
        x=0;
      }
    }
  }

  this->next_turn += (turnbias / this->speed);
  hardness = dungeon->get_hardness(x,y);
  if(hardness && !(attrib & pass)){
    if(hardness > 85){
      dungeon->get_hardness(x, y) -= 85;
      dungeon->calculate_distances();
      return 0;
    }
    dungeon->get_hardness(x,y) = 0;
    dungeon->get_environment(x, y) = '#';
    dungeon->calculate_distances();
  }

  dungeon->set_character(this->x, this->y, NULL);
  c = dungeon->get_character(x, y);
  if(c){

    if(c == dungeon->get_player()){

      rando = (this->dam.roll() - c->get_def());
      if(rando > 0){
        c->hp -= rando;
        ss << dungeon->get_display2();
        ss << rando << " damage taken from: " << this->name << ", ";
        str = ss.str();
        if(str.length() < 78)
          dungeon->set_display2(str);
      } else{
        ss << 0 << " damage taken from: " << this->name << ", ";
        str = ss.str();
        if(str.length() < 78)
          dungeon->set_display2(str);
      }
      if(c->hp < 0){
        str = "Killed by a: ";
        str += this->name;
        str += ", ";
        dungeon->set_message(str);
        dungeon->show_map();
        c->attrib = 0xffffffff;
      } else{
        dungeon->set_character(this->x, this->y, this);
        return 0;
      }
    } else{

      if(!c->displace() && c->hp > this->hp){
        dungeon->set_character(this->x, this->y, this);
        return 0;
      }
    }
  }

  it = dungeon->get_item(x, y);
  if(this->attrib & DEST && it){
    dungeon->remove_item(x, y);
  }
  if(this->attrib & pickup && it){
    this->speed += it->get_speed();
    this->hp += it->get_hit();
    this->dodge += it->get_dodge();
    this->def += it->get_def();
    this->attrib = this->attrib | it->get_attr();
    dungeon->remove_item(x, y);
  }

  this->x = x;
  this->y = y;
  dungeon->set_character(x, y, this);

  return 0;

}

#include "../headers/character"
#include "../headers/map.h"
#include "../headers/dungeon"

int character::compare(const character &other) const{
    int dif = this->next_turn - other.next_turn;
    if(dif != 0){
      return dif;
    }
    return this->get_gen() - other.get_gen();
}

bool character::displace(){
  Dungeon *d = Dungeon::get_instance();
  int i, set = 0, x, y, start;
  static const int lookup[8][2] =
  {
    {-1, -1},
    {-1, 0},
    {-1, 1},
    {0, -1},
    {0, 1},
    {1, -1},
    {1, 0},
    {1, -1}
  };
  i = start = rand() % 8;

  while(!set){
    x = this->x + lookup[i][0];
    y = this-> y + lookup[i][1];
    if(!d->get_hardness(x, y) && !d->get_character(x, y)){
      set = 1;
    } else{
      i++;
      if(i > 7){
        i = 0;
      }
      if(i == start){
        return false;
      }
    }
  }
  this->x = x;
  this->y = y;
  d->set_character(x, y, this);
  return true;
}

int compare_characters(void *child, void *parent){
  character *one;
  character *two;
  one = (character *) child;
  two = (character *) parent;
  return two->compare(*one);
}

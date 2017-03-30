#include "../headers/character"

int character::compare(const character &other) const{
    int dif = this->next_turn - other.next_turn;
    if(dif != 0){
      return dif;
    }
    return this->get_gen() - other.get_gen();
}


int compare_characters(void *child, void *parent){
  character *one;
  character *two;
  one = (character *) child;
  two = (character *) parent;
  return two->compare(*one);
}

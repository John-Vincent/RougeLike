#include "../headers/character"
#include "../headers/path_finder.h"
#include "../headers/dungeon"
#include "../headers/constants.h"
#include <sstream>


pc::pc(room_t *r, int num_rooms){
  int pos, span, i;

  color = GREEN;
  sym = '@';
  hp = 10;
  def = 0;
  dodge = 0;
  name = "YOU";
  desc = "a loser";
  this->speed = 10;
  this->attrib = 0x00;
  this->gen = 0;
  this->dam = dice(0,1,4);
  for( i = 0; i < bag_size; i++){
    bag[i] = NULL;
    equipment[i]=NULL;
  }
  for(i = bag_size; i < 12; i++){
    equipment[i] = NULL;
  }

  r = r + rand() % num_rooms;
  pos = r->xPos;
  span = r->width;
  x = rand() % span + pos;
  pos = r->yPos;
  span = r->height;
  y = rand() % span + pos;
}

pc::~pc(){
  int i;

  for(i = 0; i < bag_size; i++){
    if(bag[i]){
      delete bag[i];
    }
    if(equipment[i]){
      delete equipment[i];
    }
  }

  for(i = bag_size; i < equip_slots; i++){
    if(equipment[i]){
      delete (equipment[i]);
    }
  }
}

int pc::take_turn(int input){
  Dungeon *dungeon;
  character *c;
  item *it;
  std::stringstream ss;
  int x, y, mode, temp;
  int i, j;

  dungeon = Dungeon::get_instance();
  mode = dungeon->get_mode();

  x = dungeon->get_x();
  y = dungeon->get_y();

  switch(input){
    // 7 || y
    case '7':
    case 'y':
      if(mode == CONTROL){
        x = this->x - 1;
        y = this->y - 1;
      }
      break;
    // 8 || k
    case '8':
    case 'k':
      if(mode == CONTROL){
        x = this->x;
        y = this->y - 1;
      }else if(mode == LOOK){
        y -= look_scroll;
      }
      break;
    // 9 || u
    case '9':
    case 'u':
      if(mode == CONTROL){
        x = this->x + 1;
        y = this->y - 1;
      }
      break;
    // 6 || l
    case '6':
    case 'l':
      if(mode == CONTROL){
        x = this->x + 1;
        y = this->y;
      } else if(mode == LOOK){
        x += look_scroll;
      }
      break;
    // 3 || n
    case 'n':
    case '3':
      if(mode == CONTROL){
        x = this->x + 1;
        y = this->y + 1;
      }
      break;
    // 2 || j
    case '2':
    case 'j':
      if(mode == CONTROL){
        x = this->x;
        y = this->y + 1;
      } else if(mode == LOOK){
        y += look_scroll;
      }
      break;
    // 1 || b
    case '1':
    case 'b':
      if(mode == CONTROL){
        x = this->x - 1;
        y = this->y + 1;
      }
      break;
    // 4 || h
    case '4':
    case 'h':
      if(mode == CONTROL){
        x = this->x - 1;
        y = this->y;
      } else if(mode == LOOK){
        x -= look_scroll;
      }
      break;
    // 5 || space
    case '5':
    case ' ':
      if(mode == CONTROL){
        x = this->x;
        y = this->y;
      }
      break;
    case 27:
      return 1;
  }

  if(mode == CONTROL){ //control mode
    this->next_turn += (turnbias / this->get_speed());
    if(!dungeon->get_hardness(x, y)){

      c = dungeon->get_character(x, y);
      if(c && c != this){
        temp = (this->get_dam() - c->get_def());
        if(temp < 0){
          temp = 0;
        }
        c->hp -= temp;
        ss << temp << " damage delt to: " << c->get_name() << ", ";
        dungeon->set_display1(ss.str());
        ss.clear();

        if(c->hp < 0){
          c->attrib = 0xFFFFFFFF;
        } else{
          return 0;
        }
      }
      it = dungeon->get_item(x, y);
      if(it){
        for(i = 0; i < bag_size; i++){
          if(!bag[i]){
            bag[i] = it;
            dungeon->set_item(x, y, NULL);
            i = bag_size;
          }
        }
      }

      dungeon->set_character(this->x, this->y, NULL);
      this->x = dungeon->set_x(x);
      this->y = dungeon->set_y(y);
      dungeon->set_character(x, y, this);

      if(equipment[LIGHT_SLOT]){
        temp = equipment[LIGHT_SLOT]->get_val();
      }else{
        temp = 0;
      }
      for(i = this->y - (torch_distance + temp); i < this->y + (torch_distance + temp); i++){
        for(j = this->x - (torch_distance + temp); j < this->x + (torch_distance + temp); j++){
          if(j > 0 && j < mapWidth && i > 0 && i < mapHeight){
            dungeon->visited(j, i) = 1;
          }
        }
      }

      dungeon->calculate_distances();
    } else{
      return 1;
    }

  } else if(mode == LOOK) { //look mode is on

    if(x < 0)
      x = 0;
    else if(x > mapWidth)
      x = mapWidth;

    if(y < 0)
      y = 0;
    else if(y > mapHeight)
      y = mapHeight;

    /*ss << "x=" << x << ", y=" << y << "| ";
    dungeon->set_message(ss.str());
    ss.clear();*/

    dungeon->set_x(x);
    dungeon->set_y(y);

  }

  c = dungeon->get_character(x, y);
  if(c && (c != this || (mode == LOOK) )){
    ss << "Monster: " << c->get_name() << ", ";
  }

  it = dungeon->get_item(x, y);
  if(it){
    ss << it->get_name() << ", ";
  }
  dungeon->set_message(ss.str());

  return 0;
}


int pc::equip(int index){
  item *it;
  int type, i, displaced, open;

  displaced = open = 0;

  if(index < 0 || index >= bag_size){
    return 1;
  }
  it = bag[index];
  if(!it){
    return 1;
  }
  type = it->get_type();
  bag[index] = NULL;
  for(i = 0; i < equip_slots; i++){
    if(i < bag_size && !bag[i]){
      open++;
    }
    if(equipment[i] && equipment[i]->get_type() & type){
      displaced++;
    }
  }
  if(open < displaced){
    bag[index] = it;
    return 1;
  }
  if(type & WEAPON){
    if(equipment[WEAPON_SLOT])
      unequip(WEAPON_SLOT);
    equipment[WEAPON_SLOT] = it;
  }
  if(type & OFFHAND){
    if(equipment[OFFHAND_SLOT])
      unequip(OFFHAND_SLOT);
    equipment[OFFHAND_SLOT] = it;
  }
  if(type & RANGED){
    if(equipment[RANGED_SLOT])
      unequip(RANGED_SLOT);
    equipment[RANGED_SLOT] = it;
  }
  if(type & ARMOR){
    if(equipment[ARMOR_SLOT])
      unequip(ARMOR_SLOT);
    equipment[ARMOR_SLOT] = it;
  }
  if(type & HELMET){
    if(equipment[HELMET_SLOT])
      unequip(HELMET_SLOT);
    equipment[HELMET_SLOT] = it;
  }
  if(type & CLOAK){
    if(equipment[CLOAK_SLOT])
      unequip(CLOAK_SLOT);
    equipment[CLOAK_SLOT] = it;
  }
  if(type & GLOVES){
    if(equipment[GLOVES_SLOT])
      unequip(GLOVES_SLOT);
    equipment[GLOVES_SLOT] = it;
  }
  if(type & BOOTS){
    if(equipment[BOOTS_SLOT])
      unequip(BOOTS_SLOT);
    equipment[BOOTS_SLOT] = it;
  }
  if(type & AMULET){
    if(equipment[AMULET_SLOT])
      unequip(AMULET_SLOT);
    equipment[AMULET_SLOT] = it;
  }
  if(type & LIGHT){
    if(equipment[LIGHT_SLOT])
      unequip(LIGHT_SLOT);
    equipment[LIGHT_SLOT] = it;
  }
  if(type & RING){
    if(equipment[RING1_SLOT]){
      if(equipment[RING2_SLOT]){
        unequip(RING2_SLOT);
      }
      equipment[RING2_SLOT] = it;
    }else{
      equipment[RING1_SLOT] = it;
    }
  }

  return 0;
}

int pc::unequip(int index){
  int b = -1, j;
  item *it;

  for(j = 0; j < bag_size; j++){
    if( !(bag[j]) ){
      b = j;
      j = bag_size;
    }
  }
  if(b == -1 || !(equipment[index]) ){
    return 1;
  } else{
    it = equipment[index];
    bag[b] = equipment[index];
    for(j = 0; j < equip_slots; j++){
      if(equipment[index] == it){
        equipment[index] = NULL;
      }
    }
  }
  return 0;
}

std::string pc::equip_name(int type){
  std::string ans = "";
  switch(type){
    case WEAPON:
      if(equipment[WEAPON_SLOT]){
        return equipment[WEAPON_SLOT]->get_name();
      }
    case OFFHAND:
      if(equipment[OFFHAND_SLOT]){
        return equipment[OFFHAND_SLOT]->get_name();
      }
    case RANGED:
      if(equipment[RANGED_SLOT]){
        return equipment[RANGED_SLOT]->get_name();
      }
      break;
    case ARMOR:
      if(equipment[ARMOR_SLOT]){
        return equipment[ARMOR_SLOT]->get_name();
      }
    case HELMET:
      if(equipment[HELMET_SLOT]){
        return equipment[HELMET_SLOT]->get_name();
      }
    case CLOAK:
      if(equipment[CLOAK_SLOT]){
        return equipment[CLOAK_SLOT]->get_name();
      }
    case GLOVES:
      if(equipment[GLOVES_SLOT]){
        return equipment[GLOVES_SLOT]->get_name();
      }
    case BOOTS:
      if(equipment[BOOTS_SLOT]){
        return equipment[BOOTS_SLOT]->get_name();
      }
    case AMULET:
      if(equipment[AMULET_SLOT]){
        return equipment[AMULET_SLOT]->get_name();
      }
    case LIGHT:
      if(equipment[LIGHT_SLOT]){
        return equipment[LIGHT_SLOT]->get_name();
      }
    case RING:
      if(equipment[RING1_SLOT]){
        if(equipment[RING2_SLOT]){
          return equipment[RING2_SLOT]->get_name();
        }
      }
  }
  return ans;
}

int pc::get_speed(){
  int s, i;
  s = this->speed;

  for(i = 0; i < equip_slots; i++){
    if(equipment[i]){
      s += equipment[i]->get_speed();
    }
  }
  return s;
}

int pc::get_dam(){
  int d, i;
  d = this->dam.roll();

  for(i = 0; i < equip_slots; i++){
    if(equipment[i]){
      d += equipment[i]->get_dam();
    }
  }
  return d;
}

int pc::get_def(){
  int d, i;
  d = this->def;

  for(i = 0; i < equip_slots; i++){
    if(equipment[i]){
      d += equipment[i]->get_def();
    }
  }
  return d;
}

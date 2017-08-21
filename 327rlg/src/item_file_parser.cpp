#include "../headers/item_file_parser"
#include "../headers/constants.h"
#include <cstring>
#include <fstream>
#include <cstdio>
#include <iostream>

item_template::item_template(){
  name = desc =  "";
  type = color = 0;
  hit = dam = dodge = def = weight = speed = attribute = val = NULL;
}

item_template::~item_template(){
  if(hit)
    delete hit;
  if(dam)
    delete dam;
  if(dodge)
    delete dodge;
  if(def)
    delete def;
  if(weight)
    delete weight;
  if(speed)
    delete speed;
  if(attribute)
    delete attribute;
  if(val)
    delete val;
}

void item_template::set_name(std::string n){
  name = n;
}

void item_template::set_desc(std::string d){
  desc = d;
}

void item_template::set_color(std::string c){
  color = 0;
  if(c.find("RED") != std::string::npos){
    color = RED;
  } else if(c.find("YELLOW") != std::string::npos){
    color = YELLOW;
  } else if(c.find("MAGENTA") != std::string::npos){
    color = MAGENTA;
  } else if(c.find("CYAN") != std::string::npos){
    color = CYAN;
  } else if(c.find("BLUE") != std::string::npos || c.find("BLACK") != std::string::npos){
    color = BLUE;
  }
}

void item_template::set_type(std::string a){
  type = 0;

  if(a.find("WEAPON") != std::string::npos){
    type = type | WEAPON;
  }
  if(a.find("LIGHT") != std::string::npos){
    type = type | LIGHT;
  }
  if(a.find("OFFHAND") != std::string::npos){
    type = type | OFFHAND;
  }else if(a.find("RANGED") != std::string::npos){
    type = type | RANGED;
  }else if(a.find("ARMOR") != std::string::npos){
    type = type | ARMOR;
  }else if(a.find("HELMET") != std::string::npos){
    type = type | HELMET;
  }else if(a.find("CLOAK") != std::string::npos){
    type = type | CLOAK;
  }else if(a.find("GLOVES") != std::string::npos){
    type = type | GLOVES;
  }else if(a.find("BOOTS") != std::string::npos){
    type = type | BOOTS;
  }else if(a.find("RING") != std::string::npos){
    type = type | RING;
  }else if(a.find("AMULET") != std::string::npos){
    type = type | AMULET;
  }else if(a.find("SCROLL") != std::string::npos){
    type = type | SCROLL;
  }else if(a.find("BOOK") != std::string::npos){
    type = type | BOOK;
  }else if(a.find("FLASK") != std::string::npos){
    type = type | FLASK;
  }else if(a.find("GOLD") != std::string::npos){
    type = type | GOLD;
  }else if(a.find("AMMUNITION") != std::string::npos){
    type = type | AMMUNITION;
  }else if(a.find("FOOD") != std::string::npos){
    type = type | FOOD;
  }else if(a.find("WAND") != std::string::npos){
    type = type | WAND;
  }else if(a.find("CONTAINER") != std::string::npos){
    type = type | CONTAINER;
  }

  if(type == 0){
    throw "no type";
  }
}

void item_template::set_hit(std::string h){
  int a, b, c;
  sscanf(h.c_str(), "HIT %d+%dd%d", &a, &b, &c);
  hit = new dice(a,b,c);
}

void item_template::set_dam(std::string d){
  int a, b, c;
  sscanf(d.c_str(), "DAM %d+%dd%d", &a, &b, &c);
  dam = new dice(a,b,c);
}

void item_template::set_dodge(std::string d){
  int a, b, c;
  sscanf(d.c_str(), "DODGE %d+%dd%d", &a, &b, &c);
  dodge = new dice(a,b,c);
}

void item_template::set_def(std::string d){
  int a, b, c;
  sscanf(d.c_str(), "DEF %d+%dd%d", &a, &b, &c);
  def = new dice(a,b,c);
}

void item_template::set_weight(std::string d){
  int a, b, c;
  sscanf(d.c_str(), "WEIGHT %d+%dd%d", &a, &b, &c);
  weight = new dice(a,b,c);
}

void item_template::set_speed(std::string d){
  int a, b, c;
  sscanf(d.c_str(), "SPEED %d+%dd%d", &a, &b, &c);
  speed = new dice(a,b,c);
}

void item_template::set_attrib(std::string d){
  int a, b, c;
  sscanf(d.c_str(), "ATTR %d+%dd%d", &a, &b, &c);
  attribute = new dice(a,b,c);
}

void item_template::set_val(std::string s){
  int a, b, c;
  sscanf(s.c_str(), "VAL %d+%dd%d", &a, &b, &c);
  val = new dice(a,b,c);
}

item *item_template::generate(){
  return new item(name, desc, type, color, hit->roll(), *dam, dodge->roll(), def->roll(), weight->roll(), speed->roll(), attribute->roll(), val->roll());
}

void item_template::print_out(){
  std::cout << "Name: " << name << std::endl;
  std::cout << "Description: " << desc;
  std::cout << "Color: " << color << std::endl;
  std::cout << "Type: ";
  if(type & WEAPON){
    std::cout << "WEAPON ";
  }
  if(type & LIGHT){
    std::cout << "LIGHT ";
  }
  if(type & OFFHAND){
    std::cout << "OFFHAND ";
  }else if(type & RANGED){
    std::cout << "RANGED ";
  }else if(type & ARMOR){
    std::cout << "ARMOR ";
  }else if(type & HELMET){
    std::cout << "HELMET ";
  }else if(type & CLOAK){
    std::cout << "CLOAK ";
  }else if(type & GLOVES){
    std::cout << "GLOVES ";
  }else if(type & BOOTS){
    std::cout << "BOOTS ";
  }else if(type & RING){
    std::cout << "RING ";
  }else if(type & AMULET){
    std::cout << "AMULET ";
  }else if(type & SCROLL){
    std::cout << "SCROLL ";
  }else if(type & BOOK){
    std::cout << "BOOK ";
  }else if(type & FLASK){
    std::cout << "FLASK ";
  }else if(type & GOLD){
    std::cout << "GOLD ";
  }else if(type & AMMUNITION){
    std::cout << "AMMUNITION ";
  }else if(type & FOOD){
    std::cout << "FOOD ";
  }else if(type & WAND){
    std::cout << "WAND ";
  }else if(type & CONTAINER){
    std::cout << "CONTAINER ";
  }
  std::cout << std::endl;
  std::cout << "Hit Bonus: " << hit->to_string() << std::endl;
  std::cout << "Damage Bonus: " << dam->to_string() << std::endl;
  std::cout << "Dodge Bonus: " << dodge->to_string() << std::endl;
  std::cout << "Defense Bonus: " << def->to_string() << std::endl;
  std::cout << "Speed Bonus: " << speed->to_string() << std::endl;
  std::cout << "Special Attribute: " << attribute->to_string() << std::endl;
  std::cout << "Weight: " << weight->to_string() << std::endl;
  std::cout << "Value: " << val->to_string() << std::endl << std::endl;
}

item_creator::item_creator(){
  std::string line, arg;
  char f_path[100];
  strcpy(f_path, getenv("HOME"));
  strcat(f_path, "/.rlg327/object_desc.txt");
  std::ifstream in(f_path);
  item_template *plate;
  int invalid = 0;
  templates = 0;

  std::getline(in, line);

  if(line != "RLG327 OBJECT DESCRIPTION 1"){
    std::cout << line << in.is_open() << std::endl;
    in.close();
    std::cout << "Cant find file: " << f_path << std::endl;
    throw "can't find file";
  }

  plate = new item_template();

  while(std::getline(in, line)){
    if(line == "BEGIN OBJECT"){
      while(std::getline(in, line) && line != "END" && !invalid){
        if(line.find("NAME ") != std::string::npos){
          plate->set_name(line.substr(5, std::string::npos));
        } else if(line.find("DESC") != std::string::npos){
          arg = "";
          while(std::getline(in, line) && line != "."){
            if(line.length() < 78){
              arg += line;
              arg.append("\n");
            } else{
              invalid = 1;
            }
          }
          if(!invalid){
            plate->set_desc(arg);
          }
        } else if(line.find("TYPE ") != std::string::npos){
          try{
            plate->set_type(line.substr(5, std::string::npos));
          } catch(...){
            invalid = 1;
          }
        } else if(line.find("COLOR ") != std::string::npos){
          plate->set_color(line.substr(6, std::string::npos));
        } else if(line.find("WEIGHT ") != std::string::npos){
          try{
            plate->set_weight(line);
          } catch(...){
            invalid = 1;
          }
        } else if(line.find("HIT ") != std::string::npos){
          try{
            plate->set_hit(line);
          } catch(...){
            invalid = 1;
          }
        } else if(line.find("ATTR ") != std::string::npos){
          try{
            plate->set_attrib(line);
          } catch(...){
            invalid = 1;
          }
        } else if(line.find("VAL ") != std::string::npos){
          try{
            plate->set_val(line);
          } catch(...){
            invalid = 1;
          }
        } else if(line.find("DAM ") != std::string::npos){
          try{
            plate->set_dam(line);
          } catch(...){
            invalid = 1;
          }
        } else if(line.find("DODGE ") != std::string::npos){
          try{
            plate->set_dodge(line);
          } catch(...){
            invalid = 1;
          }
        } else if(line.find("DEF ") != std::string::npos){
          try{
            plate->set_def(line);
          } catch(...){
            invalid = 1;
          }
        } else if(line.find("SPEED ") != std::string::npos){
          try{
            plate->set_speed(line);
          } catch(...){
            invalid = 1;
          }
        }
      }
      if(!invalid && plate->is_set()){
        list.push_back(plate);
        plate = new item_template();
        templates++;
      } else{
        delete plate;
        plate = new item_template();
      }
      invalid = 0;
    }
  }
  delete plate;
  in.close();
}

item_creator::~item_creator(){
  while(!list.empty()){
    delete list.back();
    list.pop_back();
  }
}

void item_creator::print_out(){
  std::vector<item_template*>::iterator it;

  for(it = list.begin(); it != list.end(); it++){
    (*it)->print_out();
  }
}

item *item_creator::get_item(){
  int x;
  x = rand() % list.size();
  return list[x]->generate();
}

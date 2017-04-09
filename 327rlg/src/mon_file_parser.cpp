#include "../headers/mon_file_parser"
#include <cstring>
#include <fstream>
#include <cstdio>
#include <iostream>


void monster_template::set_ability(std::string a){
  ability = 0;
  if(a.find("SMART") != std::string::npos){
    ability = ability | intelligent;
  }
  if(a.find("TELE") != std::string::npos){
    ability = ability | telepathic;
  }
  if(a.find("TUNNEL") != std::string::npos){
    ability = ability | tunneler;
  }
  if(a.find("PASS") != std::string::npos){
    ability = ability | pass;
  }
  if(a.find("ERRATIC") != std::string::npos){
    ability = ability | erratic;
  }
  if(a.find("PICKUP") != std::string::npos){
    ability = ability | pickup;
  }
  if(a.find("DESTROY") != std::string::npos){
    ability = ability | dest;
  }
}

monster_template::~monster_template(){
  if(speed){
    delete speed;
  }
  if(hp){
    delete hp;
  }
  if(dam){
    delete dam;
  }
}

npc *monster_template::generate(int gen){
  return new npc(name, desc, gen, speed->roll(), *dam, hp->roll(), symb, ability, color);
}

character_creator::~character_creator(){
  while(!list.empty()){
    delete list.back();
    list.pop_back();
  }
}

//todo
npc *character_creator::get_monster(int gen){
  int x;
  x = rand() % templates;
  return list[x]->generate(gen);
}

character_creator::character_creator(){
  std::string line, arg;
  char f_path[100];
  strcpy(f_path, getenv("HOME"));
  strcat(f_path, "/.rlg327/monster_desc.txt");
  std::ifstream in(f_path);
  monster_template *temp;
  int invalid, n, sy, c, de, sp, da, h, a, v1, v2, v3;

  temp = new monster_template();
  invalid = n = sy = c = de = sp = da = h = a = 0;

  std::getline(in, line);

  if(line != "RLG327 MONSTER DESCRIPTION 1")
    throw "wrong file";

  while(std::getline(in, line)){
    if(line == "BEGIN MONSTER"){
      while(std::getline(in, line) && line != "END" && !invalid){
        if(line.find("NAME ") == 0){
          if(!n){
            arg = line.substr(5, std::string::npos);
            invalid = temp->set_name(arg);
            n = 1;
          } else{
            invalid = 1;
          }
        }else if(line.find("SYMB ") == 0){
          if(!sy){
            invalid = temp->set_symb(line[5]);
            sy = 1;
          } else{
            invalid = 1;
          }
        }else if(line.find("COLOR ") == 0){
          if(!c){
            arg = line.substr(6, std::string::npos);
            invalid = temp->set_color(arg);
            c = 1;
          } else{
            invalid = 1;
          }
        }else if(line == "DESC"){
          if(!de){
            arg = "";
            while(std::getline(in, line) && line != "." && !invalid){
              if(line.length() < 78){
                arg += line;
                arg.append("\n");
              } else{
                invalid = 1;
              }
            }
            if(!invalid){
              invalid = temp->set_desc(arg);
            }
            de = 1;
          } else{
            invalid = 1;
          }
        }else if(line.find("SPEED ") == 0 && !sp){
          if(!sp){
            if(sscanf(line.c_str(), "SPEED %d+%dd%d", &v1, &v2, &v3) != 3){
              std::cout << "error with speed line\n" << line << std::endl;
              invalid = 1;
            }
            invalid = temp->set_speed(v1, v2, v3);
            sp = 1;
          } else{
            invalid = 1;
          }
        }else if(line.find("DAM ") == 0){
          if(!da){
            if(sscanf(line.c_str(), "DAM %d+%dd%d", &v1, &v2, &v3) != 3){
              std::cout << "error with damage line\n" << line << std::endl;
              invalid = 1;
            }
            invalid = temp->set_damage(v1, v2, v3);
            da = 1;
          } else{
            invalid = 1;
          }
        }else if(line.find("HP ") == 0){
          if(!h){
            if(sscanf(line.c_str(), "HP %d+%dd%d", &v1, &v2, &v3) != 3){
              std::cout << "error with HP line\n" << line << std::endl;
              invalid = 1;
            }
            invalid = temp->set_hp(v1, v2, v3);
            h = 1;
          } else{
            invalid = 1;
          }
        } else if(line.find("ABIL ") == 0){
          if(!a){
            temp->set_ability(line.substr(5, std::string::npos));
            a = 1;
          } else{
            invalid = 1;
          }
        }
      }
      if(!invalid && temp->is_set()){
        list.push_back(temp);
        templates++;
        temp = new monster_template();
      }
      invalid = n = sy = c = de = sp = da = h = a = 0;
    }
  }

  in.close();
  delete temp;
}

void monster_template::print_out(){
  std::cout << "Name: " << name << std::endl;
  std::cout << "Symbol: " << symb << std::endl;
  std::cout << "Description: " << desc;
  std::cout << "Color: " << color << std::endl;
  std::cout << "Speed: " << speed->to_string() << std::endl;
  std::cout << "Abilities: ";
  if(ability & intelligent)
    std::cout << "Intelligent ";
  if(ability & telepathic)
    std::cout << "Telepathic ";
  if(ability & tunneler)
    std::cout << "Tunneling ";
  if(ability & erratic)
    std::cout << "Erratic ";
  if(ability & pass)
    std::cout << "Non-Corporeal ";
  if(ability & pickup)
    std::cout << "Item-using ";
  if(ability & dest)
    std::cout << "Item-destroying ";
  std::cout << std::endl;
  std::cout << "HP: " << hp->to_string() << std::endl;
  std::cout << "Damage: " << dam->to_string() << std::endl << std::endl;
}

void character_creator::print_out(){
  std::vector<monster_template*>::iterator it;

  for(it = list.begin(); it != list.end(); it++){
    (*it)->print_out();
  }
}

#ifndef CPPCHARACTER
#define CPPCHARACTER


#include "constants.h"
#include "stdint.h"
#include  <stdlib.h>

#define intelligent 0x1
#define telepathic 0x2
#define tunneler 0x4
#define erratic 0x8

#define torch_distance 5

typedef struct dungeon dungeon_t;

#ifdef __cplusplus

class character{
  protected:
    int gen;
    dungeon_t *dungeon;
  public:
    char sym;
    int speed;
    int next_turn;
    unsigned int attrib;
    int x;
    int y;
    inline const int get_gen() const{
      return gen;
    }
    virtual int take_turn(int input) = 0;
    virtual ~character(){}
    int compare(const character &other) const;
};

class npc : public character{
  public:
    npc(dungeon_t *d, int gen);
    ~npc(){}
    int take_turn(int input);
};

class pc : public character{
  public:
    pc(dungeon_t *d);
    ~pc(){}
    int take_turn(int input);
};

#include "dungeon.h"

extern "C" {
#endif

#define turnbias 1000

#ifndef __cplusplus
typedef void character;
#endif

int generate_characters(dungeon_t *dungeon, int num_monsters);

int compare_characters(void *child, void *parent);

int take_turn(character *c, int input);

void set_turn(character *c);

int on_stairs(dungeon_t *dungeon);

int is_dead(character *c);

char get_char(dungeon_t *d, int x, int y);

void free_characters(dungeon_t *dungeon);

void set_dungeon_view(dungeon_t *d);


#ifdef __cplusplus
}
#endif

#endif

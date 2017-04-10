#ifndef CONSTANTS_H
#define CONSTANTS_H

#define screen_width  80
#define screen_height 21

#define mapWidth  160
#define mapHeight 105

#define Room_Min_Width  7
#define Room_Min_Height 5

#define Room_Min_Number  10
#define Room_Width_Range 20

#define Room_Height_Range 15
#define Room_Number_Range 10

#define default_monster_num 4;

#define turnbias 1000

#define look_scroll 1

#define intelligent 0x1
#define telepathic  0x2
#define tunneler    0x4
#define erratic     0x8
#define pass        0x10
#define pickup      0x20
#define dest        0x40

#define WEAPON     0x1
#define OFFHAND    0x2
#define RANGED     0x4
#define ARMOR      0x8
#define HELMET     0x10
#define CLOAK      0x20
#define GLOVES     0x40
#define BOOTS      0x80
#define AMULET     0x100
#define LIGHT      0x200
#define RING       0x400
#define SCROLL     0x800
#define BOOK       0x1000
#define FLASK      0x2000
#define GOLD       0x4000
#define AMMUNITION 0x8000
#define FOOD       0x10000
#define WAND       0x20000
#define CONTAINER  0x40000

#define WEAPON_SLOT  0
#define OFFHAND_SLOT 1
#define RANGED_SLOT  2
#define ARMOR_SLOT   3
#define HELMET_SLOT  4
#define CLOAK_SLOT   5
#define GLOVES_SLOT  6
#define BOOTS_SLOT   7
#define AMULET_SLOT  8
#define LIGHT_SLOT   9
#define RING1_SLOT   10
#define RING2_SLOT   11

#define WHITE   1
#define GREEN   2
#define RED     3
#define YELLOW  4
#define MAGENTA 5
#define CYAN    6
#define BLUE    7
#define INVERT  8

#define CONTROL   0x01
#define LOOK      0x02
#define WEAR      0x04
#define TAKE      0x08
#define DROP      0x10
#define EX        0x20
#define EQUIPMENT 0x40
#define INVENTORY 0x80

#define num_items 20

#define torch_distance 5

#define bag_size 10

#define equip_slots 12

#endif

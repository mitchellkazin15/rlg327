#ifndef OBJECT_H
#define OBJECT_H

typedef enum object_type {
  objtype_no_type,
  objtype_WEAPON,
  objtype_OFFHAND,
  objtype_RANGED,
  objtype_LIGHT,
  objtype_ARMOR,
  objtype_HELMET,
  objtype_CLOAK,
  objtype_GLOVES,
  objtype_BOOTS,
  objtype_AMULET,
  objtype_RING,
  objtype_SCROLL,
  objtype_BOOK,
  objtype_FLASK,
  objtype_GOLD,
  objtype_AMMUNITION,
  objtype_CONSUMABLE,
  objtype_WAND,
  objtype_CONTAINER
} object_type_t;

class object
{
public:
  string name;
  string desc;
  char symbol;
  uint32_t color;
  object_type_t type;
  dice* damage;
  object* next;
  int xcoord, ycoord;
  int str,dex,intel;//stat requirements
  int  hit, dodge, defence, weight, speed, attribute, value;
  
public:
  object();
  void printObject();
  void setColor();

};

#endif

#ifndef MONSTER_H
#define MONSTER_H

class monster
{
public:
  int m_type;
  int xcoord;
  int ycoord;
  int speed;
  int hp,hpmax;
  int exp;
  dice* damage;
  int status;
  int turn;
  char symbol;
  int lastSeenX;
  int lastSeenY;
  string name;
  string desc;
  string colors;
  int boss;
  int shop;
  bool aggressive;
  bool pickup;
  bool destroy;
  bool pass;
  int difficulty;
  int shopType;
  object *shopObjs[SHOP_SLOTS];

public:
  monster();
  monster(int type, char sym);
  void mon_move();
  void print_monster();
  void setColor();
  void attack();

private:
  void moveOnGradient();
  void moveOnGradientTunneling();
  void move_0(); //no attributes (random motion)
  void move_1(); //only smart
  void move_2(); //only telepathic
  void move_3(); //smart + telepathic
  void move_4(); //only tunneling
  void move_5(); //smart + tunneling
  void move_6(); //telepathic + tunneling
  void move_7(); //smart + telepathic + tunneling
  void move_8(); //only erratic
  void move_9(); //smart + erratic
  void move_a(); //telepathic + erratic
  void move_b(); //smart + telepathic + erratic
  void move_c(); //tunneling + erratic
  void move_d(); //smart + tunneling + erratic
  void move_e(); //telepathic + tunneling + erratic
  void move_f(); //smart + telepathic + tunneling + erratic

};

monster *monster_constructor(int type, char sym);
void monster_setXY(int x, int y, monster *npc);
int monster_getX(monster *npc);
int monster_getY(monster *npc);
void monster_setSpeed(int speed, monster *npc);
int monster_getSpeed(monster *npc);
void monster_setTurn(int turn, monster *npc);
int monster_getTurn(monster *npc);
void monster_setSymbol(char symbol, monster *npc);
char monster_getSymbol(monster *npc);
void monster_setStatus(int status, monster *npc);
int monster_getStatus(monster *npc);
void monster_move(monster *npc);
int monster_getType(monster *npc);

#endif

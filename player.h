#ifndef PLAYER_H
#define PLAYER_H

class player
{
public:
  string name;
  int xcoord;
  int ycoord;
  int speed;
  int status;
  int turn;
  int color;
  char symbol;
  int hp,hpmax;
  int stamina,stammax;
  int gold,ammo;
  int level,experience;
  int str,dex,vit,intel,endur,def;
  object* weapon,*offhand,*ranged,*armor,*helmet,*cloak,*gloves,*boots,*amulet,*light,*ring1,*ring2;
  object* carry[CARRY_SLOTS];
  object* consumables[CONSUMABLE_SLOTS];
  object* scrolls[SCROLL_SLOTS];

public:
  player();
  player(int x, int y);
  void pc_move();
  void printStats();
  void setColor();
  void attack(int x,int y);
  void printInventory();
  void printEquipment();
  void initStats();
  void wearItem();
  void dropItem();
  void expungeItem();
  void takeOff();
  void inspectItem();
  void levelUp();
  void consumeItem();
  void pickUpItem();
  int rangeAttack(int rangeX,int rangeY);
  void compareItem(object* ob);
  int useScroll();
};

player *player_constructor();
void player_setXY(int x, int y, player *pc);
int player_getX(player *pc);
int player_getY(player *pc);
void player_setSpeed(int speed,player *pc);
int player_getSpeed(player *pc);
void player_setTurn(int turn, player *pc);
int player_getTurn(player *pc);
void player_setSymbol(char symbol, player *pc);
char player_getSymbol(player *pc);
void player_setStatus(int status, player *pc);
int player_getStatus(player *pc);
void player_move(player *pc);
void initKnight();
void initBarbarian();
void initElf();
void initMage();
void initOrc();
void initPeasant();
void initAutobot();
bool isBorder(int x, int y);

#endif

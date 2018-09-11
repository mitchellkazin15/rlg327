#ifndef MONSTER_DESC_H
#define MONSTER_DESC_H

class monster_desc
{
public:
  string name;
  string desc;
  string color;
  char symbol;
  bool aggressive;
  int boss;
  int shop;
  vector<string>* abil;
  dice* speed;
  dice* hp;
  dice* dam;
public:
  monster_desc();
  void print_monster_desc();
  int getDifficulty();
  monster* getMonster();
};

vector<monster*>* read_monster_file(int numMon);

#endif

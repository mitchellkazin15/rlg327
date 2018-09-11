#ifndef DICE_H
#define DICE_H

class dice
{
public:
  int numdice;
  int numsides;
  int base;
public:
  dice();
  void print_dice();
  int roll();
  int average();
};

#endif

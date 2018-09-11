#include "core.h"

object::object(){
  next = 0;
  damage = 0;
  color = 0;
  xcoord = ycoord = 0;
  str = dex = intel = 0;
  hit = dodge = defence = weight = speed = attribute = value = 0;
}

void object::setColor(){
  if(color == COLOR_BLUE){
    attron(COLOR_PAIR(BL));
  }
  else if(color == COLOR_CYAN){
    attron(COLOR_PAIR(CY));
  }
  else if(color == COLOR_GREEN){
    attron(COLOR_PAIR(GRAS));
  }
  else if(color == COLOR_MAGENTA){
    attron(COLOR_PAIR(MAG));
  }
  else if(color == COLOR_RED){
    attron(COLOR_PAIR(MONST));
  }
  else if(color == COLOR_WHITE){
    attron(COLOR_PAIR(ILUM));
  }
  else if(color == COLOR_YELLOW){
    attron(COLOR_PAIR(YEL));
  }
  else if(color == COLOR_ROCK_ILUM){
    attron(COLOR_PAIR(R_ILUM));
  }
}

void object::printObject(){
  clear();
  printw("%s\n\n",name.c_str());
  printw("SYMBOL: ");
  this->setColor();
  printw("%c\n\n",symbol);
  attron(COLOR_PAIR(ILUM));
  printw("%s\n",desc.c_str());
  printw("REQUIREMENTS:\nSTR: %d DEX: %d INT: %d\n",str,dex,intel);
}
